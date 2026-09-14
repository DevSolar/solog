#include "../solog.h"
#include "test_helper.h"
#include "greatest.h"

TEST test_format( void )
{
    char const * const out_file = "test_format.out";

    solog_config.level = SOLOG_LVL_INFO;
    solog_config.features = SOLOG_FEATURE_LEVEL;

    FILE * f = fopen( out_file, "w" );
    ASSERT( f != NULL );
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
    solog_config.stream = NULL;

    /* Build expected content */
    size_t const expected_sz = 8 + 1 + 8 + 48 + 8 + 6 + 1500 + 10;
    char * expected = (char *)malloc( expected_sz );
    ASSERT( expected != NULL );

    snprintf( expected, expected_sz,
              " INFO | \n"
              " INFO | int=42, str=bar, hex=0xabc, float=3.14\n"
              " INFO | LONG: %s\n",
              long_str );

    int const res = check_file_contents( out_file, expected );
    free( expected );
    ASSERT( res );

    remove( out_file );
    PASS();
}

SUITE( format_suite )
{
    RUN_TEST( test_format );
}
