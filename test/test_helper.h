#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void normalize_crlf( char * str )
{
    if ( !str ) return;
    char * src = str;
    char * dst = str;
    while ( *src != '\0' )
    {
        if ( *src != '\r' )
        {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

static inline int check_file_contents( const char * filename, const char * expected )
{
    FILE * f = fopen( filename, "rb" );
    if ( !f )
    {
        fprintf( stderr, "FAIL: Could not open output file '%s'\n", filename );
        return 0;
    }

    if ( fseek( f, 0, SEEK_END ) != 0 )
    {
        fclose( f );
        fprintf( stderr, "FAIL: fseek failed on '%s'\n", filename );
        return 0;
    }
    long const sz = ftell( f );
    if ( sz < 0 )
    {
        fclose( f );
        fprintf( stderr, "FAIL: ftell failed on '%s'\n", filename );
        return 0;
    }
    rewind( f );

    char * buf = (char *)malloc( (size_t)sz + 1 );
    if ( !buf )
    {
        fclose( f );
        fprintf( stderr, "FAIL: Memory allocation failed for '%s'\n", filename );
        return 0;
    }

    size_t const read_bytes = fread( buf, 1, (size_t)sz, f );
    buf[ read_bytes ] = '\0';
    fclose( f );

    normalize_crlf( buf );

    char * expected_norm = (char *)malloc( strlen( expected ) + 1 );
    if ( expected_norm )
    {
        strcpy( expected_norm, expected );
        normalize_crlf( expected_norm );
    }
    char const * const exp = expected_norm ? expected_norm : expected;

    int const match = ( strcmp( buf, exp ) == 0 );
    if ( !match )
    {
        fprintf( stderr, "FAIL: Output mismatch in '%s'\n", filename );
        fprintf( stderr, "=== Expected ===\n%s\n", exp );
        fprintf( stderr, "=== Actual ===\n%s\n", buf );
    }

    free( expected_norm );
    free( buf );
    return match;
}

#endif
