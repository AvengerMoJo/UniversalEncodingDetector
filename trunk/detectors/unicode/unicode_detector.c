#include <config.h>

#include <../src/core_detector.h>

UED_DEBUG_SECTION( UNICODE , "Detectors/Unicode", "Detector Module for Unicode" );

UED_DETECTOR( unicode );

static LinkedList *result; 
static EncodingEntry **unicode;  /* utf8, utf32_LE,utf32_BE, ucs_4_3412, ucs_4_2143, utf16_LE, utf16_BE */
#define UNCODE_ENCODEING_SIZE 7

static LinkedList* 
init(){
	UED_DEBUG_ENTER( UNICODE,  "Initizing unicoding detection list" );
	int i;
	if( unicode == NULL ) { 
		unicode = calloc( UNCODE_ENCODEING_SIZE, sizeof( EncodingEntry* ) ); 
		for( i=0; i<7; i++ ){ 
			unicode[i] = calloc( 1, sizeof( EncodingEntry ) );
		}
		unicode[0]->encoding = strdup( "UTF16-LE" );
		unicode[1]->encoding = strdup( "UTF16-BE" );
		unicode[2]->encoding = strdup( "UTF-8" );
		unicode[3]->encoding = strdup( "UTF32-LE" );
		unicode[4]->encoding = strdup( "UTF32-BE" );
		unicode[5]->encoding = strdup( "UCS4-3412" );
		unicode[6]->encoding = strdup( "UCS4-2142" );
	}
	for( i=0; i<7; i++ ){ 
		unicode[i]->score = 0;
	}
	if( result != NULL ) { 
		for( i=0; i<7; i++){ 
			list_remove( &result, &unicode[i]->link ); 
		}
	}
	UED_DEBUG_EXIT( UNICODE,  "Initizing unicoding detection complete" );
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
	UED_DEBUG_ENTER( UNICODE,  " detecting string length %d, string %s", len, stuff );
	if( len >=1 ) { 
		char utf16_LE[]={ (char)255, (char)254 }; 
		char utf16_BE[]={ (char)254, (char)255 }; 
		char utf8[]={ (char)239, (char)187, (char)191 };
		char utf32_LE[]={ (char)255, (char)254, (char)0, (char)0 }; 
		char utf32_BE[]={ (char)0, (char)0, (char)254, (char)255 }; 
		char ucs_4_3412[]={ (char)254, (char)255, (char)0, (char)0}; 
		char ucs_4_2143[]={ (char)0, (char)0, (char)255, (char)254}; 

		if( strncmp( stuff, utf16_LE, 2 ) == 0 ){
			UED_DEBUG("it is utf16_LE\n");
			unicode[0]->score = 100;
			list_prepend( &result, &unicode[0]->link );
		}
		if( strncmp( stuff, utf16_BE, 2 ) == 0 ){
			UED_DEBUG("it is utf16_BE\n");
			unicode[1]->score = 100;
			list_prepend( &result, &unicode[1]->link );
		}
		if( strncmp( stuff, utf8, 3 ) == 0 ){
			UED_DEBUG("it is utf8\n");
			unicode[2]->score = 100;
			list_prepend( &result, &unicode[2]->link );
		}
		if( strncmp( stuff, utf32_LE, 4 ) == 0 ){
			UED_DEBUG("it is utf32_LE\n");
			unicode[3]->score = 100;
			list_prepend( &result, &unicode[3]->link );
		}
		if( strncmp( stuff, utf32_BE, 4 ) == 0 ){
			UED_DEBUG("it is utf32_BE\n");
			unicode[4]->score = 100;
			list_prepend( &result, &unicode[4]->link );
		}
		if( strncmp( stuff, ucs_4_3412, 4 ) == 0 ){
			UED_DEBUG("it is ucs4_3412\n");
			unicode[5]->score = 100;
			list_prepend( &result, &unicode[5]->link );
		}
		if( strncmp( stuff, ucs_4_2143, 4 ) == 0 ){
			UED_DEBUG("it is ucs4_2143\n");
			unicode[6]->score = 100;
			list_prepend( &result, &unicode[6]->link );
		}
	}
	UED_DEBUG_EXIT( UNICODE,  "Detection Done" );
}
