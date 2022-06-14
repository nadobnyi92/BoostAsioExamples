#include "async_server.h"

io_service AsyncServer::service {};

AsyncServer::AsyncServer()
    : mSocket( service )
    , mStarted( false ) {}

AsyncServer::ptr AsyncServer::create()
{
    return ptr { new AsyncServer{} };
}

ip::tcp::socket& AsyncServer::sock()
{
    return mSocket;
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
                ptr->do_write( msg );
            }
        } );
}

void AsyncServer::do_write( const std::string& msg )
{
    if( !mStarted ) return;
    std::copy( msg.begin(), msg.end(), mWriteBuffer );
    mSocket.async_write_some( buffer( mWriteBuffer, msg.size() ),
        [ ptr = shared_from_this() ]( const std::error_code& err, size_t bytes ){
            ptr->do_read();
        } );
}

