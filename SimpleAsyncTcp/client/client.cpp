#include "client.h"

io_service AsyncClient::service{};

AsyncClient::AsyncClient()
    : mSocket( service )
    , mStarted( true ) {}

void AsyncClient::start( ip::tcp::endpoint ep )
{
    std::string login;
    std::cout << "imput login: ";
    std::cin >> login;

    mSocket.async_connect(ep, [ ptr = shared_from_this(), login ]( const std::error_code& err ){
            err ? ptr->stop() : ptr->do_write( "login:" + login + '\n' );
        } );
}

AsyncClient::ptr AsyncClient::create( ip::tcp::endpoint ep )
{
    std::shared_ptr<AsyncClient> new_{ new AsyncClient {} };
    new_->start( ep );
    return new_;
}

void AsyncClient::stop()
{
    if( !mStarted ) return;
    mStarted = false;
    mSocket.close();
}

bool AsyncClient::isRun() const
{
    return mStarted;
}

void AsyncClient::do_read()
{
    async_read_until( mSocket, mReadStreamBuf, '\n',
        [ ptr = shared_from_this() ]( const std::error_code& err, size_t bytes ){
            if( !err ) {
                std::string msg { std::istreambuf_iterator<char>{ &ptr->mReadStreamBuf },
                    std::istreambuf_iterator<char>{} };
                std::cout << "server replay: " << msg << std::endl;
            }
            std::this_thread::sleep_for( std::chrono::milliseconds( 5000 ) ); //TODO from command line
            ptr->do_write( "ping\n" );
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
