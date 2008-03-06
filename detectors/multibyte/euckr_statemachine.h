#ifndef __EUCKR_STATEMACHINE_H__
#define __EUCKR_STATEMACHINE_H__

#include <types.h>
#include <encoding_state_machine.h>
#include <char_distribution.h>
#include "EUCKRFreq.tab"

static u32 EUCKR_cls [ 256 / 8 ] = 
{
//PCK4BITS(0,1,1,1,1,1,1,1),  // 00 - 07 
PCK4BITS(1,1,1,1,1,1,1,1),  // 00 - 07 
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
PCK4BITS(0,0,0,0,0,0,0,0),  // 80 - 87 
PCK4BITS(0,0,0,0,0,0,0,0),  // 88 - 8f 
PCK4BITS(0,0,0,0,0,0,0,0),  // 90 - 97 
PCK4BITS(0,0,0,0,0,0,0,0),  // 98 - 9f 
PCK4BITS(0,2,2,2,2,2,2,2),  // a0 - a7 
PCK4BITS(2,2,2,2,2,3,3,3),  // a8 - af 
PCK4BITS(2,2,2,2,2,2,2,2),  // b0 - b7 
PCK4BITS(2,2,2,2,2,2,2,2),  // b8 - bf 
PCK4BITS(2,2,2,2,2,2,2,2),  // c0 - c7 
PCK4BITS(2,3,2,2,2,2,2,2),  // c8 - cf 
PCK4BITS(2,2,2,2,2,2,2,2),  // d0 - d7 
PCK4BITS(2,2,2,2,2,2,2,2),  // d8 - df 
PCK4BITS(2,2,2,2,2,2,2,2),  // e0 - e7 
PCK4BITS(2,2,2,2,2,2,2,2),  // e8 - ef 
PCK4BITS(2,2,2,2,2,2,2,2),  // f0 - f7 
PCK4BITS(2,2,2,2,2,2,2,0)   // f8 - ff 
};


static u32 EUCKR_st [ 2] = 
{
PCK4BITS(eError,eStart,     3,eError,eError,eError,eError,eError),//00-07 
PCK4BITS(eItsMe,eItsMe,eItsMe,eItsMe,eError,eError,eStart,eStart) //08-0f 
};

static const u32 EUCKRCharLenTable[] = {0, 1, 2, 0};

SMModel EUCKR_SMModel = 
{
  {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, EUCKR_cls },
  4,
  {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, EUCKR_st },
  EUCKRCharLenTable,
  "EUC-KR",
};

// for the Prober 
ProbingState euckr_decode(const char* stuff, u32 len );
void         euckr_reset(void);
int          euckr_score(void);

// for the CharDistributer
s32 euckr_get_order(const char* str);

UED_PROBER( EUCKR_PROBE, euckr_decode, euckr_reset, euckr_score );

UED_STATE_MACHINE(EUCKR_ESM, &EUCKR_SMModel, &EUCKR_PROBE);

UED_CHAR_DISTRIBUTER( EUCKR_CHARDIST, EUCKRCharToFreqOrder, EUCKR_TABLE_SIZE, \
	EUCKR_TYPICAL_DISTRIBUTION_RATIO,  euckr_get_order );

ProbingState euckr_decode(const char* stuff, u32 len )
{
        SMState state;
        u32 i=0, char_len=0;
	static char last_char[2]={ 0, 0 };
        UED_DEBUG( "euckr decoding started\n" );
        for (i = 0; i < len; i++){
                state = next_state( &EUCKR_ESM, stuff[i]);
                if (state == eError) {
        		UED_DEBUG( "euc-kr not found\n" );
                        return ( EUCKR_PROBE.probe_state = eNotMe );
                }
                if (state == eItsMe) {
        		UED_DEBUG( "find euc-kr\n" );
                        return ( EUCKR_PROBE.probe_state = eFoundIt );
                }
                if (state == eStart) {
			char_len = GetCurrentCharLen(&EUCKR_ESM);
			if( i == 0 ){
				last_char[1] = stuff[0];
				analysis_one_char(&EUCKR_CHARDIST, last_char, char_len);
                                EUCKR_PROBE.numer_char++;
			} else {
 				analysis_one_char(&EUCKR_CHARDIST, stuff+i-1, char_len);
			}
                }
        }
	last_char[0] = stuff[len-1];
	if (EUCKR_PROBE.probe_state == eDetecting) {
		UED_DEBUG( "may be euc-kr, read total char=%d, freq=%d, score=%d\n", 
			EUCKR_CHARDIST.mTotalChars, EUCKR_CHARDIST.mFreqChars, EUCKR_PROBE.score() );
		if ( ( EUCKR_CHARDIST.mTotalChars > ENOUGH_DATA_THRESHOLD ) &&
			(EUCKR_PROBE.score() > 65) ){
			EUCKR_PROBE.probe_state = eFoundIt;
		}
	}
        return EUCKR_PROBE.probe_state;
}

void euckr_reset(void)
{
        EUCKR_PROBE.numer_char = 0;
        EUCKR_PROBE.probe_state= eDetecting;
}

int euckr_score(void)
{
	return EUCKR_CHARDIST.score(&EUCKR_CHARDIST);
}

//  for euc-KR encoding, we are interested 
//  first  byte range: 0xb0 -- 0xfe
//  second byte range: 0xa1 -- 0xfe
//  no validation needed here. State machine has done that
s32 euckr_get_order(const char* str) 
{
	if ((u8)*str >= (u8)0xb0)
		return 94*((u8)str[0]-(u8)0xb0) + (u8)str[1] - (u8)0xa1;
	else
		return -1;
}

#endif /* __EUCKR_STATEMACHINE_H__ */
