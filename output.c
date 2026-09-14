#define SOLOG_IMPLEMENTATION SOLOG_FEATURE_ALL
#include "solog.h"

int main( void )
{
    solog_config.stream = stdout;
    solog_config.level = SOLOG_LVL_TRACE;

    /* Basic output */
    printf( "Basic output:\n\n    " );
    solog_config.features = 0;
    SOLOG( INFO, "Just the log message" );

    /* SOLOG_FEATURE_LEVEL */
    printf( "\nWith SOLOG_FEATURE_LEVEL:\n\n    " );
    solog_config.features = SOLOG_FEATURE_LEVEL;
    SOLOG( INFO, "Log level prefixed to the message" );

    /* SOLOG_FEATURE_COLOR */
    printf( "\nWith SOLOG_FEATURE_COLOR:\n\n    " );
    solog_config.features = SOLOG_FEATURE_LEVEL | SOLOG_FEATURE_COLOR;
    SOLOG( TRACE, "Trace message" );
    printf( "    " );
    SOLOG( DEBUG, "Debug message" );
    printf( "    " );
    SOLOG( INFO,  "Info message" );
    printf( "    " );
    SOLOG( WARN,  "Warning message" );
    printf( "    " );
    SOLOG( ERR,   "Error message" );
    printf( "    " );
    SOLOG( FAIL,  "Failure message" );

    /* SOLOG_FEATURE_FILE */
    printf( "\nWith SOLOG_FEATURE_FILE | SOLOG_FEATURE_LINE:\n\n    " );
    solog_config.features = SOLOG_FEATURE_LEVEL | SOLOG_FEATURE_FILE | SOLOG_FEATURE_LINE;
    SOLOG( INFO, "File / line number added to message." );

    /* SOLOG_FEATURE_FUNC */
    printf( "\nWith SOLOG_FEATURE_FILE | SOLOG_FEATURE_FUNC:\n\n    " );
    solog_config.features = SOLOG_FEATURE_LEVEL | SOLOG_FEATURE_FILE | SOLOG_FEATURE_FUNC;
    SOLOG( INFO, "If you prefer file and function name" );

    /* SOLOG_FEATURE_LINE */
    printf( "\nWith SOLOG_FEATURE_FILE | SOLOG_FEATURE_FUNC | SOLOG_FEATURE_LINE:\n\n    " );
    solog_config.features = SOLOG_FEATURE_LEVEL | SOLOG_FEATURE_FILE | SOLOG_FEATURE_FUNC | SOLOG_FEATURE_LINE;
    SOLOG( INFO, "All three together?" );

    /* SOLOG_FEATURE_TIME */
    printf( "\nWith SOLOG_FEATURE_TIME:\n\n    " );
    solog_config.features = SOLOG_FEATURE_LEVEL | SOLOG_FEATURE_TIME;
    SOLOG( INFO, "Timestamped logging, without..." );

    /* SOLOG_FEATURE_DATE */
    printf( "\nWith SOLOG_FEATURE_DATE:\n\n    " );
    solog_config.features = ( SOLOG_FEATURE_LEVEL | SOLOG_FEATURE_DATE | SOLOG_FEATURE_TIME );
    SOLOG( INFO, "...and with date." );

    /* SOLOG_FEATURE_ALL */
    printf( "\nWith all features enabled:\n\n    " );
    solog_config.features = SOLOG_FEATURE_ALL;
    SOLOG( INFO, "All the features." );

    return 0;
}
