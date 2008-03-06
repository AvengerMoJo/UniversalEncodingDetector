/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
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
 * The Original Code is Mozilla Communicator client code.
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

#ifndef __UED__CHAR_DISTRIBUTION_H__
#define __UED__CHAR_DISTRIBUTION_H__

#include "types.h"
#include "debug.h"

#define ENOUGH_DATA_THRESHOLD 1

UED_DEBUG_SECTION( CHARDIST , "Library/CharDistribution", "Char Distribution Library" );

struct __UED_CharDistributer{
	u32       mFreqChars;        //The number of characters whose frequency order is less than 512
	u32       mTotalChars;       //Total character encounted.
	const s16 *mCharToFreqOrder; //Mapping table to get frequency order from char order (get from GetOrder())
	u32       mTableSize;        //Size of above table
	float     mTypicalDistributionRatio;
	s32       (*get_order)(const char*);
        int       (*score)(CharDistributer*);
};

int cd_score(CharDistributer* cd)
{
	//if we didn't receive any character in our consideration range, or the
	// number of frequent characters is below the minimum threshold, return
	// negative answer
	UED_DEBUG( "char_distributer total char=%d, freq=%d, score=%d, ratio=%f -> %f\n",
		cd->mFreqChars, cd->mTotalChars, cd->mFreqChars, cd->mTypicalDistributionRatio, 
		cd->mFreqChars / ((cd->mTotalChars - cd->mFreqChars) * cd->mTypicalDistributionRatio) );

	if (cd->mTotalChars <= 0 || cd->mFreqChars <= 4)
		return 1; 
 	if (cd->mTotalChars != cd->mFreqChars) {
		int r = cd->mFreqChars / ((cd->mTotalChars - cd->mFreqChars) * cd->mTypicalDistributionRatio);
		if (r < 99)
			return r;
	}
	//normalize confidence, (we don't want to be 100% sure)
	return 99;
}

#define UED_CHAR_DISTRIBUTER(cd_identifier, cfo, ts, td, go)  \
     static CharDistributer cd_identifier = { 0, 0, (cfo), (ts), (td), (go), cd_score }

//feed a block of data and do distribution analysis
void 
analysis(CharDistributer* cd, const char* stuff, u32 len) {}
  
//Feed a character with known length
void 
analysis_one_char(CharDistributer* cd, const char* stuff, u32 len) {
	s32 order;

	//we only care about 2-bytes character in our distribution analysis
	order = (len == 2) ? cd->get_order(stuff) : -1;

	if (order >= 0) {
		cd->mTotalChars++;
		//order is valid
		if ((u32)order < cd->mTableSize) {
			if (512 > cd->mCharToFreqOrder[order])
				cd->mFreqChars++;
		}
	}
}

//Reset analyser, clear any state 
void
Reset(CharDistributer* cd) 
{
	cd->mTotalChars = 0;
	cd->mFreqChars = 0;
}

//It is not necessary to receive all data to draw conclusion. For charset detection,
// certain amount of data is enough
/*
bool 
got_enough_data(CharDistributer* cd)
{
	return cd->mTotalChars > ENOUGH_DATA_THRESHOLD;
}
*/

#endif /* __UED__CHAR_DISTRIBUTION_H__ */
