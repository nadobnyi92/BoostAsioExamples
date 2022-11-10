#include <iostream>
#include <thread>
#include <chrono>

#include "client.h"

int main()
{
    ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001 );

    std::string input;
    auto ptr = AsyncClient::create( ep );
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    AsyncClient::service.run();
    while ( ptr->isRun() )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
    
    return 0;
}
