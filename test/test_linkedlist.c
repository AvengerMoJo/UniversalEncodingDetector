#include "linkedlist.h"

typedef struct {
     LinkedList  link;
     int         number;
} TestEntry;

int main(){
    TestEntry  *node, *node9,*nullnode=NULL;
    LinkedList *testlink=NULL; 
    LinkedList *nulllink=NULL; 
    int i = 0;

    // Testing append and foreach 
    printf( "**** Testing append from 0 to 4 into the list ****\n", i );
    for( i=0; i< 5 ; i++) {
	bool find = false; 
        list_foreach ( node, testlink) {
          if ( node->number == i ){
               find = true;
               break;
          }
        }
        if( !find ) {
            printf( "Append number %d into the list\n", i );
            node = calloc( 1, sizeof(TestEntry) );
            if (node) {
                node ->number = i;
                list_append( &testlink, &node->link );
            }
        } else { 
            printf( "Find i %d in the list already, so not appending\n", node->number ); 
	}
    }
    list_foreach ( node, testlink) {
            printf( "In list i = %d\n", node->number ); 
    }
    printf( "**** Testing append NULL into the NULL list ****\n", i );
    list_append( &nulllink , &nullnode->link );

    printf( "**** Testing append 5 into a NULL list ****\n", i );
    node = calloc( 1, sizeof(TestEntry) );
    if (node) {
	node ->number = 5;
	list_append( &nulllink , &node->link );
    }
    list_foreach ( node, nulllink) {
            printf( "In NULL list i = %d\n", node->number ); 
    }
    free(node); 
    nulllink=NULL;

    // Testing prepend 
    printf( "**** Testing prepend 9 to the from of the list ****\n", i );
    printf( "Prepend number 9 into the list\n" );
    node9 = calloc( 1, sizeof(TestEntry) );
    node9->number = 9;
    list_prepend( &testlink, &node9->link );
    printf( "**** Testing prepend NULL into the NULL list ****\n", i );
    list_prepend( &nulllink , &nullnode->link );
    printf( "**** Testing prepend from 5 into a NULL list ****\n", i );
    node = calloc( 1, sizeof(TestEntry) );
    if (node) {
	node ->number = 5;
	list_prepend( &nulllink , &node->link );
    }
    list_foreach ( node, nulllink) {
            printf( "In NULL list i = %d\n", node->number ); 
    }
    free(node); 
    nulllink=NULL;

    // Testing contain 
    printf( "**** Testing contain 9 in list ****\n", i );
    if( list_contains_element_EXPENSIVE ( testlink, &node9->link ) ){
        printf( "Correct it get 9 in the list!\n" );
    } else {
        printf( "InCorrect it get 9 in the list!\n" );
    }
    printf( "**** Testing contain 9 in nulllist ****\n", i );
    if( list_contains_element_EXPENSIVE ( nulllink, &node9->link ) ){
        printf( "InCorrect it should not get 9 in the nulllist!\n" );
    } else {
        printf( "Correct it should not get anything in nulllist!\n" );
    }
    printf( "**** Testing contain NULL in nulllist ****\n", i );
    if( list_contains_element_EXPENSIVE ( nulllink, &nullnode->link ) ){
        printf( "InCorrect it should not get NULL in the NULL list!\n" );
    } else {
        printf( "Correct should not return NULL in nulllist!\n" );
    }
    list_foreach ( node, testlink) {
            printf( "In list i = %d\n", node->number ); 
    }

    // Testing remove
    printf( "**** Testing list remove 9 ****\n", i );
    list_remove( &testlink, &node9->link ); 
    if( list_contains_element_EXPENSIVE ( testlink, &node9->link ) ){
        printf( "InCorrect it should not get 9 in the list!\n" );
    } else {
        printf( "Correct it remove 9 from the list!\n" );
    }
    printf( "**** Testing list remove 9 from the nulllist ****\n", i );
    list_remove( &nulllink, &nullnode->link ); 
    printf( "**** Testing list remove NULL from the nulllist ****\n", i );
    list_remove( &nulllink, &nullnode->link ); 
    list_foreach ( node, testlink) {
            printf( "In list i = %d\n", node->number ); 
    }

    // Testing count 
    printf( "**** Testing list count ****\n", i );
    printf( "Now the list should get 5 different numbers and it get %d!\n", list_count_elements_EXPENSIVE( testlink ) );
    printf( "**** Testing list count for nulllist ****\n", i );
    printf( "Nulllist should get nothing and it get %d!\n", list_count_elements_EXPENSIVE( nulllink ) );

    // Testing move_to_front 
    printf( "**** Testing move 3 to the front of the list ****\n", i );
    list_foreach ( node, testlink ) {
        if ( node->number == 3 ){
            break;
        }
    }
    list_move_to_front( &testlink,  &node->link );
    list_foreach ( node, testlink) {
            printf( "In list i = %d\n", node->number ); 
    }
    printf( "**** Testing move NULL to the front of the list ****\n", i );
    list_move_to_front( &testlink,  &nullnode->link );
    printf( "**** Testing move NULL to the front of the nulllist ****\n", i );
    list_move_to_front( &nulllink,  &nullnode->link );
    return 0;
}
