#define SOLOG_IMPLEMENTATION SOLOG_FEATURE_LEVEL
#include "../solog.h"
#include "test_helper.h"
#include <assert.h>

int main( void )
{
    char const * const out_file = "test_format.out";

    FILE * f = fopen( out_file, "w" );
    assert( f != NULL );
    solog_config.stream = f;

    /* 1. Empty string format */
    SOLOG( INFO, "" );

    /* 2. Multiple format specifiers */
    SOLOG( INFO, "int=%d, str=%s, hex=0x%x, float=%.2f", 42, "bar", 0xabc, 3.14 );

    /* 3. Long format string (> 1024 chars to verify dynamic stack allocation) */
    char long_str[ 1500 ];
    memset( long_str, 'A', sizeof( long_str ) - 1 );
    long_str[ sizeof( long_str ) - 1 ] = '\0';
    SOLOG( INFO, "LONG: %s", long_str );

    fclose( f );

    /* Build expected content */
    size_t const expected_sz = 8 + 1 + 8 + 48 + 8 + 6 + 1500 + 10;
    char * expected = (char *)malloc( expected_sz );
    assert( expected != NULL );

    snprintf( expected, expected_sz,
              " INFO | \n"
              " INFO | int=42, str=bar, hex=0xabc, float=3.14\n"
              " INFO | LONG: %s\n",
              long_str );

    if ( !check_file_contents( out_file, expected ) )
    {
        free( expected );
        return 1;
    }

    free( expected );
    remove( out_file );
    printf( "PASS: test_format\n" );
    return 0;
}
