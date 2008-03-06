/*
   Copyright 2007  AvengerGear.com

   All rights reserved.

   This utility reference lib/direct/debug.h file in directfb source
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
#include <modules.h>

#ifdef PIC
#define DYNAMIC_LINKING
#endif

#ifdef DYNAMIC_LINKING
#include <dlfcn.h>
#endif

UED_DEBUG_SECTION( Modules, "Library/Modules", "Module loading and registration" );

/******************************************************************************/

#ifdef DYNAMIC_LINKING

static ModuleEntry *lookup_by_name( const ModuleDir *directory,
                                    const char      *name );

static ModuleEntry *lookup_by_file( const ModuleDir *directory,
                                    const char      *file );

static void *open_module  ( ModuleEntry *module );
static bool  load_module  ( ModuleEntry *module );
static void  unload_module( ModuleEntry *module );

#endif

/******************************************************************************/

static int
suppress_module (const char *name)
{
    int i = 0;
    if (!ued_config || !ued_config->disable_module)
        return 0;
    while (ued_config->disable_module[i]) {
        if (strcmp (ued_config->disable_module[i], name) == 0) {
            UED_DEBUG( "Library/Modules: suppress module '%s'", ued_config->disable_module[i] );
            return 1;
	}
	i++;
    }
    return 0;
}

void
modules_register( ModuleDir       *directory,
                  u32              abi_version,
                  const char      *name,
                  const void      *funcs )
{
     ModuleEntry *entry;

     UED_ASSERT( directory != NULL );
     UED_ASSERT( name != NULL );
     UED_ASSERT( funcs != NULL );

     UED_DEBUG_ENTER( Modules, "Registering '%s' ('%s')", name, directory->path );

#ifdef DYNAMIC_LINKING
     if ((entry = lookup_by_name( directory, name )) != NULL) {
          entry->loaded = true;
          entry->funcs  = funcs;

          return;
     }
#endif

     if (directory->loading)
          entry = directory->loading;
     else if (! (entry = calloc( 1, sizeof(ModuleEntry) )))
          return;

     entry->directory = directory;
     entry->loaded    = true;
     entry->name      = strdup( name );
     entry->funcs     = funcs;

     entry->disabled  = suppress_module( name );

     if (abi_version != directory->abi_version) {
          UED_DEBUG( "Library/Modules: ABI version of '%s' (%d) does not match %d!",
                   entry->file ? entry->file : entry->name,
                   abi_version, directory->abi_version );

          entry->disabled = true;
     }

     //D_MAGIC_SET( entry, ModuleEntry );

     list_prepend( &directory->entries, &entry->link );

     UED_DEBUG_EXIT( Modules, "...registered." );
}

int
modules_explore_directory( ModuleDir *directory )
{
#ifdef DYNAMIC_LINKING
     int            dir_len;
     DIR           *dir;
     struct dirent *entry = NULL;
     struct dirent  tmp;
     int            count = 0;

     UED_ASSERT( directory != NULL );
     UED_ASSERT( directory->path != NULL );

     dir_len = strlen( directory->path );
     dir     = opendir( directory->path );

     if (!dir) {
          UED_DEBUG( "Library/Modules: Could not open module directory `%s'!", directory->path );
          return 0;
     }

     while (readdir_r( dir, &tmp, &entry ) == 0 && entry) {
          void        *handle;
          ModuleEntry *module;
          int         entry_len = strlen(entry->d_name);

          if (entry_len < 4 ||
              entry->d_name[entry_len-1] != 'o' ||
              entry->d_name[entry_len-2] != 's')
               continue;

          if (lookup_by_file( directory, entry->d_name ))
               continue;


          module = calloc( 1, sizeof(ModuleEntry) );
          if (!module)
               continue;

          module->directory = directory;
          module->dynamic   = true;
          module->file      = strdup( entry->d_name );


          directory->loading = module;

          if ((handle = open_module( module )) != NULL) {
               if (!module->loaded) {
                    int    len;
                    void (*func)();

                    UED_DEBUG( "Library/Modules: Module '%s' did not register itself after loading! "
                             "Trying default module constructor...", entry->d_name );

                    len = strlen( entry->d_name );

                    entry->d_name[len-3] = 0;

                    func = dlsym( handle, entry->d_name + 3 );
                    if (func) {
                         func();

                         if (!module->loaded) {
                              UED_DEBUG( "Library/Modules: ... even did not register after "
                                       "explicitly calling the module constructor!" );
                         }
                    }
                    else {
                         UED_DEBUG( "Library/Modules: ... default contructor not found!" );
                    }

                    if (!module->loaded) {
                         module->disabled = true;

                         //D_MAGIC_SET( module, ModuleEntry );

                         list_prepend( &directory->entries,
                                              &module->link );
                    }
               }

               if (module->disabled) {
                    dlclose( handle );

                    module->loaded = false;
               }
               else {
                    module->handle = handle;

                    count++;
               }
          }
          else {
               module->disabled = true;

               //D_MAGIC_SET( module, ModuleEntry );

               list_prepend( &directory->entries, &module->link );
          }

          directory->loading = NULL;
     }

     closedir( dir );

     return count;
#else
     return 0;
#endif
}

const void *
module_ref( ModuleEntry *module )
{
     UED_ASSERT( module != NULL );

     if (module->disabled)
          return NULL;

#ifdef DYNAMIC_LINKING
     if (!module->loaded && !load_module( module ))
          return NULL;
#endif

     module->refs++;

     return module->funcs;
}

void
module_unref( ModuleEntry *module )
{
     // D_MAGIC_ASSERT( module, ModuleEntry );
     UED_ASSERT( module->refs > 0 );

     if (--module->refs)
          return;

#ifdef DYNAMIC_LINKING
     if (module->dynamic)
          unload_module( module );
#endif
}

/******************************************************************************/

#ifdef DYNAMIC_LINKING

static ModuleEntry *
lookup_by_name( const ModuleDir *directory,
                const char      *name )
{
     LinkedList *l;

     UED_ASSERT( directory != NULL );
     UED_ASSERT( name != NULL );

     list_foreach (l, directory->entries) {
          ModuleEntry *entry = (ModuleEntry*) l;

          // D_MAGIC_ASSERT( entry, ModuleEntry );

          if (!entry->name)
               continue;

          if (!strcmp( entry->name, name ))
               return entry;
     }

     return NULL;
}

static ModuleEntry *
lookup_by_file( const ModuleDir *directory,
                const char      *file )
{
     LinkedList *l;

     UED_ASSERT( directory != NULL );
     UED_ASSERT( file != NULL );

     list_foreach (l, directory->entries) {
          ModuleEntry *entry = (ModuleEntry*) l;

          // D_MAGIC_ASSERT( entry, ModuleEntry );

          if (!entry->file)
               continue;

          if (!strcmp( entry->file, file ))
               return entry;
     }

     return NULL;
}

static bool
load_module( ModuleEntry *module )
{
     //D_MAGIC_ASSERT( module, ModuleEntry );
     UED_ASSERT( module->dynamic == true );
     UED_ASSERT( module->file != NULL );
     UED_ASSERT( module->loaded == false );
     UED_ASSERT( module->disabled == false );

     module->handle = open_module( module );

     return module->loaded;
}

static void
unload_module( ModuleEntry *module )
{
     //D_MAGIC_ASSERT( module, ModuleEntry );
     UED_ASSERT( module->dynamic == true );
     UED_ASSERT( module->handle != NULL );
     UED_ASSERT( module->loaded == true );

     dlclose( module->handle );

     module->handle = NULL;
     module->loaded = false;
}

static void *
open_module( ModuleEntry *module )
{
     ModuleDir       *directory = module->directory;
     int              entry_len = strlen(module->file);
     int              buf_len   = strlen(directory->path) + entry_len + 2;
     char             buf[buf_len];
     void            *handle;

     snprintf( buf, buf_len, "%s/%s", directory->path, module->file );

     UED_DEBUG_ENTER( Modules, "Loading '%s'...", buf );

     handle = dlopen( buf, RTLD_NOW );
     if (!handle)
          UED_DEBUG( "Library/Modules: Unable to dlopen `%s %s'!", buf, dlerror() );

     UED_DEBUG_EXIT( Modules, "Done loading modules!");
     return handle;
}

#endif

