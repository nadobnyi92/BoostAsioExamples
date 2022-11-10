#include "client_storage.h"

#include <algorithm>

ClientStorage::ClientStorage() {}

void ClientStorage::addClient( const std::string& name )
{
    std::lock_guard< std::mutex > lk{ mMutext};
    mClients.push_back( name );
}

void ClientStorage::removeClient( const std::string& name )
{
    std::lock_guard< std::mutex > lk{ mMutext};
    mClients.erase( std::remove( mClients.begin(), mClients.end(), name ),
        mClients.end() );
}

std::vector< std::string > ClientStorage::getClients() const
{
    return mClients;
}
