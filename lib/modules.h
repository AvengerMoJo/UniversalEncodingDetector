#ifndef __UED__MODULES_H__
#define __UED__MODULES_H__
/*
   (c) Copyright 2007  AvengerGear.com

   All rights reserved.

   This utility reference lib/direct/modules.h file in directfb source
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

#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

#include <types.h>
#include <linkedlist.h>
#include <debug.h>
#include <../src/configurator.h>

struct __UED_ModuleEntry {
     LinkedList         link;

     // int                magic;

     ModuleDir          *directory;

     bool               loaded;
     bool               dynamic;
     bool               disabled;

     char              *name;
     const void        *funcs;

     int                refs;
     char              *file;
     void              *handle;
};

struct __UED_ModuleDir {
     pthread_mutex_t    lock;

     const char        *path;
     u32               abi_version;

     LinkedList        *entries;

     ModuleEntry       *loading;
};

#define DECLARE_MODULE_DIRECTORY(d)  \
     extern ModuleDir d

#define DEFINE_MODULE_DIRECTORY(d,p,n)                 \
     ModuleDir d = {                                   \
          lock:        PTHREAD_MUTEX_INITIALIZER,      \
          path:        MODULEDIR"/"p,                  \
          abi_version: n,                              \
          entries:     NULL,                           \
          loading:     NULL                            \
     }

int   modules_explore_directory( ModuleDir *directory );

void  modules_register( ModuleDir       *directory,
                        u32             abi_version,
                        const char      *name,
                        const void      *funcs );

const void *module_ref  ( ModuleEntry *module );
void        module_unref( ModuleEntry *module );

#endif
