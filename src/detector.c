#include <detector.h>

DEFINE_MODULE_DIRECTORY( ued_detector, "detectors", UED_DETECTOR_ABI );

static ModuleEntry     *detector_module = NULL;
static const DetectorFuncs *detector_funcs  = NULL;
static const LinkedList *detector_result = NULL;

LinkedList* detect( char* stuff, int length ){
    LinkedList *l, *result=NULL;
    EncodingEntry *node=NULL;

    UED_DEBUG( "exploring...\n" );
    modules_explore_directory( &ued_detector );

    list_foreach( l, ued_detector.entries ) {
        ModuleEntry     *module = (ModuleEntry*) l;
        const DetectorFuncs *funcs;
	LinkedList *tmp;

        funcs = module_ref( module );
        if (!funcs)
            continue;

        detector_module = module;
        detector_funcs  = funcs;

	UED_DEBUG( "initialize...\n" );
	funcs->Initialize(); 
	UED_DEBUG( "detecting ...\n" );
	funcs->DetectEncoding( stuff, length ); 
	UED_DEBUG( "getBestGuessEncodingName...\n" );
	if( (tmp = funcs->GetBestGuessEncodingName()) != NULL ){
		UED_DEBUG( "appending....\n" );
		list_foreach ( node, tmp ) {
		    UED_DEBUG( "In list encoding = %s score = %d\n", node->encoding, node->score);
	            list_prepend( &result, &node->link );
		}
	}
    }
    return result;
}
