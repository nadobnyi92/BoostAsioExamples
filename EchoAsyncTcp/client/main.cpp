#include <iostream>
#include <thread>
#include <chrono>

#include "async_client.h"

int main()
{
    ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001 );

    std::string input;
    while( input != "exit" )
    {
        std::cout << "print message: ";
        std::cin >> input;
        AsyncClient::start( ep, input );
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
    AsyncClient::service.run();
    return 0;
}
