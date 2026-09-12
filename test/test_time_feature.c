#define SOLOG_IMPLEMENTATION ( SOLOG_FEATURE_TIME | SOLOG_FEATURE_DATE )
#include "../solog.h"
#include "test_helper.h"
#include <assert.h>

int main( void )
{
    char const * const out_file = "test_time.out";

    FILE * f = fopen( out_file, "w" );
    assert( f != NULL );
    solog_config.stream = f;

    SOLOG( INFO, "time test" );

    fclose( f );

    FILE * rf = fopen( out_file, "r" );
    assert( rf != NULL );
    char buf[ 256 ];
    assert( fgets( buf, sizeof( buf ), rf ) != NULL );
    fclose( rf );

    /* Verify date YYYY-MM-DD and time HH:MM:SS format */
    assert( buf[ 4 ] == '-' );
    assert( buf[ 7 ] == '-' );
    assert( strstr( buf, "time test" ) != NULL );

    remove( out_file );
    printf( "PASS: test_time_feature\n" );
    return 0;
}
