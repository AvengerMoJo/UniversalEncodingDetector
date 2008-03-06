#include <config.h>

#include <stdio.h>
#include <string.h>

#include <../lib/linkedlist.h>
#include <../lib/modules.h>
#include <../test/testing_framework.h>

int main(int argc, char* argv[]){
#if BUILD_UED_DEBUG
    ued_debug_set_section( "Main/Configurator" , true );
    ued_debug_set_section( "Library" , true );
    ued_debug_set_section( "Main/Configurator" , true );
#endif

    UEDInit( &argc, &argv );
    testing( "new stuff " );

    return 0;
}
