#include <iostream>

#include <boost/asio.hpp>
#include <boost/program_options.hpp>

#include "client_processor.h"
#include "client_storage.h"

using namespace boost::asio;
namespace po = boost::program_options;

int main( int argc, char** argv )
{
    uint32_t timeout = 0;

    po::options_description desc;
    desc.add_options()
        ( "timeout", po::value< uint32_t >( &timeout )->default_value( 1000 ), "maximum ping request interval from user in milliseconmds" );
    po::variables_map vm;
    po::store( po::parse_command_line( argc, argv, desc ), vm );
    po::notify( vm );

    std::cout << "timeout: " << timeout << std::endl;
    ClientStorage storage { timeout };
    ip::tcp::acceptor acceptor { ClientProcessor::service, ip::tcp::endpoint{ ip::tcp::v4(), 8001} };
    std::thread threadAccept { [ &storage, &acceptor ]()
        {
            while( true )
            { 
                storage.newClient( acceptor );
            }
        }
    };
    std::thread threadProcess { [ &storage ]()
        {
            while( true )
            {
                storage.processRequests();
                storage.removeOvertime();
            }
        }
    };
    threadProcess.join();
    threadAccept.join();
    return 0;
}

