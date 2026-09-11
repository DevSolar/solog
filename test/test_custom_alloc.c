#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

static int custom_malloc_called = 0;
static int custom_free_called = 0;

static void * my_custom_malloc( size_t sz )
{
    custom_malloc_called++;
    return malloc( sz );
}

static void my_custom_free( void * ptr )
{
    custom_free_called++;
    free( ptr );
}

#define solog_malloc( sz ) my_custom_malloc( sz )
#define solog_free( ptr ) my_custom_free( ptr )

#define SOLOG_IMPLEMENTATION
#include "../solog.h"

int main( void )
{
    /* Test custom heap allocation via solog_malloca */
    char * ptr = (char *)solog_malloca( 2048 );
    assert( ptr != NULL );
    assert( custom_malloc_called == 1 );

    solog_freea( ptr );
    assert( custom_free_called == 1 );

    printf( "PASS: test_custom_alloc\n" );
    return 0;
}
