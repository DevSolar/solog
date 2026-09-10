/* SoLog
 * Some Ordinary Logging
 *
 * CC0 1.0 Universal / Public Domain
 *
 * Usage:
 *
 * * Define SOLOG_IMPLEMENTATION in exactly one translation unit before
 *   including this header.
 *
 * * Basic functionality: SOLOG( <lvl>, <fmt>, ... )
 *   * <lvl> can be one of TRACE, DEBUG, INFO, WARN, ERR, FAIL
 *   * printf-style format string and argument list
 *   * single (and thus atomic) call to vfprintf()
 *   * current logging level can be set by solog_config.level = SOLOG_LVL_*
 *     (default: SOLOG_LVL_INFO)
 *   * output stream can be set by solog_config.stream = ...
 *     (default: stderr)
 *
 * * SOLOG_H can be used for version-checking; will be incremented in future
 *   versions.
 *
 * HISTORY:
 *
 *    1 -- Initial release, basic functionality
 *
 * FUTURE:
 *
 * * timestamping
 * * scoping (module-local log levels)
 * * ...
 */

/* ---------------------------------------------------------------------- */

#ifndef SOLOG_H
#define SOLOG_H 1

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Declarations for every translation unit */

typedef enum
{
    SOLOG_LVL_TRACE = 0,
    SOLOG_LVL_DEBUG,
    SOLOG_LVL_INFO,
    SOLOG_LVL_WARN,
    SOLOG_LVL_ERR,
    SOLOG_LVL_FAIL
} solog_level_t;

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#elif defined(_MSC_VER)
#pragma warning( push )
/* Padding */
#pragma warning( disable : 4820 )
#endif
typedef struct
{
    FILE * stream;
    solog_level_t level;
} solog_config_t;
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning( pop )
#endif

extern solog_config_t solog_config;

#define SOLOG( lvl, ... ) do { if ( SOLOG_LVL_##lvl >= solog_config.level ) { solog( SOLOG_LVL_##lvl, __VA_ARGS__ ); } else { (void)0; } } while ( 0 )

void solog( solog_level_t level, char const * fmt, ... );

#ifdef __cplusplus
}
#endif

#endif

/* ---------------------------------------------------------------------- */

#ifdef SOLOG_IMPLEMENTATION
#undef SOLOG_IMPLEMENTATION

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definitions for exactly one translation unit */

solog_config_t solog_config = {
    NULL,
    SOLOG_LVL_INFO
};

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
#elif defined(_MSC_VER)
#pragma warning( push )
/* Spectre mitigation for memory load */
#pragma warning( disable : 5045 )
#endif

void solog( solog_level_t level, char const * fmt, ... )
{
    static char const * const solog_hdrs[] = {
        "TRACE | ",
        "DEBUG | ",
        " INFO | ",
        " WARN | ",
        "  ERR | ",
        " FAIL | ",
        "ILLGL | "
    };

    char const * hdr = ( ( level >= SOLOG_LVL_TRACE ) && ( level <= SOLOG_LVL_FAIL ) ) ? solog_hdrs[ level ] : solog_hdrs[ SOLOG_LVL_FAIL + 1 ];
    FILE * stream = ( solog_config.stream == NULL ) ? stderr : solog_config.stream;
    char fmt_buf[1024];
    va_list ap;

    va_start( ap, fmt );
    snprintf( fmt_buf, sizeof( fmt_buf ), "%s%s\n", hdr, fmt );
    vfprintf( stream, fmt_buf, ap );
    va_end( ap );
}

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning( pop )
#endif

#ifdef __cplusplus
}
#endif

#endif
