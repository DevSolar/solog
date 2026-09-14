#include "../solog.h"
#include "test_helper.h"
#include "greatest.h"

TEST test_location_feature( void )
{
    char const * const out_file = "test_location.out";

    solog_config.level = SOLOG_LVL_INFO;
    solog_config.features = SOLOG_FEATURE_FILE | SOLOG_FEATURE_FUNC | SOLOG_FEATURE_LINE;

    FILE * f = fopen( out_file, "w" );
    ASSERT( f != NULL );
    solog_config.stream = f;

    SOLOG( INFO, "loc test" );

    fclose( f );
    solog_config.stream = NULL;

    /* Expect formatted string containing file name, func name, line number */
    FILE * rf = fopen( out_file, "r" );
    ASSERT( rf != NULL );
    char buf[ 256 ];
    ASSERT( fgets( buf, sizeof( buf ), rf ) != NULL );
    fclose( rf );

    normalize_crlf( buf );

    ASSERT( strstr( buf, "test_location" ) != NULL );
    ASSERT( strstr( buf, "loc test" ) != NULL );

    remove( out_file );
    PASS();
}

SUITE( location_feature_suite )
{
    RUN_TEST( test_location_feature );
}
