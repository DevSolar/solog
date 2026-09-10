#define SOLOG_IMPLEMENTATION SOLOG_FEATURE_LEVEL
#include "../solog.h"
#include "test_helper.h"
#include <cassert>
#include <string>

namespace test_space {
    class LoggerUser {
    public:
        void doLog( const std::string & msg ) {
            SOLOG( INFO, "C++ class method: %s", msg.c_str() );
        }
    };
}

int main( void )
{
    char const * const out_file = "test_cpp.out";

    FILE * f = fopen( out_file, "w" );
    assert( f != NULL );
    solog_config.stream = f;

    SOLOG( INFO, "C++ test start" );

    test_space::LoggerUser user;
    user.doLog( "hello from std::string" );

    SOLOG( WARN, "C++ test warning %d", 42 );

    fclose( f );

    char const * const expected =
        " INFO | C++ test start\n"
        " INFO | C++ class method: hello from std::string\n"
        " WARN | C++ test warning 42\n";

    if ( !check_file_contents( out_file, expected ) )
    {
        return 1;
    }

    remove( out_file );
    printf( "PASS: test_cpp\n" );
    return 0;
}
