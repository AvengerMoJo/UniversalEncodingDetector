#include <config.h>

#include <stdio.h>
#include <string.h>

#include <../lib/linkedlist.h>
#include <../lib/modules.h>

#include <../test/testing_framework.h>

DEFINE_MODULE_DIRECTORY( ued_test, "test", UED_TESTING_ABI );

static ModuleEntry     *test_module = NULL;
static const TestFuncs *test_funcs  = NULL;

void testing(){
    LinkedList *l;
    modules_explore_directory( &ued_test );

    list_foreach( l, ued_test.entries ) {
        ModuleEntry     *module = (ModuleEntry*) l;
        const TestFuncs *funcs;

        funcs = module_ref( module );
        if (!funcs)
            continue;

        test_module = module;
        test_funcs  = funcs;

	UED_DEBUG( "String from framework %s\n",  funcs->GetInfo() ); 
    }
}
