#include "async_server.h"

#include <iostream>
#include <numeric>
#include <regex>

io_service AsyncServer::service {};
ip::tcp::acceptor AsyncServer::acceptor { AsyncServer::service, ip::tcp::endpoint { ip::tcp::v4(), 8001 } };

AsyncServer::AsyncServer( StoragePtr storage )
    : mStorage( storage )
    , mSocket( service )
    , mStarted( false ) {}

AsyncServer::ptr AsyncServer::create( StoragePtr storage )
{
    return ptr { new AsyncServer{ storage } };
}

ip::tcp::socket& AsyncServer::sock()
{
    return mSocket;
}

AsyncServer::StoragePtr AsyncServer::storage() const
{
    return mStorage;
}

void AsyncServer::start()
{
    mStarted = true;
    do_read();
}

void AsyncServer::stop()
{
    if( !mStarted ) return;
    mStarted = false;
    mSocket.close();
}

void AsyncServer::do_read()
{
    async_read_until( mSocket, mReadStreamBuf, '\n',
        [ ptr = shared_from_this() ]( const std::error_code& err, size_t bytes ){
            if( !err ) {
                std::string msg { std::istreambuf_iterator<char>{ &ptr->mReadStreamBuf },
                    std::istreambuf_iterator<char>{} };
                msg = msg.substr( 0, msg.find_last_of('\n') );
                ptr->do_write( ptr->process_message( msg ) );
            }
        } );
}

void AsyncServer::do_write( const std::string& msg )
{
    if( !mStarted ) return;
    std::copy( msg.begin(), msg.end(), mWriteBuffer );
    mSocket.async_write_some( buffer( mWriteBuffer, msg.size() ),
        [ ptr = shared_from_this() ]( const std::error_code& err, size_t bytes ){
            std::cout << "write " << bytes << " bytes" << std::endl;
            ptr->do_read();
        } );
}

std::string AsyncServer::process_message( const std::string& msg )
{
    std::string responce {};
    if( msg.rfind( "login" ) == 0 )
    {
        mStorage->addClient( msg.substr( msg.find(':') + 1 ) );
        auto clients = mStorage->getClients();
        responce = "login:" + std::accumulate( clients.begin(), clients.end(), std::string{";"} ) + "\n";
    }
    else if( msg.rfind( "ping" ) == 0 )
    {
        std::string clients = updateClients();
        responce = clients.empty() ? "ping:ok\n" : "clients:" + clients + '\n'; 
    }
    else
    {
        std::cout << "incorret message format: \"" << msg << "\"" << std::endl;
    }

    return responce;
}

std::string AsyncServer::updateClients()
{
    std::string changeList;
    auto newClientList = mStorage->getClients();
    std::vector < std::string > addClients, delClients;
    std::set_difference( newClientList.begin(), newClientList.end(),
                         mLocalClients.begin(), mLocalClients.end(),
                         std::back_inserter( addClients ) );
    std::set_difference( mLocalClients.begin(), mLocalClients.end(),
                         newClientList.begin(), newClientList.end(),
                         std::back_inserter( delClients ) );

    for( auto& s: addClients )
        changeList += "(add:"+s+")";
    for( auto& s: delClients )
        changeList += "(del:"+s+")";

    mLocalClients = newClientList;

    return changeList;
}


