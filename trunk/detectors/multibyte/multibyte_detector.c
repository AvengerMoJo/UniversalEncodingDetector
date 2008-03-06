#include <config.h>

#include <../../lib/encoding_state_machine.h>
#include <../src/core_detector.h>
#include <unicode_statemachine.h>
#include <gb18030_statemachine.h>
#include <big5_statemachine.h>
#include <euctw_statemachine.h>
#include <euckr_statemachine.h>
#include <shift_jis_statemachine.h>

UED_DEBUG_SECTION( MULTIBYTE , "Detectors/Multibyte", "Detector Module for Multibyte" );

UED_DETECTOR( multibyte );

static int multibyte_keep_next=0;
static LinkedList *result; 
static EncodingEntry **multibyte;  /*  "UTF8", "SJIS", "EUCJP", "GB18030", "EUCKR", "Big5", "EUCTW", */ 

#define MULTIBYTE_ENCODEING_SIZE 7


static LinkedList* 
init(){
	UED_DEBUG_ENTER( MULTIBYTE,  "Initizing multibyte detection module list" );
	int i;
	if( multibyte == NULL ) { 
		multibyte = calloc( MULTIBYTE_ENCODEING_SIZE , sizeof( EncodingEntry* ) ); 
		for( i=0; i<MULTIBYTE_ENCODEING_SIZE; i++ ){ 
			multibyte[i] = calloc( 1, sizeof( EncodingEntry ) );
		}
		multibyte[0]->encoding = strdup( "UTF-8" );
		multibyte[1]->encoding = strdup( "GB18030" );
		multibyte[2]->encoding = strdup( "BIG5" );
		multibyte[3]->encoding = strdup( "EUCTW" );
		multibyte[4]->encoding = strdup( "EUCKR" );
		multibyte[5]->encoding = strdup( "SJIS" );
		multibyte[6]->encoding = strdup( "EUCJP" );
	}
	for( i=0; i<7; i++ ){ 
		multibyte[i]->score = 0;
	}
	if( result != NULL ) { 
		for( i=0; i<7; i++){ 
			list_remove( &result, &multibyte[i]->link ); 
		}
	}
	UED_DEBUG_EXIT( MULTIBYTE,  "Initizing multibyte detection module complete" );
	return result;
}


static LinkedList*
get_best_guess_encoding_name( ){
/*
	if( result ) {
		EncodingEntry *tmp = (EncodingEntry*)(result);	
        	return tmp;
	}
	else
        	return strdup( "NONE" );
*/
	return result;
}

static void
detect_encoding( char* stuff, int len ){
	u32 i = 0, j = 0;
	u32 keep_next = multibyte_keep_next, start=0;
	//int active_esm = MULTIBYTE_ENCODEING_SIZE;
	int active_esm = 5;
	bool is_active_esm[MULTIBYTE_ENCODEING_SIZE];
	SMState state;
	EncodingStateMachine* esms[MULTIBYTE_ENCODEING_SIZE];
	esms[0] = &UTF8_ESM;
	esms[1] = &GB18030_ESM;
	esms[2] = &BIG5_ESM;
	esms[3] = &EUCTW_ESM;
	esms[4] = &EUCKR_ESM;
	esms[5] = &SHIFT_JIS_ESM;
/*
	esms[6] = &ISO2022KR_ESM;
	esms[7] = &ISO2022KR_ESM;
*/

	for( i = 0; i < MULTIBYTE_ENCODEING_SIZE; i++ ){
		is_active_esm[i] = true;
	}

	UED_DEBUG_ENTER( MULTIBYTE,  " detecting string length %d, string %s", len, stuff );
	for( i = 0; i < len; i++ ){
		if (stuff[i] & 0x80){
		UED_DEBUG( "detecting char >%d<\n", stuff[i] );
			if (!keep_next)
				start = i;
			keep_next = 2;
		} else if ( keep_next ){
			if( --keep_next == 0 ) {
				for (j = 0; j < 6; j++) {
					if( !is_active_esm[j] )
						continue;
					state = esms[j]->prober->decoding(stuff + start, i + 1 - start);
					if (state == eFoundIt){
						UED_DEBUG( "Find it is %s\n", esms[j]->mModel->name );
						multibyte[j]->score = esms[j]->prober->score();
						list_prepend( &result, &multibyte[j]->link );
						return;
					} else if (state == eNotMe) {
						UED_DEBUG( "It is not %s\n", esms[j]->mModel->name );
						is_active_esm[j] = false;
						if (--active_esm <= 0)
							return;
					}
				}
			}
		}
        }
	if (keep_next){
		for (j = 0; j < 6 ; j++) {
			if (!is_active_esm[j])
				continue;
			state = esms[j]->prober->decoding(stuff + start, len + 1 - start);
			if (state == eFoundIt) {
				UED_DEBUG( "Find it is %s\n", esms[j]->mModel->name );
				multibyte[j]->score = esms[j]->prober->score();
				list_prepend( &result, &multibyte[j]->link );
				return;
			} else if (state == eNotMe) {
				UED_DEBUG( "It is not %s\n", esms[j]->mModel->name );
				is_active_esm[j] = false;
				if (--active_esm <= 0) {
					return;
				}
			}
		}
	}	
	multibyte_keep_next = keep_next;
	UED_DEBUG_EXIT( MULTIBYTE,  "Detection Done" );
}
