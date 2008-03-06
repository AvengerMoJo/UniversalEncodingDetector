/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef __UED__PACKINT_H__
#define __UED__PACKINT_H__

#include "types.h"

enum __UED_IndexSoft {
	eIdxSft4bits  = 3,
	eIdxSft8bits  = 2,
	eIdxSft16bits = 1
};

enum __UED_SoftMask {
	eSftMsk4bits  = 7,
	eSftMsk8bits  = 3,
	eSftMsk16bits = 1
}; 

enum __UED_BitShift {
	eBitSft4bits  = 2,
	eBitSft8bits  = 3,
	eBitSft16bits = 4
}; 

enum __UED_UnitMask {
	eUnitMsk4bits  = 0x0000000FL,
	eUnitMsk8bits  = 0x000000FFL,
	eUnitMsk16bits = 0x0000FFFFL
}; 

struct __UED_PackInt {
	IdxSft  idxsft;
	SftMsk  sftmsk;
	BitSft  bitsft;
	UnitMsk unitmsk;
	u32     *data;
};

// pack 8 different 4-bits number to a 32bits u32 ( 0x87654321 )

#define PCK16BITS(a,b)            ((u32)(((b) << 16) | (a)))

#define PCK8BITS(a,b,c,d)         PCK16BITS( ((u32)(((b) << 8) | (a))),  \
                                             ((u32)(((d) << 8) | (c))))

#define PCK4BITS(a,b,c,d,e,f,g,h) PCK8BITS(  ((u32)(((b) << 4) | (a))), \
                                             ((u32)(((d) << 4) | (c))), \
                                             ((u32)(((f) << 4) | (e))), \
                                             ((u32)(((h) << 4) | (g))) )

#define GETFROMPCK(i, c) \
 (((((c).data)[(i)>>(c).idxsft])>>(((i)&(c).sftmsk)<<(c).bitsft))&(c).unitmsk)

#endif /* __UED__PACKINT_H__ */
