#ifndef __GB18030_STATEMACHINE_H__
#define __GB18030_STATEMACHINE_H__

#include <types.h>
#include <encoding_state_machine.h>
#include <char_distribution.h>
#include "GB18030Freq.tab"

// the following state machine data was created by perl script in 
// intl/chardet/tools. It should be the same as in PSM detector.
static u32 GB18030_cls [ 256 / 8 ] = 
{
PCK4BITS(1,1,1,1,1,1,1,1),  // 00 - 07 
PCK4BITS(1,1,1,1,1,1,0,0),  // 08 - 0f 
PCK4BITS(1,1,1,1,1,1,1,1),  // 10 - 17 
PCK4BITS(1,1,1,0,1,1,1,1),  // 18 - 1f 
PCK4BITS(1,1,1,1,1,1,1,1),  // 20 - 27 
PCK4BITS(1,1,1,1,1,1,1,1),  // 28 - 2f 
PCK4BITS(3,3,3,3,3,3,3,3),  // 30 - 37 
PCK4BITS(3,3,1,1,1,1,1,1),  // 38 - 3f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 40 - 47 
PCK4BITS(2,2,2,2,2,2,2,2),  // 48 - 4f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 50 - 57 
PCK4BITS(2,2,2,2,2,2,2,2),  // 58 - 5f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 60 - 67 
PCK4BITS(2,2,2,2,2,2,2,2),  // 68 - 6f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 70 - 77 
PCK4BITS(2,2,2,2,2,2,2,4),  // 78 - 7f 
PCK4BITS(5,6,6,6,6,6,6,6),  // 80 - 87 
PCK4BITS(6,6,6,6,6,6,6,6),  // 88 - 8f 
PCK4BITS(6,6,6,6,6,6,6,6),  // 90 - 97 
PCK4BITS(6,6,6,6,6,6,6,6),  // 98 - 9f 
PCK4BITS(6,6,6,6,6,6,6,6),  // a0 - a7 
PCK4BITS(6,6,6,6,6,6,6,6),  // a8 - af 
PCK4BITS(6,6,6,6,6,6,6,6),  // b0 - b7 
PCK4BITS(6,6,6,6,6,6,6,6),  // b8 - bf 
PCK4BITS(6,6,6,6,6,6,6,6),  // c0 - c7 
PCK4BITS(6,6,6,6,6,6,6,6),  // c8 - cf 
PCK4BITS(6,6,6,6,6,6,6,6),  // d0 - d7 
PCK4BITS(6,6,6,6,6,6,6,6),  // d8 - df 
PCK4BITS(6,6,6,6,6,6,6,6),  // e0 - e7 
PCK4BITS(6,6,6,6,6,6,6,6),  // e8 - ef 
PCK4BITS(6,6,6,6,6,6,6,6),  // f0 - f7 
PCK4BITS(6,6,6,6,6,6,6,0)   // f8 - ff 
};


static u32 GB18030_st [ 6] = 
{
PCK4BITS(eError,eStart,eStart,eStart,eStart,eStart,     3,eError),//00-07 
PCK4BITS(eError,eError,eError,eError,eError,eError,eItsMe,eItsMe),//08-0f 
PCK4BITS(eItsMe,eItsMe,eItsMe,eItsMe,eItsMe,eError,eError,eStart),//10-17 
PCK4BITS(     4,eError,eStart,eStart,eError,eError,eError,eError),//18-1f 
PCK4BITS(eError,eError,     5,eError,eError,eError,eItsMe,eError),//20-27 
PCK4BITS(eError,eError,eStart,eStart,eStart,eStart,eStart,eStart) //28-2f 
};

// To be accurate, the length of class 6 can be either 2 or 4. 
// But it is not necessary to discriminate between the two since 
// it is used for frequency analysis only, and we are validing 
// each code range there as well. So it is safe to set it to be 
// 2 here. 
static const u32 GB18030CharLenTable[] = {0, 1, 1, 1, 1, 1, 2};

SMModel GB18030_SMModel = 
{
  {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, GB18030_cls },
   7,
  {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, GB18030_st },
  GB18030CharLenTable,
  "GB18030",
};

// for the Prober 
ProbingState gb18030_decode(const char* stuff, u32 len );
void         gb18030_reset(void);
int          gb18030_score(void);

// for the CharDistributer
s32 gb18030_get_order(const char* str);

UED_PROBER( GB18030_PROBE, gb18030_decode, gb18030_reset, gb18030_score );

UED_STATE_MACHINE(GB18030_ESM, &GB18030_SMModel, &GB18030_PROBE);

UED_CHAR_DISTRIBUTER( GB18030_CHARDIST, GB18030CharToFreqOrder, GB18030_TABLE_SIZE, \
	GB18030_TYPICAL_DISTRIBUTION_RATIO,  gb18030_get_order );

ProbingState gb18030_decode(const char* stuff, u32 len )
{
        SMState state;
        u32 i=0, char_len=0;
	static char last_char[2]={ 0, 0 };
        UED_DEBUG( "gb18030 decoding started\n" );
        for (i = 0; i < len; i++){
                state = next_state( &GB18030_ESM, stuff[i]);
                if (state == eError) {
        		UED_DEBUG( "gb18030 not found\n" );
                        return ( GB18030_PROBE.probe_state = eNotMe );
                }
                if (state == eItsMe) {
        		UED_DEBUG( "find gb18030\n" );
                        return ( GB18030_PROBE.probe_state = eFoundIt );
                }
                if (state == eStart) {
			char_len = GetCurrentCharLen(&GB18030_ESM);
			if( i == 0 ){
				last_char[1] = stuff[0];
				analysis_one_char(&GB18030_CHARDIST, last_char, char_len);
                                GB18030_PROBE.numer_char++;
			} else {
 				analysis_one_char(&GB18030_CHARDIST, stuff+i-1, char_len);
			}
                }
        }
	last_char[0] = stuff[len-1];
	if (GB18030_PROBE.probe_state == eDetecting) {
		UED_DEBUG( "may be gb18030, read total char=%d, score=%d\n", 
			GB18030_CHARDIST.mTotalChars,  GB18030_PROBE.score() );
		if ( ( GB18030_CHARDIST.mTotalChars > ENOUGH_DATA_THRESHOLD ) &&
			(GB18030_PROBE.score() > 65) ){
			GB18030_PROBE.probe_state = eFoundIt;
		}
	}
        return GB18030_PROBE.probe_state;
}

void gb18030_reset(void)
{
        GB18030_PROBE.numer_char = 0;
        GB18030_PROBE.probe_state= eDetecting;
}

int gb18030_score(void)
{
	return GB18030_CHARDIST.score(&GB18030_CHARDIST);
}

//for GB18030 encoding, we are interested 
//  first  byte range: 0xb0 -- 0xfe
//  second byte range: 0xa1 -- 0xfe
//no validation needed here. State machine has done that
s32 gb18030_get_order(const char* str) 
{
	if ((u8)*str >= (u8)0xb0 && (u8)str[1] >= (u8)0xa1)
		return 94*((u8)str[0]-(u8)0xb0) + (u8)str[1] - (u8)0xa1;
	else
		return -1;
}

#endif
