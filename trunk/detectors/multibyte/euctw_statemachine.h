#ifndef __EUCTW_STATEMACHINE_H__
#define __EUCTW_STATEMACHINE_H__

#include <types.h>
#include <encoding_state_machine.h>
#include <char_distribution.h>
#include "EUCTWFreq.tab"

static u32 EUCTW_cls [ 256 / 8 ] =
{
//PCK4BITS(0,2,2,2,2,2,2,2),  // 00 - 07 
PCK4BITS(2,2,2,2,2,2,2,2),  // 00 - 07 
PCK4BITS(2,2,2,2,2,2,0,0),  // 08 - 0f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 10 - 17 
PCK4BITS(2,2,2,0,2,2,2,2),  // 18 - 1f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 20 - 27 
PCK4BITS(2,2,2,2,2,2,2,2),  // 28 - 2f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 30 - 37 
PCK4BITS(2,2,2,2,2,2,2,2),  // 38 - 3f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 40 - 47 
PCK4BITS(2,2,2,2,2,2,2,2),  // 48 - 4f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 50 - 57 
PCK4BITS(2,2,2,2,2,2,2,2),  // 58 - 5f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 60 - 67 
PCK4BITS(2,2,2,2,2,2,2,2),  // 68 - 6f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 70 - 77 
PCK4BITS(2,2,2,2,2,2,2,2),  // 78 - 7f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 80 - 87 
PCK4BITS(0,0,0,0,0,0,6,0),  // 88 - 8f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 90 - 97 
PCK4BITS(0,0,0,0,0,0,0,0),  // 98 - 9f 
PCK4BITS(0,3,4,4,4,4,4,4),  // a0 - a7 
PCK4BITS(5,5,1,1,1,1,1,1),  // a8 - af 
PCK4BITS(1,1,1,1,1,1,1,1),  // b0 - b7 
PCK4BITS(1,1,1,1,1,1,1,1),  // b8 - bf 
PCK4BITS(1,1,3,1,3,3,3,3),  // c0 - c7 
PCK4BITS(3,3,3,3,3,3,3,3),  // c8 - cf 
PCK4BITS(3,3,3,3,3,3,3,3),  // d0 - d7 
PCK4BITS(3,3,3,3,3,3,3,3),  // d8 - df 
PCK4BITS(3,3,3,3,3,3,3,3),  // e0 - e7 
PCK4BITS(3,3,3,3,3,3,3,3),  // e8 - ef 
PCK4BITS(3,3,3,3,3,3,3,3),  // f0 - f7 
PCK4BITS(3,3,3,3,3,3,3,0)   // f8 - ff 
};


static u32 EUCTW_st [ 6] =
{
PCK4BITS(eError,eError,eStart,     3,     3,     3,     4,eError),//00-07 
PCK4BITS(eError,eError,eError,eError,eError,eError,eItsMe,eItsMe),//08-0f 
PCK4BITS(eItsMe,eItsMe,eItsMe,eItsMe,eItsMe,eError,eStart,eError),//10-17 
PCK4BITS(eStart,eStart,eStart,eError,eError,eError,eError,eError),//18-1f 
PCK4BITS(     5,eError,eError,eError,eStart,eError,eStart,eStart),//20-27 
PCK4BITS(eStart,eError,eStart,eStart,eStart,eStart,eStart,eStart) //28-2f 
};

static const u32 EUCTWCharLenTable[] = {0, 0, 1, 2, 2, 2, 3};

SMModel EUCTW_SMModel =
{
  {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, EUCTW_cls },
   7,
  {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, EUCTW_st },
  EUCTWCharLenTable,
  "x-euc-tw",
};

// for the Prober 
ProbingState euctw_decode(const char* stuff, u32 len );
void         euctw_reset(void);
int          euctw_score(void);

// for the CharDistributer
s32 euctw_get_order(const char* str);

UED_PROBER( EUCTW_PROBE, euctw_decode, euctw_reset, euctw_score );

UED_STATE_MACHINE(EUCTW_ESM, &EUCTW_SMModel, &EUCTW_PROBE);

UED_CHAR_DISTRIBUTER( EUCTW_CHARDIST, EUCTWCharToFreqOrder, EUCTW_TABLE_SIZE, \
	EUCTW_TYPICAL_DISTRIBUTION_RATIO,  euctw_get_order );

ProbingState euctw_decode(const char* stuff, u32 len )
{
        SMState state;
        u32 i=0, char_len=0;
	static char last_char[2]={ 0, 0 };
        UED_DEBUG( "euctw decoding started\n" );
        for (i = 0; i < len; i++){
                state = next_state( &EUCTW_ESM, stuff[i]);
                if (state == eError) {
        		UED_DEBUG( "euctw not found\n" );
                        return ( EUCTW_PROBE.probe_state = eNotMe );
                }
                if (state == eItsMe) {
        		UED_DEBUG( "find euctw\n" );
                        return ( EUCTW_PROBE.probe_state = eFoundIt );
                }
                if (state == eStart) {
			char_len = GetCurrentCharLen(&EUCTW_ESM);
			if( i == 0 ){
				last_char[1] = stuff[0];
				analysis_one_char(&EUCTW_CHARDIST, last_char, char_len);
                                EUCTW_PROBE.numer_char++;
			} else {
 				analysis_one_char(&EUCTW_CHARDIST, stuff+i-1, char_len);
			}
                }
        }
	last_char[0] = stuff[len-1];
	if (EUCTW_PROBE.probe_state == eDetecting) {
		UED_DEBUG( "may be euctw, read total char=%d, score=%d\n", 
			EUCTW_CHARDIST.mTotalChars,  EUCTW_PROBE.score() );
		if ( ( EUCTW_CHARDIST.mTotalChars > ENOUGH_DATA_THRESHOLD ) &&
			(EUCTW_PROBE.score() > 65) ){
			EUCTW_PROBE.probe_state = eFoundIt;
		}
	}
        return EUCTW_PROBE.probe_state;
}

void euctw_reset(void)
{
        EUCTW_PROBE.numer_char = 0;
        EUCTW_PROBE.probe_state= eDetecting;
}

int euctw_score(void)
{
	return EUCTW_CHARDIST.score(&EUCTW_CHARDIST);
}

//  for euctw encoding, we are interested 
//  first  byte range: 0xc4 -- 0xfe
//  second byte range: 0xa1 -- 0xfe
//  no validation needed here. State machine has done that
s32 euctw_get_order(const char* str) 
{
	if ((u8)*str >= (u8)0xc4)
		return 94*((u8)str[0]-(u8)0xc4) + (u8)str[1] - (u8)0xa1;
	else
		return -1;
}

#endif
