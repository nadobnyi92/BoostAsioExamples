#include <iostream>

#include <boost/asio.hpp>

using namespace boost::asio;

int main()
{
    io_service service;
    ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001 );

    std::string input;
    while( input != "exit" )
    {
        std::cout << "print message: ";
        std::cin >> input;
        input += '\n';
        ip::tcp::socket sock { service };
        sock.connect( ep );
        sock.write_some( buffer(input) );
        std::cout << "message sending" << std::endl;
        streambuf responce;
        read_until(sock, responce, '\n');
        std::string responce_str{ std::istreambuf_iterator<char>{ &responce }, 
                         std::istreambuf_iterator<char>{} };
        std::cout << "responce: " << responce_str << std::endl;
    }
    return 0;
}