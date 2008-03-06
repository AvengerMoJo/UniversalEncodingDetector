#ifndef __BIG5_STATEMACHINE_H__
#define __BIG5_STATEMACHINE_H__

#include <types.h>
#include <encoding_state_machine.h>
#include <char_distribution.h>
#include "BIG5Freq.tab"

/*
Modification from frank tang's original work:
. 0x00 is allowed as a legal character. Since some web pages contains this char in 
  text stream.
*/

// BIG5 
static u32 BIG5_cls [ 256 / 8 ] = 
{
//PCK4BITS(0,1,1,1,1,1,1,1),  // 00 - 07 
PCK4BITS(1,1,1,1,1,1,1,1),  // 00 - 07    //allow 0x00 as legal value
PCK4BITS(1,1,1,1,1,1,0,0),  // 08 - 0f 
PCK4BITS(1,1,1,1,1,1,1,1),  // 10 - 17 
PCK4BITS(1,1,1,0,1,1,1,1),  // 18 - 1f 
PCK4BITS(1,1,1,1,1,1,1,1),  // 20 - 27 
PCK4BITS(1,1,1,1,1,1,1,1),  // 28 - 2f 
PCK4BITS(1,1,1,1,1,1,1,1),  // 30 - 37 
PCK4BITS(1,1,1,1,1,1,1,1),  // 38 - 3f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 40 - 47 
PCK4BITS(2,2,2,2,2,2,2,2),  // 48 - 4f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 50 - 57 
PCK4BITS(2,2,2,2,2,2,2,2),  // 58 - 5f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 60 - 67 
PCK4BITS(2,2,2,2,2,2,2,2),  // 68 - 6f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 70 - 77 
PCK4BITS(2,2,2,2,2,2,2,1),  // 78 - 7f 
PCK4BITS(4,4,4,4,4,4,4,4),  // 80 - 87 
PCK4BITS(4,4,4,4,4,4,4,4),  // 88 - 8f 
PCK4BITS(4,4,4,4,4,4,4,4),  // 90 - 97 
PCK4BITS(4,4,4,4,4,4,4,4),  // 98 - 9f 
PCK4BITS(4,3,3,3,3,3,3,3),  // a0 - a7 
PCK4BITS(3,3,3,3,3,3,3,3),  // a8 - af 
PCK4BITS(3,3,3,3,3,3,3,3),  // b0 - b7 
PCK4BITS(3,3,3,3,3,3,3,3),  // b8 - bf 
PCK4BITS(3,3,3,3,3,3,3,3),  // c0 - c7 
PCK4BITS(3,3,3,3,3,3,3,3),  // c8 - cf 
PCK4BITS(3,3,3,3,3,3,3,3),  // d0 - d7 
PCK4BITS(3,3,3,3,3,3,3,3),  // d8 - df 
PCK4BITS(3,3,3,3,3,3,3,3),  // e0 - e7 
PCK4BITS(3,3,3,3,3,3,3,3),  // e8 - ef 
PCK4BITS(3,3,3,3,3,3,3,3),  // f0 - f7 
PCK4BITS(3,3,3,3,3,3,3,0)   // f8 - ff 
};


static u32 BIG5_st [ 3] = 
{
PCK4BITS(eError,eStart,eStart,     3,eError,eError,eError,eError),//00-07 
PCK4BITS(eError,eError,eItsMe,eItsMe,eItsMe,eItsMe,eItsMe,eError),//08-0f 
PCK4BITS(eError,eStart,eStart,eStart,eStart,eStart,eStart,eStart) //10-17 
};

static const u32 BIG5CharLenTable[] = {0, 1, 1, 2, 0};

SMModel BIG5_SMModel = 
{
  {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, BIG5_cls },
    5,
  {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, BIG5_st },
  BIG5CharLenTable,
  "Big5"
};

// for the Prober 
ProbingState big5_decode(const char* stuff, u32 len );
void         big5_reset(void);
int          big5_score(void);

// for the CharDistributer
s32 big5_get_order(const char* str);

UED_PROBER( BIG5_PROBE, big5_decode, big5_reset, big5_score );

UED_STATE_MACHINE(BIG5_ESM, &BIG5_SMModel, &BIG5_PROBE);

UED_CHAR_DISTRIBUTER( BIG5_CHARDIST, BIG5CharToFreqOrder, BIG5_TABLE_SIZE, \
	BIG5_TYPICAL_DISTRIBUTION_RATIO,  big5_get_order );

ProbingState big5_decode(const char* stuff, u32 len )
{
        SMState state;
        u32 i=0, char_len=0;
	static char last_char[2]={ 0, 0 };
        UED_DEBUG( "big5 decoding started\n" );
        for (i = 0; i < len; i++){
                state = next_state( &BIG5_ESM, stuff[i]);
                if (state == eError) {
        		UED_DEBUG( "big5 not found\n" );
                        return ( BIG5_PROBE.probe_state = eNotMe );
                }
                if (state == eItsMe) {
        		UED_DEBUG( "find big5\n" );
                        return ( BIG5_PROBE.probe_state = eFoundIt );
                }
                if (state == eStart) {
			char_len = GetCurrentCharLen(&BIG5_ESM);
			if( i == 0 ){
				last_char[1] = stuff[0];
				analysis_one_char(&BIG5_CHARDIST, last_char, char_len);
                                BIG5_PROBE.numer_char++;
			} else {
 				analysis_one_char(&BIG5_CHARDIST, stuff+i-1, char_len);
			}
                }
        }
	last_char[0] = stuff[len-1];
	if (BIG5_PROBE.probe_state == eDetecting) {
		UED_DEBUG( "may be big5, read total char=%d, score=%d\n", 
			BIG5_CHARDIST.mTotalChars,  BIG5_PROBE.score() );
		if ( ( BIG5_CHARDIST.mTotalChars > ENOUGH_DATA_THRESHOLD ) &&
			(BIG5_PROBE.score() > 65) ){
			BIG5_PROBE.probe_state = eFoundIt;
		}
	}
        return BIG5_PROBE.probe_state;
}

void big5_reset(void)
{
        BIG5_PROBE.numer_char = 0;
        BIG5_PROBE.probe_state= eDetecting;
}

int big5_score(void)
{
	return BIG5_CHARDIST.score(&BIG5_CHARDIST);
}

//  for big5 encoding, we are interested 
//  first  byte range: 0xa4 -- 0xfe
//  second byte range: 0x40 -- 0x7e , 0xa1 -- 0xfe
//  no validation needed here. State machine has done that
s32 big5_get_order(const char* str) 
{
	if ((u8)*str >= (u8)0xa4)
		if ((u8)str[1] >= (u8)0xa1)
			return 157*((u8)str[0]-(u8)0xa4) + (u8)str[1] - (u8)0xa1 +63;
		else
			return 157*((u8)str[0]-(u8)0xa4) + (u8)str[1] - (u8)0x40;
	else
		return -1;
}

#endif
