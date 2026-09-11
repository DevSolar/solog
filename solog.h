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

/* COMPILE TIME CONFIGURATION */
/* See comment at top / Readme.md on usage */

/* Display the message level in the output */
#define SOLOG_FEATURE_LEVEL (1<<0)


/* RUNTIME CONFIGURATION */
/* Struct gets padded; warning suppressed */
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#elif defined(_MSC_VER)
#pragma warning( push )
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

/* LOG LEVELS */
/* SOLOG_LVL_* automatically prefixed by SOLOG() macro */
typedef enum
{
    SOLOG_LVL_TRACE = 0,
    SOLOG_LVL_DEBUG,
    SOLOG_LVL_INFO,
    SOLOG_LVL_WARN,
    SOLOG_LVL_ERR,
    SOLOG_LVL_FAIL
} solog_level_t;


/* SOLOG MACRO */
/* Feel free to shorten the macro itself to LOG, the rest should remain
 * prefixed / namespaced.
 */
#define SOLOG( lvl, ... ) do { if ( SOLOG_LVL_##lvl >= solog_config.level ) { solog( SOLOG_LVL_##lvl, __VA_ARGS__ ); } else { (void)0; } } while ( 0 )


/* The worker function called by the macro */
void solog( solog_level_t level, char const * fmt, ... );

#ifdef __cplusplus
}
#endif

#endif

/* ---------------------------------------------------------------------- */
/* End of the header part                                                 */
/* ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
/* Begin of the implementation part; see usage at top of file / Readme.md */
/* ---------------------------------------------------------------------- */

/* COMPILE TIME CONFIGURATION, DEFAULT == ULTRALIGHT */
#ifdef SOLOG_IMPLEMENTATION
#if SOLOG_IMPLEMENTATION + 0 == 0
#undef SOLOG_IMPLEMENTATION
#define SOLOG_IMPLEMENTATION 0
#endif

#include <stdarg.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RUN TIME CONFIGURATION, INIT */
solog_config_t solog_config = {
    NULL,
    SOLOG_LVL_INFO,
    SOLOG_IMPLEMENTATION
};

/* Suppressed warnings:
 * -Wformat-nonliteral
 *   We are passing the format string by variable; this is a potential
 *   security risk if that variable is filled by user input (which it is
 *   not, in our case).
 * -Wunsafe-buffer-usage
 *   A C++-based warning, triggered whenever pointer-and-index access is
 *   being done. Which is bad style in C++, but unavoidable in C.
 * C5045
 *   Purely informational warning that /Qspectre would add mitigation
 *   code. (Triggered by range-checking an index by comparison.)
 */
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
#elif defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 5045 )
#endif

/* WORKER FUNCTION */
void solog( solog_level_t level, char const * fmt, ... )
{
    /* Step 1: Calculate the max. space required to hold the header plus
     * the user's format string ('fmt_cmpl').
     * This depends on the features selected, plus the length of the user's
     * format string, plus a newline.
    */

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

    /* Step 2: Allocate header memory */

    char * fmt_cmpl = (char *)solog_alloca( SOLOG_SZ_FMT + strlen( fmt ) );
    char * fptr = fmt_cmpl;

    /* Step 3: Range-check the log level */

#if SOLOG_IMPLEMENTATION & SOLOG_FEATURE_LEVEL
    int const lvl = ( ( level >= SOLOG_LVL_TRACE ) && ( level <= SOLOG_LVL_FAIL ) ) ? level : SOLOG_LVL_FAIL + 1;
#else
    (void)level;
#endif

    /* Step 4: Final declarations */

    FILE * stream = ( solog_config.stream == NULL ) ? stderr : solog_config.stream;
    va_list ap;

    /* Step 5: Assemble the header */

#if SOLOG_IMPLEMENTATION & SOLOG_FEATURE_LEVEL
    if ( solog_config.features & SOLOG_FEATURE_LEVEL )
    {
        fptr += sprintf( fptr, "%.*s | ", SOLOG_SZ_LEVEL - 3, solog_hdrs[ lvl ] );
    }
#endif

    /* Step 6: Append the user's format string to the header */

    sprintf( fptr, "%s\n", fmt );

    /* Step 7: Print the log message using our header-plus-format-string. */

    va_start( ap, fmt );
    vfprintf( stream, fmt_cmpl, ap );
    va_end( ap );

    /* Step 8: Clean up. */

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
