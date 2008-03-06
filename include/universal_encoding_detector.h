#ifndef __UNIVERSAL_ENCODING_DETECTOR_H__
#define __UNIVERSAL_ENCODING_DETECTOR_H__

/*
   (c) Copyright 2007  AvengerGear.com

   All rights reserved.

   This utility reference include/directfb.h file in directfb source
   Please reference http://www.directfb.org

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

/*
 * Every interface method returns this result code.<br>
 * Any other value to be returned adds an argument pointing
 * to a location the value should be written to.
 */

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
    UED_OK,             /* No error occured. */
    UED_INVARG,         /* A wrong arg from configure file. */
    UED_UNSUPPORTED,    /* A unsupported arg in configuration. */
    UED_FAILURE         /* A general or unknown error occured. */
} UEDResult;

/*
 * Parses the command-line and initializes some variables. 
 */

UEDResult
UEDInit( int *argc, char *(*argv[]) );


/*
 * Main interface of UniversalEncodingDetector, created by UEDCreate().
 */
// DECLARE_INTERFACE( IUED )


#ifdef __cplusplus
}
#endif

#endif // __UNIVERSAL_ENCODING_DETECTOR_H__
