#include "../solog.h"
#include "test_helper.h"
#include "greatest.h"

static int side_effect_counter = 0;

static int side_effect( void )
{
    return ++side_effect_counter;
}

TEST test_default_config( void )
{
    solog_config.stream = NULL;
    solog_config.level = SOLOG_LVL_INFO;
    solog_config.features = 0;

    ASSERT_EQ( NULL, solog_config.stream );
    ASSERT_EQ( SOLOG_LVL_INFO, solog_config.level );
    ASSERT_EQ( 0, solog_config.features );
    PASS();
}

TEST test_basic_logging( void )
{
    char const * const out_file = "test_basic.out";

    solog_config.level = SOLOG_LVL_INFO;
    solog_config.features = 0;
    side_effect_counter = 0;

    FILE * f = fopen( out_file, "w" );
    ASSERT( f != NULL );
    solog_config.stream = f;

    /* 1. Default INFO level: TRACE and DEBUG should be filtered */
    SOLOG( TRACE, "msg trace %d", 1 );
    SOLOG( DEBUG, "msg debug %d", 2 );
    SOLOG( INFO,  "msg info %d", 3 );
    SOLOG( WARN,  "msg warn %d", 4 );
    SOLOG( ERR,   "msg err %d", 5 );
    SOLOG( FAIL,  "msg fail %d", 6 );

    /* 2. Short-circuiting: argument evaluation */
    SOLOG( DEBUG, "eval %d", side_effect() ); /* filtered, side_effect should not run */
    ASSERT_EQ( 0, side_effect_counter );

    SOLOG( WARN,  "eval %d", side_effect() ); /* logged, side_effect runs */
    ASSERT_EQ( 1, side_effect_counter );

    /* 3. Runtime level modification */
    solog_config.level = SOLOG_LVL_TRACE;
    SOLOG( TRACE, "trace now visible" );
    SOLOG( DEBUG, "debug now visible" );

    solog_config.level = SOLOG_LVL_FAIL;
    SOLOG( ERR,  "err filtered" );
    SOLOG( FAIL, "fail visible" );

    fclose( f );
    solog_config.stream = NULL;

    char const * const expected =
        "msg info 3\n"
        "msg warn 4\n"
        "msg err 5\n"
        "msg fail 6\n"
        "eval 1\n"
        "trace now visible\n"
        "debug now visible\n"
        "fail visible\n";

    ASSERT( check_file_contents( out_file, expected ) );
    remove( out_file );
    PASS();
}

TEST test_allocator( void )
{
    solog_freea( NULL );

    /* Small allocation (stack) */
    char * p_small = (char *)solog_malloca( 64 );
    ASSERT( p_small != NULL );
    memset( p_small, 'X', 64 );
    ASSERT_EQ( 'X', p_small[ 0 ] );
    solog_freea( p_small );

    /* Large allocation (heap) */
    char * p_large = (char *)solog_malloca( 2048 );
    ASSERT( p_large != NULL );
    memset( p_large, 'Y', 2048 );
    ASSERT_EQ( 'Y', p_large[ 0 ] );
    ASSERT_EQ( 'Y', p_large[ 2047 ] );
    solog_freea( p_large );

    PASS();
}

SUITE( basic_suite )
{
    RUN_TEST( test_default_config );
    RUN_TEST( test_basic_logging );
    RUN_TEST( test_allocator );
}
