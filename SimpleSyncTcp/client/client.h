#pragma once

#include <string>

#include <boost/asio.hpp>

using namespace boost::asio;

class Client
{
public:
    Client( uint32_t timeout );
    bool login( const std::string& address, int port, const std::string& login);
    void start();

private:
    std::string mLogin;
    uint32_t mTimeout;

    io_service mService;
    ip::tcp::socket mSocket;
};

