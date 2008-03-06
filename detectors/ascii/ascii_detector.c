#include <config.h>

#include <../src/core_detector.h>
#include <esc_statemachine.h>

UED_DEBUG_SECTION( ASCII, "Detectors/ASCII", "Detector Module for ASCII" );

UED_DETECTOR( ascii );

static LinkedList *result; 
static EncodingEntry **ascii;  /* ascii only */
#define ASCII_ENCODEING_SIZE 5

static LinkedList* 
init(){
	UED_DEBUG_ENTER( ASCII ,  "Initizing ASCII detection list" );
	int i;
	if( ascii == NULL ) { 
		ascii = calloc( ASCII_ENCODEING_SIZE, sizeof( EncodingEntry* ) ); 
		for( i=0; i<ASCII_ENCODEING_SIZE ; i++ ){ 
			ascii[i] = calloc( 1, sizeof( EncodingEntry ) );
		}
		ascii[0]->encoding = strdup( "ASCII" );
		ascii[1]->encoding = strdup( "HZ-GB-2312" );
		ascii[2]->encoding = strdup( "ISO-2022-CN" );
		ascii[3]->encoding = strdup( "ISO-2022-JP" );
		ascii[4]->encoding = strdup( "ISO-2022-KR" );
	}
	for( i=0; i<ASCII_ENCODEING_SIZE; i++ ){ 
		ascii[i]->score = 0;
	}
	if( result != NULL ) { 
		for( i=0; i<ASCII_ENCODEING_SIZE; i++){ 
			list_remove( &result, &ascii[i]->link ); 
		}
	}
	UED_DEBUG_EXIT( ASCII,  "Initizing ASCII detection complete" );
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
*/
	return result;
}

static void
detect_encoding( char* stuff, int len ){
	int i=0,j=0;
	bool isEsc=false;
	/* all ascii should be lower then 128 */
	char printable[]={ (char)128 };
	char space[]={ (char)160 };
	char lastchar=0; 

	/* for the escaped route */
	bool is_active_esm[4]={ true, true, true, true };
	SMState state;
	int active_esm = 4;
	EncodingStateMachine* esms[4]; 

	UED_DEBUG_ENTER( ASCII,  " detecting string length %d\n", len );
	for( i = 0; i < len; i++ ){
		if ( stuff[i] & printable[0] && (stuff[i] != space[0]) ) {
			UED_DEBUG("it is not ascii\n");
			ascii[0]->score = 0;
			UED_DEBUG_EXIT( ASCII,  "Detection Done" );
			return; 
		} else if( stuff[i] == '\033' || (stuff[i] == '{' && lastchar == '~')) {
			isEsc = true; 
		}
		lastchar = stuff[i];
	}
	if( isEsc ){
		// LinkedList* sm_list=NULL; 
		esms[0] = &HZ_ESM;
		esms[1] = &ISO2022CN_ESM;
		esms[2] = &ISO2022JP_ESM;
		esms[3] = &ISO2022KR_ESM;
		UED_DEBUG("it is escaped ascii\n");
		for( i = 0; i < len; i++ ){
			for (j = 0; j<4; j++) {
				if( ! is_active_esm[j] )
					continue;
				state = next_state( esms[j], stuff[i]);
				if (state == eError) {
					UED_DEBUG("ESM %s eError\n", esms[j]->mModel->name);
					is_active_esm[j] = false;
					// no state machines match all 
					if( --active_esm == 0 ){
						UED_DEBUG("escaped but error all handler\n");
						return;
					}
				} else if (state == eItsMe) {
					UED_DEBUG("it is escaped %s\n", ascii[(j+1)]->encoding );
					ascii[(j+1)]->score = 100; 
					list_append( &result, &ascii[(j+1)]->link );
					return;
				}
			}	
		}
		UED_DEBUG("escaped but no match\n");
	} else { 
		UED_DEBUG("it is ascii\n");
		ascii[0]->score = 100;
		list_append( &result, &ascii[0]->link );
	}
	UED_DEBUG_EXIT( ASCII,  "Detection Done" );
}

