#ifndef __UED__TESTING_FRAMEWORK_H__
#define __UED__TESTING_FRAMEWORK_H__

#include <config.h>

#include <stdio.h>
#include <string.h>

#include <../lib/linkedlist.h>
#include <../lib/modules.h>

DECLARE_MODULE_DIRECTORY( ued_test );

#define UED_TESTING_ABI 1

typedef struct {
	char* (*GetInfo)( void );
} TestFuncs;

void testing(); 

#endif
