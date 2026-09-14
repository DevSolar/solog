#include <stdlib.h>
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

#ifdef _WIN32
#define solog_malloca( sz ) my_custom_malloc( sz )
#define solog_freea( ptr ) my_custom_free( ptr )
#else
#define solog_malloc( sz ) my_custom_malloc( sz )
#define solog_free( ptr ) my_custom_free( ptr )
#endif

#include "../solog.h"
#include "greatest.h"

TEST test_custom_allocator( void )
{
    custom_malloc_called = 0;
    custom_free_called = 0;

    /* Test custom heap allocation via solog_malloca */
    char * ptr = (char *)solog_malloca( 2048 );
    ASSERT( ptr != NULL );
    ASSERT_EQ( 1, custom_malloc_called );

    solog_freea( ptr );
    ASSERT_EQ( 1, custom_free_called );

    PASS();
}

SUITE( custom_alloc_suite )
{
    RUN_TEST( test_custom_allocator );
}
