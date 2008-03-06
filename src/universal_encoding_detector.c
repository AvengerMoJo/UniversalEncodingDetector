/*
   (c) Copyright 2007  AvengerGear.com

   All rights reserved.

   Created by Alex Lau <avengermojo@gmail.com> 

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

#include <config.h>

#include <stdlib.h>
/*
#include <stdio.h>
#include <unistd.h>
*/

#include <universal_encoding_detector.h>
#include <universal_encoding_detector_version.h>

#include <configurator.h>

/*
 * Version checking
 */
const u32 ued_major_version = UED_MAJOR_VERSION;
const u32 ued_minor_version = UED_MINOR_VERSION;
const u32 ued_micro_version = UED_MICRO_VERSION;


const char *
UniversalEncodingDetectorCheckVersion( u32 required_major,
                                       u32 required_minor )
{
     if (required_major > UED_MAJOR_VERSION)
          return "Universal Encoding Detector version too old (major mismatch)";
     if (required_major < UED_MAJOR_VERSION)
          return "Universal Encoding Detector version too new (major mismatch)";
     if (required_minor > UED_MINOR_VERSION)
          return "Universal Encoding Detector version too old (minor mismatch)";
     if (required_minor < UED_MINOR_VERSION)
          return "Universal Encoding Detector version too new (minor mismatch)";

     return NULL;
}

const char *
UniversalEncodingDetectorUsageString( void )
{
     return ued_config_usage();
}

UEDResult
UEDInit( int *argc, char *(*argv[]) )
{
     return ued_config_init( argc, argv );
}


