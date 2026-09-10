#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    if ( strcmp( buf, expected ) != 0 )
    {
        fprintf( stderr, "FAIL: Output mismatch in '%s'\n", filename );
        fprintf( stderr, "=== Expected ===\n%s\n", expected );
        fprintf( stderr, "=== Actual ===\n%s\n", buf );
        free( buf );
        return 0;
    }

    free( buf );
    return 1;
}

#endif
