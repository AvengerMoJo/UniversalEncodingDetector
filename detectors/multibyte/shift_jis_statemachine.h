#ifndef __SHIFT_JIS_STATEMACHINE_H__
#define __SHIFT_JIS_STATEMACHINE_H__

#include <../../lib/encoding_state_machine.h>
#include "JISFreq.tab"

static u32 SJIS_cls [ 256 / 8 ] =
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
PCK4BITS(2,2,2,2,2,2,2,2),  // 40 - 47 
PCK4BITS(2,2,2,2,2,2,2,2),  // 48 - 4f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 50 - 57 
PCK4BITS(2,2,2,2,2,2,2,2),  // 58 - 5f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 60 - 67 
PCK4BITS(2,2,2,2,2,2,2,2),  // 68 - 6f 
PCK4BITS(2,2,2,2,2,2,2,2),  // 70 - 77 
PCK4BITS(2,2,2,2,2,2,2,1),  // 78 - 7f 
PCK4BITS(3,3,3,3,3,3,3,3),  // 80 - 87 
PCK4BITS(3,3,3,3,3,3,3,3),  // 88 - 8f 
PCK4BITS(3,3,3,3,3,3,3,3),  // 90 - 97 
PCK4BITS(3,3,3,3,3,3,3,3),  // 98 - 9f 
//0xa0 is illegal in sjis encoding, but some pages does 
//contain such byte. We need to be more error forgiven.
PCK4BITS(2,2,2,2,2,2,2,2),  // a0 - a7     
PCK4BITS(2,2,2,2,2,2,2,2),  // a8 - af 
PCK4BITS(2,2,2,2,2,2,2,2),  // b0 - b7 
PCK4BITS(2,2,2,2,2,2,2,2),  // b8 - bf 
PCK4BITS(2,2,2,2,2,2,2,2),  // c0 - c7 
PCK4BITS(2,2,2,2,2,2,2,2),  // c8 - cf 
PCK4BITS(2,2,2,2,2,2,2,2),  // d0 - d7 
PCK4BITS(2,2,2,2,2,2,2,2),  // d8 - df 
PCK4BITS(3,3,3,3,3,3,3,3),  // e0 - e7 
PCK4BITS(3,3,3,3,3,4,4,4),  // e8 - ef 
PCK4BITS(4,4,4,4,4,4,4,4),  // f0 - f7 
PCK4BITS(4,4,4,4,4,0,0,0)   // f8 - ff 
};


static u32 SJIS_st [ 3] = 
{
PCK4BITS(eError,eStart,eStart,     3,eError,eError,eError,eError),//00-07 
PCK4BITS(eError,eError,eError,eError,eItsMe,eItsMe,eItsMe,eItsMe),//08-0f 
PCK4BITS(eItsMe,eItsMe,eError,eError,eStart,eStart,eStart,eStart) //10-17 
};

static const u32 SJISCharLenTable[] = {0, 1, 1, 2, 0, 0};

SMModel SJIS_SMModel = 
{
  {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, SJIS_cls },
   6,
  {eIdxSft4bits, eSftMsk4bits, eBitSft4bits, eUnitMsk4bits, SJIS_st },
  SJISCharLenTable,
  "Shift_JIS",
};

// for the Prober
ProbingState shift_jis_decode(const char* stuff, u32 len );
void         shift_jis_reset(void);
int          shift_jis_score(void);

// for the CharDistributer
s32 shift_jis_get_order(const char* str);

UED_PROBER( SHIFT_JIS_PROBE, shift_jis_decode, shift_jis_reset, shift_jis_score ); 

UED_STATE_MACHINE(SHIFT_JIS_ESM, &SJIS_SMModel, &SHIFT_JIS_PROBE);

UED_CHAR_DISTRIBUTER( SHIFT_JIS_CHARDIST, JISCharToFreqOrder, JIS_TABLE_SIZE, \
        JIS_TYPICAL_DISTRIBUTION_RATIO,  shift_jis_get_order );

ProbingState shift_jis_decode(const char* stuff, u32 len ){
	SMState state;
	u32 i=0,char_len=0; 
        static char last_char[2]={ 0, 0 };
	UED_DEBUG( "Shift_JIS decoding started\n" );
	for (i = 0; i < len; i++){
		state = next_state( &SHIFT_JIS_ESM, stuff[i]);
		if (state == eError) {
			return ( SHIFT_JIS_PROBE.probe_state = eNotMe );
		}
		if (state == eItsMe) {
			return ( SHIFT_JIS_PROBE.probe_state = eFoundIt );
		} 
		if (state == eStart) {
			char_len = GetCurrentCharLen(&SHIFT_JIS_ESM);
			if( i == 0 ){
				last_char[1] = stuff[0];
                                analysis_one_char(&SHIFT_JIS_CHARDIST, last_char, char_len);
                                SHIFT_JIS_PROBE.numer_char++;
			} else { 
                                analysis_one_char(&SHIFT_JIS_CHARDIST, stuff+i-1, char_len);
			}
		}
	}
	last_char[0] = stuff[len-1];
	UED_DEBUG( "may be shift_jis, read total char=%d, score=%d\n", \
			SHIFT_JIS_CHARDIST.mTotalChars,  SHIFT_JIS_PROBE.score() );
	if ( ( SHIFT_JIS_CHARDIST.mTotalChars > ENOUGH_DATA_THRESHOLD ) &&
                        (SHIFT_JIS_PROBE.score() > 65) ){
                        SHIFT_JIS_PROBE.probe_state = eFoundIt;
	}
	return SHIFT_JIS_PROBE.probe_state;
}

void shift_jis_reset(void){
	SHIFT_JIS_PROBE.numer_char = 0;
	SHIFT_JIS_PROBE.probe_state= eDetecting;
}

int shift_jis_score(void)
{
        return SHIFT_JIS_CHARDIST.score(&SHIFT_JIS_CHARDIST);
}

//  for sjis encoding, we are interested 
//  first  byte range: 0x81 -- 0x9f , 0xe0 -- 0xfe
//  second byte range: 0x40 -- 0x7e,  0x81 -- oxfe
//  no validation needed here. State machine has done that
s32 shift_jis_get_order(const char* str)
{ 
	s32 order;
	if ((u8)*str >= (u8)0x81 && (u8)*str <= (u8)0x9f)
		order = 188 * ((u8)str[0]-(u8)0x81);
	else if ((u8)*str >= (u8)0xe0 && (u8)*str <= (u8)0xef)
		order = 188 * ((u8)str[0]-(u8)0xe0 + 31);
	else
		return -1;
	order += (u8)*(str+1) - 0x40;
	if ((u8)str[1] > (u8)0x7f)
		order--;
	return order;
}

#endif
