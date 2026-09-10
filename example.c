/*#define SOLOG_IMPLEMENTATION SOLOG_FEATURE_LEVEL*/
#define SOLOG_IMPLEMENTATION
#include "solog.h"

static int hello( void )
{
    puts( "Hello." );
    return 0;
}

int main( void )
{
    SOLOG( INFO, "Entering '%s'. This logs to stderr (default).", __func__ );

    solog_config.stream = stdout;

    SOLOG( INFO, "This logs to stdout." );

    solog_config.level = SOLOG_LVL_WARN;

    SOLOG( INFO, "This does not log at all as it is below the current warning level."
           "It also does not say 'Hello' as arguments are only evaluated "
           "if there is an actual message being logged.", hello() );

    return 0;
}
