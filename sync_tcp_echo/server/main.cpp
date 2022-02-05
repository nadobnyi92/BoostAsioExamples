#include <iostream>

#include <boost/asio.hpp>

using namespace boost::asio;

io_service service;

int main()
{
    ip::tcp::acceptor acceptor { service, ip::tcp::endpoint{ ip::tcp::v4(), 8001} };
    while( true )
    {
        ip::tcp::socket sock{ service };
        acceptor.accept( sock );
        std::cout << "client connected" << std::endl;
        streambuf responce;
        read_until(sock, responce, '\n');
        std::string responce_str{ std::istreambuf_iterator<char>{ &responce }, 
                         std::istreambuf_iterator<char>{} };
        std::cout << "read: " << responce_str << std::endl;
        responce_str = "resp: " + responce_str;
        sock.write_some( buffer(responce_str) );
        sock.close();
    }

    return 0;
}