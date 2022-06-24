#pragma once

#include <vector>
#include <set>
#include <map>
#include <mutex>

#include <boost/asio.hpp>

class ClientStorage;

using namespace boost::asio;

class ClientProcessor
{
public:
    static io_service service;

    ClientProcessor( ClientStorage& storage, uint32_t timeout );

    std::string clientName() const;

    bool login();
    void checkRequest();
    bool isOverTime();

    ip::tcp::socket& socket();

private:
    std::string clientChangeList();

private:
    uint32_t mTimeout;
    std::string mName;

    ip::tcp::socket mSocket;
    std::chrono::system_clock::time_point mActiveTime;

    ClientStorage& mClientStorage;
    std::vector <std::string> mClientList;
};

