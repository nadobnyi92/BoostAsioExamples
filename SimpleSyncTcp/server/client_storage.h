#pragma once

#include <string>
#include <vector>
#include <set>
#include <mutex>

#include <boost/asio.hpp>

using namespace boost::asio;

class ClientProcessor;

class ClientStorage
{
public:
    ClientStorage( uint32_t timeout );

    void newClient( ip::tcp::acceptor& acceptor );
    void processRequests();
    void removeOvertime();

    std::vector< std::string > getClientsNames() const;

private:
    uint32_t mTimeout; 
    std::vector< std::unique_ptr< ClientProcessor > > mClients;
    mutable std::mutex mMutext;
};

