#include "../solog.h"
#include "test_helper.h"
#include "greatest.h"

TEST test_level_feature( void )
{
    char const * const out_file = "test_level_feature.out";

    solog_config.features = SOLOG_FEATURE_LEVEL;

    ASSERT_EQ( SOLOG_FEATURE_LEVEL, solog_config.features );

    FILE * f = fopen( out_file, "w" );
    ASSERT( f != NULL );
    solog_config.stream = f;
    solog_config.level = SOLOG_LVL_TRACE;

    /* 1. All valid levels formatted with prefixes */
    SOLOG( TRACE, "trace message" );
    SOLOG( DEBUG, "debug message" );
    SOLOG( INFO,  "info message" );
    SOLOG( WARN,  "warn message" );
    SOLOG( ERR,   "err message" );
    SOLOG( FAIL,  "fail message" );

    /* 2. Illegal / out-of-range levels via direct solog() call */
    solog( (solog_level_t)99, __FILE__, __func__, __LINE__, "illegal level above" );
    solog( (solog_level_t)-1, __FILE__, __func__, __LINE__, "illegal level below" );

    /* 3. Runtime disable feature */
    solog_config.features &= ~SOLOG_FEATURE_LEVEL;
    SOLOG( INFO, "level prefix disabled at runtime" );

    /* 4. Runtime re-enable feature */
    solog_config.features |= SOLOG_FEATURE_LEVEL;
    SOLOG( INFO, "level prefix re-enabled at runtime" );

    fclose( f );
    solog_config.stream = NULL;

    char const * const expected =
        "TRACE | trace message\n"
        "DEBUG | debug message\n"
        " INFO | info message\n"
        " WARN | warn message\n"
        "  ERR | err message\n"
        " FAIL | fail message\n"
        "ILLGL | illegal level above\n"
        "ILLGL | illegal level below\n"
        "level prefix disabled at runtime\n"
        " INFO | level prefix re-enabled at runtime\n";

    ASSERT( check_file_contents( out_file, expected ) );
    remove( out_file );
    PASS();
}

SUITE( level_feature_suite )
{
    RUN_TEST( test_level_feature );
}
