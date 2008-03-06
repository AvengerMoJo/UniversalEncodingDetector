#ifndef __UED__TESTING__H__
#define __UED__TESTING__H__

#include <config.h>

#include <../test/testing_framework.h>

static char*  
get_info( );


static TestFuncs test_funcs = { 
	GetInfo: get_info
};

#define UED_TESTING(shortname)                          \
__attribute__((constructor)) void ued_##shortname();    \
                                                        \
void                                                    \
ued_##shortname()                                       \
{                                                       \
     modules_register( &ued_test,                       \
                       UED_TESTING_ABI,                 \
                       #shortname, &test_funcs );       \
}


#endif
