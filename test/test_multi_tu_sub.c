#include "../solog.h"
#include "test_multi_tu_sub.h"

void log_from_sub_unit( void )
{
    SOLOG( INFO, "message from sub unit" );
    SOLOG( DEBUG, "debug from sub unit" );
}
