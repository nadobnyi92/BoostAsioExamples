#include <iostream>
#include <string>
#include <boost/program_options.hpp>

#include "client.h"

namespace po = boost::program_options;

int main( int argc, const char** argv )
{
    uint32_t timeout = 0;
    po::options_description desc;
    desc.add_options()
        ("timeout", po::value< uint32_t >( &timeout )->default_value( 1000 ), "ping request interval in milliseconds");
    po::parse_command_line( argc, argv, desc );
    po::variables_map vm;
    po::store( po::parse_command_line( argc, argv, desc ), vm );
    po::notify( vm );

    Client client { timeout };

    std::string login;
    std::cout << "Input login: ";
    std::cin >> login;

    if( client.login( "127.0.0.1", 8001, login ) )
    {
        client.start();
    }
    else
    {
        std::cout << "failed login to client" << std::endl;
    }
    return 0;
}
