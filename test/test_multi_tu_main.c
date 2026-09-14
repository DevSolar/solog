#include "../solog.h"
#include "test_multi_tu_sub.h"
#include "test_helper.h"
#include "greatest.h"

TEST test_multi_tu( void )
{
    char const * const out_file = "test_multi_tu.out";

    solog_config.level = SOLOG_LVL_INFO;
    solog_config.features = SOLOG_FEATURE_LEVEL;

    FILE * f = fopen( out_file, "w" );
    ASSERT( f != NULL );
    solog_config.stream = f;

    SOLOG( INFO, "main unit start" );
    log_from_sub_unit(); /* debug suppressed at default INFO level */

    solog_config.level = SOLOG_LVL_DEBUG;
    log_from_sub_unit(); /* both INFO and DEBUG printed */

    SOLOG( INFO, "main unit end" );

    fclose( f );
    solog_config.stream = NULL;

    char const * const expected =
        " INFO | main unit start\n"
        " INFO | message from sub unit\n"
        " INFO | message from sub unit\n"
        "DEBUG | debug from sub unit\n"
        " INFO | main unit end\n";

    ASSERT( check_file_contents( out_file, expected ) );
    remove( out_file );
    PASS();
}

SUITE( multi_tu_suite )
{
    RUN_TEST( test_multi_tu );
}
