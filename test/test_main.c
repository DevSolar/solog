#define SOLOG_IMPLEMENTATION SOLOG_FEATURE_ALL
#include "../solog.h"
#include "greatest.h"

SUITE_EXTERN( basic_suite );
SUITE_EXTERN( level_feature_suite );
SUITE_EXTERN( color_feature_suite );
SUITE_EXTERN( multi_tu_suite );
SUITE_EXTERN( cpp_suite );
SUITE_EXTERN( format_suite );
SUITE_EXTERN( custom_alloc_suite );
SUITE_EXTERN( location_feature_suite );
SUITE_EXTERN( time_feature_suite );

GREATEST_MAIN_DEFS();

int main( int argc, char **argv )
{
    GREATEST_MAIN_BEGIN();

    RUN_SUITE( basic_suite );
    RUN_SUITE( level_feature_suite );
    RUN_SUITE( color_feature_suite );
    RUN_SUITE( multi_tu_suite );
    RUN_SUITE( cpp_suite );
    RUN_SUITE( format_suite );
    RUN_SUITE( custom_alloc_suite );
    RUN_SUITE( location_feature_suite );
    RUN_SUITE( time_feature_suite );

    GREATEST_MAIN_END();
}
