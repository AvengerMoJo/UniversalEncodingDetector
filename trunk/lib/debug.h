#ifndef __UED__DEBUG_H__
#define __UED__DEBUG_H__

/*
   (c) Copyright 2007  AvengerGear.com

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

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>          // for strchr & strlen
#include <signal.h>          // for SIGTRAP


#include <build.h>
#include <types.h>

#if __GNUC__ >= 3
#define UED_FORMAT_PRINTF(n)	__attribute__((__format__ (__printf__, n, n+1)))
#else
#define UED_FORMAT_PRINTF(n)
#endif //__GNUC__

typedef struct __UED_DebugSection {
     u32            age;
     bool           enabled;
     bool           registered;

     const char    *name;
     const char    *description;
} UEDDebugSection;


#if BUILD_UED_DEBUG

void ued_debug_set_section( const char *name, bool enable );

#define UED_DEBUG_SECTION(identifier,name,description)                         \
     static UEDDebugSection identifier = { 0, false, false, name, description }

void ued_debug( const char *format, ... )  UED_FORMAT_PRINTF(1);

void ued_debug_enter( UEDDebugSection *section,
			const char    *function,
			const char    *filename,
			int	       line,
			const char    *format, ... )  UED_FORMAT_PRINTF(5);

void ued_debug_exit( UEDDebugSection *section,
			const char   *function,
			const char   *filename,
                        int	      line,
			const char   *format, ... )  UED_FORMAT_PRINTF(5);

void ued_assertion( const char *exp,
                    const char *func,
                    const char *file,
                    int         line );

void signal_handler( int num, siginfo_t *info, void *foo );

// #if 1
#if SIGTRAP_TERMINATE
#define SIGTRAP_RETURN
#define SIGTRAP_HANDLE
#else 
#define SIGTRAP_RETURN return
#define SIGTRAP_HANDLE                                                         \
    struct sigaction action;                                                   \
    action.sa_sigaction = signal_handler;                                      \
    action.sa_flags     = SA_SIGINFO;                                          \
    sigemptyset( &action.sa_mask );                                            \
    sigaction( SIGTRAP, &action, NULL )
#endif // SIGTRAP_TERMINATE

#define UED_DEBUG(x...)                                                        \
   do {                                                                        \
       ued_debug( x );                                                         \
   } while (0)

#define UED_DEBUG_ENTER(d,x...)                                                \
   do {                                                                        \
       ued_debug_enter( &d, __FUNCTION__, __FILE__, __LINE__, x );             \
   } while (0)

#define UED_DEBUG_EXIT(d,x...)                                                 \
   do {                                                                        \
       ued_debug_exit( &d, __FUNCTION__, __FILE__, __LINE__, x );              \
   } while (0)

#define UED_ASSERT(exp)                                                        \
   do {                                                                        \
       if (!(exp)) {                                                           \
           SIGTRAP_HANDLE;                                                     \
	   ued_assertion( #exp, __FUNCTION__, __FILE__, __LINE__ );            \
           SIGTRAP_RETURN;                                                     \
       }                                                                       \
   } while (0)

#else // BUILD_UED_DEBUG

#define UED_DEBUG_SECTION(i,n,d)
#define UED_DEBUG(x...)              do {} while (0)
#define UED_DEBUG_ENTER(d,x...)      do {} while (0)
#define UED_DEBUG_EXIT(d,x...)       do {} while (0)
#define UED_ASSERT(exp)              do {} while (0)

#endif // BUILD_UED_DEBUG

#endif // __UED__DEBUG_H__
