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
#ifndef __UED__ENCODING_STATE_MACHINE_H__
#define __UED__ENCODING_STATE_MACHINE_H__

#include <types.h>
#include <linkedlist.h>
#include <debug.h>
#include <packint.h>

UED_DEBUG_SECTION( ESM , "Library/ESM", "Encoding State Machine Library" );

enum __UED_SMState{
	eStart = 0,
	eError = 1,
	eItsMe = 2 
};

enum __UED_ProbingState{
	eDetecting = 0,   //We are still detecting, no sure answer yet, but caller can ask for confidence.
	eFoundIt = 1,     //That's a positive answer
	eNotMe = 2        //Negative answer
};

#define GETCLASS(c,s) GETFROMPCK(((u8)(c)), (s)->mModel->classTable)

//state machine model
struct __UED_SMModel{
	PackInt classTable;
	u32 classFactor;
	PackInt stateTable;
	const u32* charLenTable;
	const char* name;
};

//state machine structure 
struct __UED_EncodingStateMachine{
	SMState              mCurrentState;
	u32                  mCurrentCharLen;
	u32                  mCurrentBytePos;
	const SMModel        *mModel;
	CharSetProber        *prober;
};

#define UED_STATE_MACHINE(sm_identifier, sm_model, probe)                   \
     static EncodingStateMachine sm_identifier = { eStart, 0, 0, sm_model, probe }

struct __UED_CharSetProber {
	ProbingState probe_state;
	u32           numer_char;
	ProbingState (*decoding)(const char*, u32);
 	void         (*reset)(void);
 	int          (*score)(void);
};

#define UED_PROBER(prober_identifier, d, r ,s )                         \
     static CharSetProber prober_identifier= { eDetecting, 0, (d), (r), (s) }

static inline SMState
next_state( EncodingStateMachine* sm, char c )
{
        UED_DEBUG_ENTER( ESM,  " EncodingStateMachine detection started" );
	if( sm != NULL ){
            UED_DEBUG( "ESM detection char(%d) >%c< state=%d, char len=%d, BytePos=%d, SMModel-Name=%s\n", c, c ,\
		sm->mCurrentState, sm->mCurrentCharLen, sm->mCurrentBytePos, sm->mModel->name );
	}

	//for each byte we get its class , if it is first byte, we also get byte length
	u32 byteCls = GETCLASS(c, sm);
	UED_DEBUG( "ESM byte length =%d\n" , byteCls ); 
	if (sm->mCurrentState == eStart) {
		sm->mCurrentBytePos = 0;
		sm->mCurrentCharLen = sm->mModel->charLenTable[byteCls];
	}
	//from byte's class and stateTable, we get its next state
        UED_DEBUG( "ESM detection GETFROMPCK(state=%d*classFactor=%d+bytecls=%d)\n", \
		sm->mCurrentState, sm->mModel->classFactor, byteCls );
	(sm->mCurrentState)= GETFROMPCK( ((sm->mCurrentState)*(sm->mModel->classFactor))+byteCls, (sm->mModel->stateTable) );
        UED_DEBUG( "ESM detection GETFROMPCK()=%d\n",sm->mCurrentState ); 
	sm->mCurrentBytePos++;
        UED_DEBUG_EXIT( ESM,  " ESM detection done" );
	return sm->mCurrentState;
}

static u32
GetCurrentCharLen( EncodingStateMachine* sm )
{
	return sm->mCurrentCharLen;
}

/*
static void
Reset( EncodingStateMachine* sm )
{
	sm->mCurrentState = eStart;
}

static const char* 
GetCodingStateMachine( EncodingStateMachine* sm )
{
	return sm->mModel->name;
}
*/

#endif /* __UED__ENCODING_STATE_MACHINE_H__ */
