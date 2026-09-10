#define SOLOG_IMPLEMENTATION SOLOG_FEATURE_LEVEL
#include "../solog.h"
#include "test_multi_tu_sub.h"
#include "test_helper.h"
#include <assert.h>

int main( void )
{
    char const * const out_file = "test_multi_tu.out";

    FILE * f = fopen( out_file, "w" );
    assert( f != NULL );
    solog_config.stream = f;

    SOLOG( INFO, "main unit start" );
    log_from_sub_unit(); /* debug suppressed at default INFO level */

    solog_config.level = SOLOG_LVL_DEBUG;
    log_from_sub_unit(); /* both INFO and DEBUG printed */

    SOLOG( INFO, "main unit end" );

    fclose( f );

    char const * const expected =
        " INFO | main unit start\n"
        " INFO | message from sub unit\n"
        " INFO | message from sub unit\n"
        "DEBUG | debug from sub unit\n"
        " INFO | main unit end\n";

    if ( !check_file_contents( out_file, expected ) )
    {
        return 1;
    }

    remove( out_file );
    printf( "PASS: test_multi_tu\n" );
    return 0;
}
