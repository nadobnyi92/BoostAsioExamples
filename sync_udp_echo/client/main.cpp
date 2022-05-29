#include <iostream>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

using namespace boost::asio;

io_service service;
ip::udp::endpoint ep { ip::address::from_string( "127.0.0.1" ), 8001 };

void sync_echo( const std::string& msg )
{
    ip::udp::socket sock { service, ip::udp::endpoint { ip::udp::v4(), 0 } };
    sock.send_to( buffer( msg ), ep );
    char buff[1024] = { 0 };
    ip::udp::endpoint sender_ep {};
    int bytes = sock.receive_from( buffer( buff ), sender_ep );
    std::string res ( buff, bytes );
    std::cout << "server recive: " << res << std::endl;
    sock.close();
}

int main() 
{
    std::string input;
    while( input != "exit" )
    {
        std::cout << "print message: ";
        std::cin >> input;
        sync_echo( input );
    }
}

