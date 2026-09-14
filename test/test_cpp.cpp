#include "../solog.h"
#include "test_helper.h"
#include "greatest.h"
#include <string>

namespace test_space {
    class LoggerUser {
    public:
        void doLog( const std::string & msg ) {
            SOLOG( INFO, "C++ class method: %s", msg.c_str() );
        }
    };
}

TEST test_cpp_integration( void )
{
    char const * const out_file = "test_cpp.out";

    solog_config.level = SOLOG_LVL_INFO;
    solog_config.features = SOLOG_FEATURE_LEVEL;

    FILE * f = fopen( out_file, "w" );
    ASSERT( f != NULL );
    solog_config.stream = f;

    SOLOG( INFO, "C++ test start" );

    test_space::LoggerUser user;
    user.doLog( "hello from std::string" );

    SOLOG( WARN, "C++ test warning %d", 42 );

    /* Test solog_malloca / solog_freea in C++ */
    char * cpp_small = (char *)solog_malloca( 32 );
    ASSERT( cpp_small != NULL );
    cpp_small[ 0 ] = 'C';
    solog_freea( cpp_small );

    char * cpp_large = (char *)solog_malloca( 2048 );
    ASSERT( cpp_large != NULL );
    cpp_large[ 0 ] = 'P';
    solog_freea( cpp_large );

    fclose( f );
    solog_config.stream = NULL;

    char const * const expected =
        " INFO | C++ test start\n"
        " INFO | C++ class method: hello from std::string\n"
        " WARN | C++ test warning 42\n";

    ASSERT( check_file_contents( out_file, expected ) );
    remove( out_file );
    PASS();
}

extern "C" {
SUITE( cpp_suite )
{
    RUN_TEST( test_cpp_integration );
}
}
