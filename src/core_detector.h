#ifndef __UED_CORE_DETECTOR__H__
#define __UED_CORE_DETECTOR__H__

#include <detector.h>

static void
detect_encoding( char*, int );

static LinkedList*  
get_best_guess_encoding_name( );

static LinkedList* 
init( void );

static DetectorFuncs detector_funcs = { 
	     Initialize: init,                                 /* internal initization */
	 DetectEncoding: detect_encoding,                      /* actually map to the detecting call */ 
	GetBestGuessEncodingName: get_best_guess_encoding_name /* return result best encoding name */
};

#define UED_DETECTOR(shortname)                         \
__attribute__((constructor)) void ued_##shortname();    \
                                                        \
void                                                    \
ued_##shortname()                                       \
{                                                       \
     modules_register( &ued_detector,                   \
                       UED_DETECTOR_ABI,                \
                       #shortname, &detector_funcs );   \
}


#endif
