#include <config.h>

#include <stdio.h>
#include <string.h>

#include <../lib/debug.h>
#include <../test/testing.h>
// #include <test/xxx_modules.h>

UED_TESTING(xxx);

static char*
get_info( )
{
	return strdup( "-->String from module<--" );
}
