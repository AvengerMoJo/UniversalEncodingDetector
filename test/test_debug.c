#define SIGTRAP_TERMINATE 1
#include "debug.h"

UED_DEBUG_SECTION( TESTING, "Main/Testing", "The Main testing program" );

void 
entering()
{
	UED_DEBUG_ENTER( TESTING, "Crazy in\n" );
	UED_DEBUG_EXIT( TESTING, "Crazy out\n" );
}

int main(){
	UED_DEBUG( "Testing let's get started %d\n", 1 );
	ued_debug_set_section( "Main" , true );
	entering();
	ued_debug_set_section( "Main" , false );
	entering();

	ued_debug_set_section( "Main/Testing" , true );
	entering();
	ued_debug_set_section( "Main/Testing" , false );
	entering();

	ued_debug_set_section( "Main" , true );
	entering();
	ued_debug_set_section( "Main" , false );
	entering();

	return 0;
}
