/*
   (c) Copyright 2007  AvengerGear.com

   All rights reserved.

   This utility reference lib/direct/debug.c file in directfb source
   Please reference http://www.directfb.org

   Modified by Alex Lau <avengermojo@gmail.com> 

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/


#include <debug.h>
#include <linkedlist.h>

#if BUILD_UED_DEBUG

typedef struct {
     LinkedList  link;
     char       *name;
     bool        enabled;
} DebugSectionEntry;

/**************************************************************************************************/

static u32              sections_age  = 1;
static LinkedList      *sections      = NULL;

/**************************************************************************************************/

__attribute__((no_instrument_function))
static inline DebugSectionEntry *
lookup_section( const char *name, bool sub )
{
     DebugSectionEntry *entry;

     list_foreach(entry, sections) {
          if (! strcasecmp( entry->name, name ))
               return entry;
     }

     /*
      * spliting the section by '/'
      */
     if (sub && strchr(name, '/')) {
          int passed_name_len = strlen( name );

          list_foreach (entry, sections) {
               int entry_len = strlen( entry->name );
               if ((passed_name_len > entry_len) &&
                   (name[entry_len] == '/') &&
                   (! strncasecmp( entry->name, name, entry_len))) {
                    return entry;
               }
          }
     }
     return NULL;
}

__attribute__((no_instrument_function))
static inline bool
check_section( UEDDebugSection *section )
{
     if (section->age != sections_age) {
          DebugSectionEntry *entry = lookup_section( section->name, true );

          section->age = sections_age;

          if (entry) {
               section->registered = true;
               section->enabled    = entry->enabled;
          }
     }
     /* 
      * TODO: replace the false statement with the configurator->debug option; 
      */
     return section->registered ? section->enabled : false;
}

/**************************************************************************************************/

void
ued_debug_set_section( const char *name, bool enable )
{
     DebugSectionEntry *entry;

     /*
      * TODO: for mult-process / mult-thread enable, using the following 
      *    
      *  if( futex( &sections_lock, FUTEX_WAIT, 0 ) ){ ... }
      */

     entry = lookup_section( name, false );
     if (!entry) {
          entry = calloc( 1, sizeof(DebugSectionEntry) );
          if (!entry) {
               UED_DEBUG( "out of memory" );
               // pthread_mutex_unlock( &domains_lock );
               return;
          }

          entry->name = strdup( name );

          list_prepend( &sections, &entry->link );
     }
     entry->enabled = enable;

     if (! ++sections_age)
          sections_age++;

     /*
      *  if( futex( &sections_lock, FUTEX_WAKE, 1 ) ){ ... }
      */
}

/**************************************************************************************************/

__attribute__((no_instrument_function))
void
ued_debug( const char *format, ... )
{
     // char        buf[512];
     //long long   millis = direct_clock_get_millis();
     // const char *name   = direct_thread_self_name();

     va_list ap;

     va_start( ap, format );

     vfprintf( stderr , format, ap );

     va_end( ap );
}

__attribute__((no_instrument_function))
void
ued_debug_enter( UEDDebugSection *section,
		      const char *function,
		      const char *filename,
		      int         line,
		      const char *format, ... )
{
     if (check_section( section )) {
          char        buf[512];

          va_list     ap;
          va_start( ap, format );
          vsnprintf( buf, sizeof(buf), format, ap );
          va_end( ap );

          ued_debug( "<%s> Entering {%s()} %s [%s:%d]\n", section->name, function, buf, filename, line );  
     }
}

__attribute__((no_instrument_function))
void
ued_debug_exit( UEDDebugSection *section,
		     const char *function,
		     const char *filename,
		     int         line,
		     const char *format, ... )
{
     if (check_section( section )) {
          char        buf[512];

          va_list     ap;
          va_start( ap, format );
          vsnprintf( buf, sizeof(buf), format, ap );
          va_end( ap );

          ued_debug( "<%s> Exiting {%s()} %s [%s:%d]\n", section->name, function, buf, filename, line );  
     }
}

__attribute__((no_instrument_function))
void
signal_handler( int num, siginfo_t *info, void *foo )
{
    if( num == SIGTRAP ){
        ued_debug( "*** SIGTRAP *** \n" ); 
    }
}


__attribute__((no_instrument_function))
void
ued_assertion( const char *exp,
               const char *func,
               const char *file,
               int         line )
{
    ued_debug( "*** Assertion [%s] failed *** [%s:%d in %s()]\n", exp, file, line, func );
    raise( SIGTRAP );
}

#else // BUILD_UED_DEBUG
void
ued_debug_set_section( const char *name, bool enable )
{
}

#endif // BUILD_UED_DEBUG
