#include "client_storage.h"
#include "client_processor.h"


ClientStorage::ClientStorage( uint32_t timeout )
    : mTimeout( timeout ) {}

void ClientStorage::processRequests()
{
    std::lock_guard< std::mutex > lk{ mMutext};
    for( auto& client: mClients)
        client->checkRequest();
}

void ClientStorage::removeOvertime()
{
    std::lock_guard< std::mutex > lk{ mMutext};
    mClients.erase( std::remove_if( mClients.begin(), mClients.end(),
        []( auto& client ) { return client->isOverTime(); } ), mClients.end() );
}

void ClientStorage::newClient( ip::tcp::acceptor& acceptor )
{
    ClientProcessor* client = new ClientProcessor( *this, mTimeout );
    acceptor.accept( client->socket() );
    if( client->login() )
    {
        std::lock_guard< std::mutex > lk{ mMutext};
        mClients.emplace_back( std::unique_ptr< ClientProcessor >( client ) );
    }
    else
    {
        delete client;
    }
}

std::vector< std::string > ClientStorage::getClientsNames() const
{
    std::vector< std::string > names( mClients.size() );
    for( auto& client: mClients )
        names.push_back( client->clientName() );
    return names;
}
