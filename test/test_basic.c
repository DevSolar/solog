#define SOLOG_IMPLEMENTATION
#include "../solog.h"
#include "test_helper.h"
#include <assert.h>

static int side_effect_counter = 0;

static int side_effect( void )
{
    return ++side_effect_counter;
}

int main( void )
{
    char const * const out_file = "test_basic.out";

    /* Assert default configuration */
    assert( solog_config.stream == NULL );
    assert( solog_config.level == SOLOG_LVL_INFO );
    assert( solog_config.features == 0 );

    FILE * f = fopen( out_file, "w" );
    assert( f != NULL );
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
    assert( side_effect_counter == 0 );

    SOLOG( WARN,  "eval %d", side_effect() ); /* logged, side_effect runs */
    assert( side_effect_counter == 1 );

    /* 3. Runtime level modification */
    solog_config.level = SOLOG_LVL_TRACE;
    SOLOG( TRACE, "trace now visible" );
    SOLOG( DEBUG, "debug now visible" );

    solog_config.level = SOLOG_LVL_FAIL;
    SOLOG( ERR,  "err filtered" );
    SOLOG( FAIL, "fail visible" );

    fclose( f );

    char const * const expected =
        "msg info 3\n"
        "msg warn 4\n"
        "msg err 5\n"
        "msg fail 6\n"
        "eval 1\n"
        "trace now visible\n"
        "debug now visible\n"
        "fail visible\n";

    if ( !check_file_contents( out_file, expected ) )
    {
        return 1;
    }

    remove( out_file );
    printf( "PASS: test_basic\n" );
    return 0;
}
