#include "client.h"

#include <iostream>
#include <chrono>
#include <thread>

Client::Client( uint32_t timeout )
    : mTimeout( timeout )
    , mSocket( mService ) {}

bool Client::login( const std::string& address, int port, const std::string& login)
{
    ip::tcp::endpoint ep( ip::address::from_string( address ), port );
    mSocket.connect( ep );
    mSocket.write_some( buffer( "login: " + login + '\n' ) );

    streambuf responce;
    read_until(mSocket, responce, '\n');
    std::string responce_str{ std::istreambuf_iterator<char>{ &responce }, 
                     std::istreambuf_iterator<char>{} };
    std::cout << "responce: " << responce_str << std::flush;

    return true;
}

void Client::start()
{
    while( true )
    {
        boost::system::error_code ec {};
        mSocket.write_some( buffer( "ping\n" ) );
        streambuf responce;
        read_until(mSocket, responce, '\n', ec );
        if( ec )
        {
            std::cout << "ping error: " << ec.message() << std::endl;
            break;
        }
        std::string responce_str{ std::istreambuf_iterator<char>{ &responce }, 
                         std::istreambuf_iterator<char>{} };
        std::cout << "responce: " << responce_str << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( mTimeout ) );
    }
}
