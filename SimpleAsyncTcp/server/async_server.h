#pragma once

#include <memory>

#include <boost/asio.hpp>

#include "client_storage.h"

using namespace boost::asio;

class AsyncServer : public std::enable_shared_from_this<AsyncServer>
{
public:
    typedef std::shared_ptr<AsyncServer> ptr;
    typedef std::shared_ptr<ClientStorage> StoragePtr;

    AsyncServer( const AsyncServer& ) = delete;
    AsyncServer& operator=( const AsyncServer& ) = delete;

    static ptr create( StoragePtr storage );
    static io_service service;
    static ip::tcp::acceptor acceptor;

    ip::tcp::socket& sock();
    StoragePtr storage() const;

    void start();
    void stop();

    void do_read();
    void do_write( const std::string& msg );

    std::string process_message( const std::string& msg );

private:
    AsyncServer( StoragePtr storage ); 
    std::string updateClients();

private:
    StoragePtr mStorage;
    std::vector< std::string > mLocalClients;
    ip::tcp::socket mSocket;
    bool mStarted;
    streambuf mReadStreamBuf;
    char mWriteBuffer[ 1024 ];
};


