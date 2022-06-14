#include <iostream>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

using namespace boost::asio;

int main( int argc, char* argv[] )
{
    io_service service {};
    char buff[1024] = { 0 };

    ip::udp::socket sock{ service, ip::udp::endpoint{ip::udp::v4(), 8001} };
    while( true ) 
    {
        ip::udp::endpoint sender_ep{};
        int bytes = sock.receive_from( buffer( buff ), sender_ep );
        std::string msg( buff, bytes );
        sock.send_to( buffer( msg ), sender_ep );
    }
}

