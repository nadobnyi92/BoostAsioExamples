#pragma once

#include <boost/asio.hpp>

using namespace boost::asio;

class AsyncServer : public std::enable_shared_from_this<AsyncServer>
{
    AsyncServer(); 

public:
    typedef std::shared_ptr<AsyncServer> ptr;

    AsyncServer( const AsyncServer& ) = delete;
    AsyncServer& operator=( const AsyncServer& ) = delete;

    static ptr create();
    static io_service service;

    ip::tcp::socket& sock();

    void start();
    void stop();

    void do_read();
    void do_write( const std::string& msg );

private:
    ip::tcp::socket mSocket;
    bool mStarted;
    streambuf mReadStreamBuf;
    char mWriteBuffer[ 1024 ];
};

