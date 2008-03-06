#include "debug.h"
#include "universal_encoding_detector.h"


int main(int argc, char* argv[]){
#if BUILD_UED_DEBUG
        ued_debug_set_section( "Main/Configurator" , true );
#endif
        UEDInit( &argc, &argv );

	return 0;
}
