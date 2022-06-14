#include <boost/asio/completion_condition.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <iostream>
#include <iterator>
#include <memory>
#include <system_error>
#include <string_view>

#include "async_server.h"

ip::tcp::acceptor acceptor { AsyncServer::service, ip::tcp::endpoint { ip::tcp::v4(), 8001 } };

void handle_accept( AsyncServer::ptr client, const std::error_code& err )
{
    client->start();
    auto newClient = AsyncServer::create();
    acceptor.async_accept( newClient->sock(), [ newClient ]( const std::error_code& err ){
            handle_accept( newClient, err );
        } );
}

int main()
{
    auto newClient = AsyncServer::create();
    acceptor.async_accept( newClient->sock(), [ newClient ] ( const std::error_code& err ){
            handle_accept( newClient, err );
        } );
    AsyncServer::service.run();
}
