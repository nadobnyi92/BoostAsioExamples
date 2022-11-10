#pragma once

#include <string>
#include <vector>
#include <mutex>

class ClientStorage
{
public:
    ClientStorage();

    void addClient( const std::string& name );
    void removeClient( const std::string& name );
    std::vector< std::string > getClients() const;

private:
    std::vector< std::string > mClients;
    mutable std::mutex mMutext;
};

