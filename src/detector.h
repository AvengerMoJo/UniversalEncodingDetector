#ifndef __UED_DETECTOR_H__
#define __UED_DETECTOR_H__

#include <../lib/linkedlist.h>
#include <../lib/modules.h>

DECLARE_MODULE_DIRECTORY( ued_detector );

#define UED_DETECTOR_ABI 1

typedef struct {
     LinkedList  link;
     int         score;
     char*       encoding;
} EncodingEntry;


typedef struct {
	LinkedList* (*Initialize)( void );
	void (*DetectEncoding)( char*, int );
	LinkedList* (*GetBestGuessEncodingName)( void );
} DetectorFuncs;

LinkedList* detect( char*, int ); 

#endif
