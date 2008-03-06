#ifndef __CONFIGURATOR_H__
#define __CONFIGURATOR_H__

/*
   (c) Copyright 2007  AvengerGear.com

   All rights reserved.

   This utility reference src/misc/conf.h file in directfb source
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
#include <stdlib.h>
#include <string.h>

#include <config.h>
#include <types.h>

#include <debug.h>
#include <universal_encoding_detector.h>

typedef struct 
{
    bool    enable;          /* is already enable */
    char  **disable_module;  /* modules that will not be loaded */
} UEDConfigurator;

extern UEDConfigurator *ued_config;

// Debugging declairation for configurator 
UED_DEBUG_SECTION( CONFIGURATOR , "Main/Configurator", "The configurator program" );

/*
 * Allocate Configuration struct, fill with defaults and parse command line 
 * options for overrides.
 */
UEDResult ued_config_init( int *argc, char *(*argv[]) );

/*
 * Read configuration options from file. Called by ued_config_init().
 */
UEDResult ued_config_read( const char *filename );

/*
 * Set indiviual option. Used by ued_config_init(), ued_config_read()
 */
UEDResult ued_config_set( const char *name, const char *value );

/*
 * Provide how to use the configuration file and description
 */
const char *ued_config_usage( void );

#endif // __CONFIGURATOR_H__
