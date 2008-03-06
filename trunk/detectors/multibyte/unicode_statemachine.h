#ifndef __UNICODE_STATEMACHINE_H__
#define __UNICODE_STATEMACHINE_H__

#include "../../lib/encoding_state_machine.h"

static u32 UCS2BE_cls [ 256 / 8 ] = {
PCK4BITS(0,0,0,0,0,0,0,0),  // 00 - 07 
PCK4BITS(0,0,1,0,0,2,0,0),  // 08 - 0f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 10 - 17 
PCK4BITS(0,0,0,3,0,0,0,0),  // 18 - 1f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 20 - 27 
PCK4BITS(0,3,3,3,3,3,0,0),  // 28 - 2f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 30 - 37 
PCK4BITS(0,0,0,0,0,0,0,0),  // 38 - 3f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 40 - 47 
PCK4BITS(0,0,0,0,0,0,0,0),  // 48 - 4f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 50 - 57 
PCK4BITS(0,0,0,0,0,0,0,0),  // 58 - 5f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 60 - 67 
PCK4BITS(0,0,0,0,0,0,0,0),  // 68 - 6f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 70 - 77 
PCK4BITS(0,0,0,0,0,0,0,0),  // 78 - 7f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 80 - 87 
PCK4BITS(0,0,0,0,0,0,0,0),  // 88 - 8f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 90 - 97 
PCK4BITS(0,0,0,0,0,0,0,0),  // 98 - 9f 
PCK4BITS(0,0,0,0,0,0,0,0),  // a0 - a7 
PCK4BITS(0,0,0,0,0,0,0,0),  // a8 - af 
PCK4BITS(0,0,0,0,0,0,0,0),  // b0 - b7 
PCK4BITS(0,0,0,0,0,0,0,0),  // b8 - bf 
PCK4BITS(0,0,0,0,0,0,0,0),  // c0 - c7 
PCK4BITS(0,0,0,0,0,0,0,0),  // c8 - cf 
PCK4BITS(0,0,0,0,0,0,0,0),  // d0 - d7 
PCK4BITS(0,0,0,0,0,0,0,0),  // d8 - df 
PCK4BITS(0,0,0,0,0,0,0,0),  // e0 - e7 
PCK4BITS(0,0,0,0,0,0,0,0),  // e8 - ef 
PCK4BITS(0,0,0,0,0,0,0,0),  // f0 - f7 
PCK4BITS(0,0,0,0,0,0,4,5)   // f8 - ff 
};


static u32 UCS2BE_st [ 7] = {
PCK4BITS(     5,     7,     7,eError,     4,     3,eError,eError),//00-07 
PCK4BITS(eError,eError,eError,eError,eItsMe,eItsMe,eItsMe,eItsMe),//08-0f 
PCK4BITS(eItsMe,eItsMe,     6,     6,     6,     6,eError,eError),//10-17 
PCK4BITS(     6,     6,     6,     6,     6,eItsMe,     6,     6),//18-1f 
PCK4BITS(     6,     6,     6,     6,     5,     7,     7,eError),//20-27 
PCK4BITS(     5,     8,     6,     6,eError,     6,     6,     6),//28-2f 
PCK4BITS(     6,     6,     6,     6,eError,eError,eStart,eStart) //30-37 
};

static const u32 UCS2BECharLenTable[] = {2, 2, 2, 0, 2, 2};

static const SMModel UCS2BE_SMModel = {
  {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, UCS2BE_cls },
   6,
  {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, UCS2BE_st },
  UCS2BECharLenTable,
  "UTF-16BE",
};

UED_STATE_MACHINE(UCS2BE_ESM, &UCS2BE_SMModel, NULL );

static u32 UCS2LE_cls [ 256 / 8 ] = {
PCK4BITS(0,0,0,0,0,0,0,0),  // 00 - 07 
PCK4BITS(0,0,1,0,0,2,0,0),  // 08 - 0f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 10 - 17 
PCK4BITS(0,0,0,3,0,0,0,0),  // 18 - 1f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 20 - 27 
PCK4BITS(0,3,3,3,3,3,0,0),  // 28 - 2f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 30 - 37 
PCK4BITS(0,0,0,0,0,0,0,0),  // 38 - 3f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 40 - 47 
PCK4BITS(0,0,0,0,0,0,0,0),  // 48 - 4f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 50 - 57 
PCK4BITS(0,0,0,0,0,0,0,0),  // 58 - 5f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 60 - 67 
PCK4BITS(0,0,0,0,0,0,0,0),  // 68 - 6f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 70 - 77 
PCK4BITS(0,0,0,0,0,0,0,0),  // 78 - 7f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 80 - 87 
PCK4BITS(0,0,0,0,0,0,0,0),  // 88 - 8f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 90 - 97 
PCK4BITS(0,0,0,0,0,0,0,0),  // 98 - 9f 
PCK4BITS(0,0,0,0,0,0,0,0),  // a0 - a7 
PCK4BITS(0,0,0,0,0,0,0,0),  // a8 - af 
PCK4BITS(0,0,0,0,0,0,0,0),  // b0 - b7 
PCK4BITS(0,0,0,0,0,0,0,0),  // b8 - bf 
PCK4BITS(0,0,0,0,0,0,0,0),  // c0 - c7 
PCK4BITS(0,0,0,0,0,0,0,0),  // c8 - cf 
PCK4BITS(0,0,0,0,0,0,0,0),  // d0 - d7 
PCK4BITS(0,0,0,0,0,0,0,0),  // d8 - df 
PCK4BITS(0,0,0,0,0,0,0,0),  // e0 - e7 
PCK4BITS(0,0,0,0,0,0,0,0),  // e8 - ef 
PCK4BITS(0,0,0,0,0,0,0,0),  // f0 - f7 
PCK4BITS(0,0,0,0,0,0,4,5)   // f8 - ff 
};

static u32 UCS2LE_st [ 7] = {
PCK4BITS(     6,     6,     7,     6,     4,     3,eError,eError),//00-07 
PCK4BITS(eError,eError,eError,eError,eItsMe,eItsMe,eItsMe,eItsMe),//08-0f 
PCK4BITS(eItsMe,eItsMe,     5,     5,     5,eError,eItsMe,eError),//10-17 
PCK4BITS(     5,     5,     5,eError,     5,eError,     6,     6),//18-1f 
PCK4BITS(     7,     6,     8,     8,     5,     5,     5,eError),//20-27 
PCK4BITS(     5,     5,     5,eError,eError,eError,     5,     5),//28-2f 
PCK4BITS(     5,     5,     5,eError,     5,eError,eStart,eStart) //30-37 
};

static const u32 UCS2LECharLenTable[] = {2, 2, 2, 2, 2, 2};

static const SMModel UCS2LE_SMModel = {
  {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, UCS2LE_cls },
   6,
  {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, UCS2LE_st },
  UCS2LECharLenTable,
  "UTF-16LE",
};

UED_STATE_MACHINE(UCS2LE_ESM, &UCS2LE_SMModel, NULL );

static u32 UTF8_cls [ 256 / 8 ] = {
//PCK4BITS(0,1,1,1,1,1,1,1),  // 00 - 07 
PCK4BITS(1,1,1,1,1,1,1,1),  // 00 - 07  //allow 0x00 as a legal value
PCK4BITS(1,1,1,1,1,1,0,0),  // 08 - 0f 
PCK4BITS(1,1,1,1,1,1,1,1),  // 10 - 17 
PCK4BITS(1,1,1,0,1,1,1,1),  // 18 - 1f 
PCK4BITS(1,1,1,1,1,1,1,1),  // 20 - 27 
PCK4BITS(1,1,1,1,1,1,1,1),  // 28 - 2f 
PCK4BITS(1,1,1,1,1,1,1,1),  // 30 - 37 
PCK4BITS(1,1,1,1,1,1,1,1),  // 38 - 3f 
PCK4BITS(1,1,1,1,1,1,1,1),  // 40 - 47 
PCK4BITS(1,1,1,1,1,1,1,1),  // 48 - 4f 
PCK4BITS(1,1,1,1,1,1,1,1),  // 50 - 57 
PCK4BITS(1,1,1,1,1,1,1,1),  // 58 - 5f 
PCK4BITS(1,1,1,1,1,1,1,1),  // 60 - 67 
PCK4BITS(1,1,1,1,1,1,1,1),  // 68 - 6f 
PCK4BITS(1,1,1,1,1,1,1,1),  // 70 - 77 
PCK4BITS(1,1,1,1,1,1,1,1),  // 78 - 7f 
PCK4BITS(2,2,2,2,3,3,3,3),  // 80 - 87 
PCK4BITS(4,4,4,4,4,4,4,4),  // 88 - 8f 
PCK4BITS(4,4,4,4,4,4,4,4),  // 90 - 97 
PCK4BITS(4,4,4,4,4,4,4,4),  // 98 - 9f 
PCK4BITS(5,5,5,5,5,5,5,5),  // a0 - a7 
PCK4BITS(5,5,5,5,5,5,5,5),  // a8 - af 
PCK4BITS(5,5,5,5,5,5,5,5),  // b0 - b7 
PCK4BITS(5,5,5,5,5,5,5,5),  // b8 - bf 
PCK4BITS(0,0,6,6,6,6,6,6),  // c0 - c7 
PCK4BITS(6,6,6,6,6,6,6,6),  // c8 - cf 
PCK4BITS(6,6,6,6,6,6,6,6),  // d0 - d7 
PCK4BITS(6,6,6,6,6,6,6,6),  // d8 - df 
PCK4BITS(7,8,8,8,8,8,8,8),  // e0 - e7 
PCK4BITS(8,8,8,8,8,9,8,8),  // e8 - ef 
PCK4BITS(10,11,11,11,11,11,11,11),  // f0 - f7 
PCK4BITS(12,13,13,13,14,15,0,0)   // f8 - ff 
};


static u32 UTF8_st [ 26] = {
PCK4BITS(eError,eStart,eError,eError,eError,eError,     12,     10),//00-07 
PCK4BITS(     9,     11,     8,     7,     6,     5,     4,     3),//08-0f 
PCK4BITS(eError,eError,eError,eError,eError,eError,eError,eError),//10-17 
PCK4BITS(eError,eError,eError,eError,eError,eError,eError,eError),//18-1f 
PCK4BITS(eItsMe,eItsMe,eItsMe,eItsMe,eItsMe,eItsMe,eItsMe,eItsMe),//20-27 
PCK4BITS(eItsMe,eItsMe,eItsMe,eItsMe,eItsMe,eItsMe,eItsMe,eItsMe),//28-2f 
PCK4BITS(eError,eError,     5,     5,     5,     5,eError,eError),//30-37 
PCK4BITS(eError,eError,eError,eError,eError,eError,eError,eError),//38-3f 
PCK4BITS(eError,eError,eError,     5,     5,     5,eError,eError),//40-47 
PCK4BITS(eError,eError,eError,eError,eError,eError,eError,eError),//48-4f 
PCK4BITS(eError,eError,     7,     7,     7,     7,eError,eError),//50-57 
PCK4BITS(eError,eError,eError,eError,eError,eError,eError,eError),//58-5f 
PCK4BITS(eError,eError,eError,eError,     7,     7,eError,eError),//60-67 
PCK4BITS(eError,eError,eError,eError,eError,eError,eError,eError),//68-6f 
PCK4BITS(eError,eError,     9,     9,     9,     9,eError,eError),//70-77 
PCK4BITS(eError,eError,eError,eError,eError,eError,eError,eError),//78-7f 
PCK4BITS(eError,eError,eError,eError,eError,     9,eError,eError),//80-87 
PCK4BITS(eError,eError,eError,eError,eError,eError,eError,eError),//88-8f 
PCK4BITS(eError,eError,     12,     12,     12,     12,eError,eError),//90-97 
PCK4BITS(eError,eError,eError,eError,eError,eError,eError,eError),//98-9f 
PCK4BITS(eError,eError,eError,eError,eError,     12,eError,eError),//a0-a7 
PCK4BITS(eError,eError,eError,eError,eError,eError,eError,eError),//a8-af 
PCK4BITS(eError,eError,     12,     12,     12,eError,eError,eError),//b0-b7 
PCK4BITS(eError,eError,eError,eError,eError,eError,eError,eError),//b8-bf 
PCK4BITS(eError,eError,eStart,eStart,eStart,eStart,eError,eError),//c0-c7 
PCK4BITS(eError,eError,eError,eError,eError,eError,eError,eError) //c8-cf 
};

static const u32 UTF8CharLenTable[] = {0, 1, 0, 0, 0, 0, 2, 3, 
                            3, 3, 4, 4, 5, 5, 6, 6 };

static const SMModel UTF8_SMModel = {
  {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, UTF8_cls },
   16,
  {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, UTF8_st },
  UTF8CharLenTable,
  "UTF-8",
};

ProbingState utf8_decode(const char* stuff, u32 len );
void         utf8_reset(void);
int          utf8_score(void);

UED_PROBER( UTF8_PROBE, utf8_decode, utf8_reset, utf8_score ); 

UED_STATE_MACHINE(UTF8_ESM, &UTF8_SMModel, &UTF8_PROBE);

ProbingState utf8_decode(const char* stuff, u32 len ){
	SMState state;
	u32 i=0; 
	UED_DEBUG( "UTF8 decoding started\n" );
	for (i = 0; i < len; i++){
		state = next_state( &UTF8_ESM, stuff[i]);
		if (state == eError) {
			return ( UTF8_PROBE.probe_state = eNotMe );
		}
		if (state == eItsMe) {
			return ( UTF8_PROBE.probe_state = eFoundIt );
		} 
		if (state == eStart) {
			if (GetCurrentCharLen(&UTF8_ESM) >= 2)
				UTF8_PROBE.numer_char++;
		}
	}
	if (UTF8_PROBE.probe_state == eDetecting) {
		UED_DEBUG( "UTF8 score = %d\n", UTF8_PROBE.score() );
		if (UTF8_PROBE.score() > 65)
			UTF8_PROBE.probe_state = eFoundIt;
	}
	UED_DEBUG( "UTF8 decoding done\n" );
	return UTF8_PROBE.probe_state;
}

void utf8_reset(void){
	UTF8_PROBE.numer_char = 0;
	UTF8_PROBE.probe_state= eDetecting;
}

int utf8_score(void)
{

	int unlike = 99, i=0;
	if (UTF8_PROBE.numer_char < 6) {
		for (i = 0; i < UTF8_PROBE.numer_char ; i++)
			unlike *= 0.50;
		return 100 - unlike;
	} else
		return 99;
}

#endif
