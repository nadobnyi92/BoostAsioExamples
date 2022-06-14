#pragma once

#include <iostream>

#include <system_error>
#include <boost/asio.hpp>
#include <memory>

using namespace boost::asio;


class AsyncClient : public std::enable_shared_from_this<AsyncClient>
{
    AsyncClient( const std::string &message );

    void start( ip::tcp::endpoint ep );
    void stop();

public:
    typedef std::shared_ptr<AsyncClient> ptr;

    static ptr start( ip::tcp::endpoint ep, const std::string& message );
    static io_service service;

    void do_read();
    void do_write( const std::string& msg );

    AsyncClient( const AsyncClient& ) = delete;
    AsyncClient& operator = ( const AsyncClient& ) = delete;

private:
    ip::tcp::socket mSocket;
    bool mStarted;
    std::string mMessage;
    streambuf mReadStreamBuf;
    char mWriteBuffer[1024];
};

