#include "../solog.h"
#include "test_helper.h"
#include "greatest.h"

TEST test_color_feature( void )
{
    char const * const out_file = "test_color_feature.out";

    solog_config.features = SOLOG_FEATURE_COLOR | SOLOG_FEATURE_LEVEL;

    ASSERT_EQ( SOLOG_FEATURE_COLOR | SOLOG_FEATURE_LEVEL, solog_config.features );

    FILE * f = fopen( out_file, "w" );
    ASSERT( f != NULL );
    solog_config.stream = f;
    solog_config.level = SOLOG_LVL_TRACE;

    /* 1. Colorized log messages */
    SOLOG( TRACE, "trace message" );
    SOLOG( DEBUG, "debug message" );
    SOLOG( INFO,  "info message" );
    SOLOG( WARN,  "warn message" );
    SOLOG( ERR,   "err message" );
    SOLOG( FAIL,  "fail message" );

    /* 2. Runtime disable color feature */
    solog_config.features &= ~SOLOG_FEATURE_COLOR;
    SOLOG( INFO, "color disabled at runtime" );

    /* 3. Runtime re-enable color feature */
    solog_config.features |= SOLOG_FEATURE_COLOR;
    SOLOG( INFO, "color re-enabled at runtime" );

    fclose( f );
    solog_config.stream = NULL;

    char const * const expected =
        "\033[90mTRACE | trace message\033[39m\n"
        "DEBUG | debug message\n"
        "\033[1m INFO | info message\033[22m\n"
        "\033[1;33m WARN | warn message\033[39;22m\n"
        "\033[1;31m  ERR | err message\033[39;22m\n"
        "\033[41;97m FAIL | fail message\033[49;39m\n"
        " INFO | color disabled at runtime\n"
        "\033[1m INFO | color re-enabled at runtime\033[22m\n";

    ASSERT( check_file_contents( out_file, expected ) );
    remove( out_file );
    PASS();
}

SUITE( color_feature_suite )
{
    RUN_TEST( test_color_feature );
}
