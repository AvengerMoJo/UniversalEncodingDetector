#ifndef __UED__TYPES_H__
#define __UED__TYPES_H__

/*
   (c) Copyright 2007  AvengerGear.com

   All rights reserved.

   This utility reference lib/direct/types.h file in directfb source
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


#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#if BUILD_UED_STDBOOL
#include <stdbool.h>
#else // BUILD_UED_STDBOOL

#if !defined(__cplusplus) && !defined(__bool_true_false_are_defined)
#ifndef false
#define false (0)
#endif

#ifndef true
#define true (!false)
#endif

typedef u8 bool;
#endif // !cpp && bool_defined 
#endif // BUILD_UED_STDBOOL 

typedef struct __UED_LinkedList    LinkedList;
typedef struct __UED_ModuleDir     ModuleDir;
typedef struct __UED_ModuleEntry   ModuleEntry;

typedef struct __UED_PackInt       PackInt; 
typedef enum __UED_IndexSoft       IdxSft; 
typedef enum __UED_SoftMask        SftMsk; 
typedef enum __UED_BitShift        BitSft; 
typedef enum __UED_UnitMask        UnitMsk; 

typedef struct __UED_EncodingStateMachine EncodingStateMachine;
typedef struct __UED_CharSetProber        CharSetProber;
typedef struct __UED_SMModel              SMModel;
typedef enum __UED_SMState                SMState;
typedef enum __UED_ProbingState           ProbingState;

typedef struct __UED_CharDistributer      CharDistributer;


#endif // __UED__TYPES_H__
