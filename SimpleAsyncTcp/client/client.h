#pragma once

#include <iostream>

#include <system_error>
#include <boost/asio.hpp>
#include <memory>

using namespace boost::asio;


class AsyncClient : public std::enable_shared_from_this<AsyncClient>
{
    AsyncClient();

    void start( ip::tcp::endpoint ep );
    void stop();

public:
    typedef std::shared_ptr<AsyncClient> ptr;

    static ptr create( ip::tcp::endpoint ep );
    static io_service service;

    void do_read();
    void do_write( const std::string& msg );
    bool isRun() const;

    AsyncClient( const AsyncClient& ) = delete;
    AsyncClient& operator = ( const AsyncClient& ) = delete;

private:
    ip::tcp::socket mSocket;
    bool mStarted;
    streambuf mReadStreamBuf;
    char mWriteBuffer[1024];
};

