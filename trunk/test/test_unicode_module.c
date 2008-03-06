#include <stdio.h>
#include <stdlib.h>
#include <../lib/linkedlist.h>
#include <../src/detector.h>

int main( int argc, char** argv ){
    int i=0;
    EncodingEntry* node=NULL;
    LinkedList* result=NULL;

#if BUILD_UED_DEBUG
    ued_debug_set_section( "Main" , true );
    ued_debug_set_section( "Library" , true );
    ued_debug_set_section( "Detectors" , true );
#endif 

    FILE *f = fopen( argv[1], "r" ); 
    if( f ) { 
	    char *stuff = calloc( 1, 10000 );
	    //int length = fscanf( stdin, "%n$\n" ,stuff); 
	    while( 1 ){
		int tmp = fgetc( f );
		if( tmp == EOF )
			break;
		else stuff[i++]=(char)tmp;
	    }
	    //while( stuff[i++] = (char) fgetc( f ) );
	    // for( i=0; stuff[i-1]!=EOF; i++){
	    int length = i;
	    //int length = fscanf( stdin, "%n$\n" ,stuff); 
	    printf( "%s %d %d\n", stuff, strlen(stuff), length ); 
            result = detect( stuff, length );
            list_foreach( node,  result ){ 
	    	printf( "Encoding = %s, Score = %d\n", node->encoding, node->score );
            }
    }

    return 0;
}
