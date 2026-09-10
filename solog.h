/* SoLog
 * Some Ordinary Logging
 *
 * CC0 1.0 Universal / Public Domain
 *
 * Usage:
 *
 * * Define SOLOG_IMPLEMENTATION in exactly one translation unit before
 *   including this header.
 * * By setting SOLOG_IMPLEMENTATION to an OR-ed selection of features
 *   (SOLOG_FEATURE_*) you activate those features at compile time; by
 *   AND-ing them out of solog_config.features you can deactivate them
 *   at runtime.
 *   * SOLOG_FEATURE_LEVEL: Show the level of the message in the output.
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
 *    2 -- Making level output optional, preparing for extension
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
#define SOLOG_H 2

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

/* Display the message level in the output */
#define SOLOG_FEATURE_LEVEL (1<<0)

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
    unsigned features;
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
#if SOLOG_IMPLEMENTATION + 0 == 0
/* No features selected; keep ultra-light */
#undef SOLOG_IMPLEMENTATION
#define SOLOG_IMPLEMENTATION 0
#endif

/* Features selected; provide solog_alloca() */
#if defined(_WIN32)
#include <malloc.h>
#define solog_alloca(sz) _malloca(sz)
#define solog_freea(p)    _freea(p)
#elif defined(__GNUC__) || defined(__clang__)
#define solog_alloca(sz) __builtin_alloca(sz)
#define solog_freea(p)    ((void)(p))
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__APPLE__)
#include <stdlib.h>
#define solog_alloca(sz) alloca(sz)
#define solog_freea(p)    ((void)(p))
#else
#include <alloca.h>
#define solog_alloca(sz) alloca(sz)
#define solog_freea(p)    ((void)(p))
#endif

#include <stdarg.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definitions for exactly one translation unit */

solog_config_t solog_config = {
    NULL,
    SOLOG_LVL_INFO,
    SOLOG_IMPLEMENTATION
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
#if SOLOG_IMPLEMENTATION & SOLOG_FEATURE_LEVEL
/* max. level length + ' | ' */
#define SOLOG_SZ_LEVEL 8
    static char const * const solog_hdrs[] = {
        "TRACE",
        "DEBUG",
        " INFO",
        " WARN",
        "  ERR",
        " FAIL",
        "ILLGL"
    };
#else
#define SOLOG_SZ_LEVEL 0
#endif

#define SOLOG_SZ_FMT ( \
        SOLOG_SZ_LEVEL \
        + 2 )

    char * fmt_cmpl = (char *)solog_alloca( SOLOG_SZ_FMT + strlen( fmt ) );
    char * fptr = fmt_cmpl;

#if SOLOG_IMPLEMENTATION & SOLOG_FEATURE_LEVEL
    int const lvl = ( ( level >= SOLOG_LVL_TRACE ) && ( level <= SOLOG_LVL_FAIL ) ) ? level : SOLOG_LVL_FAIL + 1;
#else
    (void)level;
#endif
    FILE * stream = ( solog_config.stream == NULL ) ? stderr : solog_config.stream;
    va_list ap;

#if SOLOG_IMPLEMENTATION & SOLOG_FEATURE_LEVEL
    if ( solog_config.features & SOLOG_FEATURE_LEVEL )
    {
        fptr += sprintf( fptr, "%.*s | ", SOLOG_SZ_LEVEL - 3, solog_hdrs[ lvl ] );
    }
#endif

    sprintf( fptr, "%s\n", fmt );

    va_start( ap, fmt );
    vfprintf( stream, fmt_cmpl, ap );
    va_end( ap );
    solog_freea( fmt_cmpl );
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
