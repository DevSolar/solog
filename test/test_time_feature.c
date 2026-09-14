#include "../solog.h"
#include "test_helper.h"
#include "greatest.h"
#include <ctype.h>

static int read_first_line( char const * filename, char * buf, size_t maxlen )
{
    FILE * rf = fopen( filename, "r" );
    if ( !rf ) return 0;
    if ( fgets( buf, (int)maxlen, rf ) == NULL )
    {
        fclose( rf );
        return 0;
    }
    fclose( rf );
    normalize_crlf( buf );
    return 1;
}

TEST test_date_and_time( void )
{
    char const * const out_file = "test_time.out";
    char buf[ 256 ];

    solog_config.level = SOLOG_LVL_INFO;

    /* 1. Both DATE and TIME enabled */
    FILE * f = fopen( out_file, "w" );
    ASSERT( f != NULL );
    solog_config.stream = f;
    solog_config.features = SOLOG_FEATURE_TIME | SOLOG_FEATURE_DATE;
    SOLOG( INFO, "date and time test" );
    fclose( f );

    ASSERT( read_first_line( out_file, buf, sizeof( buf ) ) );
    /* Format: YYYY-MM-DD HH:MM:SS | ... */
    ASSERT( isdigit( (unsigned char)buf[ 0 ] ) && isdigit( (unsigned char)buf[ 1 ] ) );
    ASSERT_EQ( '-', buf[ 4 ] );
    ASSERT_EQ( '-', buf[ 7 ] );
    ASSERT( strstr( buf, "date and time test" ) != NULL );
    ASSERT( strstr( buf, " | " ) != NULL );

    /* 2. TIME feature only at runtime */
    f = fopen( out_file, "w" );
    ASSERT( f != NULL );
    solog_config.stream = f;
    solog_config.features = SOLOG_FEATURE_TIME;
    SOLOG( INFO, "time only test" );
    fclose( f );

    ASSERT( read_first_line( out_file, buf, sizeof( buf ) ) );
    /* Format: HH:MM:SS | ... (No date YYYY-MM-DD prefix) */
    ASSERT_EQ( ':', buf[ 2 ] );
    ASSERT_EQ( ':', buf[ 5 ] );
    ASSERT( buf[ 4 ] != '-' );
    ASSERT( strstr( buf, "time only test" ) != NULL );

    /* 3. DATE feature only at runtime (should enable TIME automatically) */
    f = fopen( out_file, "w" );
    ASSERT( f != NULL );
    solog_config.stream = f;
    solog_config.features = SOLOG_FEATURE_DATE;
    SOLOG( INFO, "date only test" );
    fclose( f );

    ASSERT( read_first_line( out_file, buf, sizeof( buf ) ) );
    /* Format: YYYY-MM-DD HH:MM:SS | ... */
    ASSERT_EQ( '-', buf[ 4 ] );
    ASSERT_EQ( '-', buf[ 7 ] );
    ASSERT( strstr( buf, "date only test" ) != NULL );

    solog_config.stream = NULL;
    remove( out_file );
    PASS();
}

SUITE( time_feature_suite )
{
    RUN_TEST( test_date_and_time );
}
