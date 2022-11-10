#include <boost/asio/completion_condition.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <iostream>
#include <iterator>
#include <memory>
#include <system_error>
#include <string_view>

#include "async_server.h"
#include "client_storage.h"

void handle_accept( AsyncServer::ptr client, const std::error_code& err )
{
    client->start();
    auto newClient = AsyncServer::create( client->storage() );
    AsyncServer::acceptor.async_accept( newClient->sock(), [ newClient ]( const std::error_code& err ){
            handle_accept( newClient, err );
        } );
}

int main()
{
    auto storage = std::make_shared<ClientStorage>();
    auto newClient = AsyncServer::create( storage );
    AsyncServer::acceptor.async_accept( newClient->sock(), [ newClient ] ( const std::error_code& err ){
            handle_accept( newClient, err );
        } );
    AsyncServer::service.run();
}
