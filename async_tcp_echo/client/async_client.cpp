#include "async_client.h"

io_service AsyncClient::service{};

AsyncClient::AsyncClient( const std::string &message )
    : mSocket( service )
    , mStarted( true )
    , mMessage( message ) {}

void AsyncClient::start( ip::tcp::endpoint ep )
{
    auto ptr = shared_from_this();
    mSocket.async_connect(ep, [ ptr ]( const std::error_code& err ){
            err ? ptr->stop() : ptr->do_write( ptr->mMessage + '\n' );
        } );
}

AsyncClient::ptr AsyncClient::start( ip::tcp::endpoint ep, const std::string& message )
{
    std::shared_ptr<AsyncClient> new_{ new AsyncClient { message } };
    new_->start( ep );
    return new_;
}

void AsyncClient::stop()
{
    if( !mStarted ) return;
    mStarted = false;
    mSocket.close();
}

void AsyncClient::do_read()
{
    async_read_until( mSocket, mReadStreamBuf, '\n',
        [ ptr = shared_from_this() ]( const std::error_code& err, size_t bytes ){
            if( !err ) {
                std::string msg { std::istreambuf_iterator<char>{ &ptr->mReadStreamBuf },
                    std::istreambuf_iterator<char>{} };
                std::cout << "server echoed: " << msg << std::endl;
            }
            ptr->stop();
        } );
}

void AsyncClient::do_write( const std::string& msg )
{
    if( !mStarted ) return;
    std::copy( msg.begin(), msg.end(), mWriteBuffer );
    std::cout << "start write " << msg << std::endl;
    mSocket.async_write_some( buffer( mWriteBuffer, msg.size() ), 
        [ ptr = shared_from_this() ]( const std::error_code& err, size_t bytes ){ 
            ptr->do_read();
    } );
}
