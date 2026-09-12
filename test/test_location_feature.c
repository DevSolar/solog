#define SOLOG_IMPLEMENTATION ( SOLOG_FEATURE_FILE | SOLOG_FEATURE_FUNC | SOLOG_FEATURE_LINE )
#include "../solog.h"
#include "test_helper.h"
#include <assert.h>

int main( void )
{
    char const * const out_file = "test_location.out";

    FILE * f = fopen( out_file, "w" );
    assert( f != NULL );
    solog_config.stream = f;

    SOLOG( INFO, "loc test" );

    fclose( f );

    /* Expect formatted string containing file name, func name, line number */
    FILE * rf = fopen( out_file, "r" );
    assert( rf != NULL );
    char buf[ 256 ];
    assert( fgets( buf, sizeof( buf ), rf ) != NULL );
    fclose( rf );

    assert( strstr( buf, "test_location_f" ) != NULL || strstr( buf, "test_location" ) != NULL );
    assert( strstr( buf, "main" ) != NULL );
    assert( strstr( buf, "loc test" ) != NULL );

    remove( out_file );
    printf( "PASS: test_location_feature\n" );
    return 0;
}
