#include <system_error>
#include <regex>
#include <chrono>
#include <iostream>
#include <iomanip>

#include <boost/asio/error.hpp>

#include "client_processor.h"
#include "client_storage.h"

io_service ClientProcessor::service {};

ClientProcessor::ClientProcessor( ClientStorage& storage, uint32_t timeout )
    : mTimeout( timeout )
    , mSocket( service )
    , mClientStorage( storage ) {}


std::string ClientProcessor::clientName() const
{
    return mName;
}

bool ClientProcessor::login()
{
    streambuf responce {};
    boost::system::error_code ec {};
    read_until(mSocket, responce, '\n', ec);
    if( ec == boost::asio::error::eof )
    {
        std::cout << "connection closed" << std::endl;
        return false;
    }

    std::string responce_str{ std::istreambuf_iterator<char>{ &responce }, 
                     std::istreambuf_iterator<char>{} };
    std::regex re { "^login:(.*)\n" };
    std::smatch match {};
    if( std::regex_search( responce_str, match, re ) && match.size() == 2 )
    {
        std::cout << "new client loggin: " << match[1] << std::endl;
        mName = match[1];

        std::string changeList = clientChangeList();
        mSocket.write_some( buffer( changeList.empty() ? "login:ok\n" : "clients:"+changeList+"\n" ) );

        mActiveTime = std::chrono::high_resolution_clock::now();
    }
    else
    {
        std::cout << "incorret login message format" << std::endl;
        return false;
    }

    return true;
}

void ClientProcessor::checkRequest()
{
    streambuf responce {};
    boost::system::error_code ec {};
    if( !mSocket.available() )
    {
        return;
    }
    read_until(mSocket, responce, '\n', ec);
    if( ec == boost::asio::error::eof )
    {
        std::cout << "connection closed" << std::endl;
        return;
    }

    std::string responce_str{ std::istreambuf_iterator<char>{ &responce }, 
                     std::istreambuf_iterator<char>{} };
    responce_str = responce_str.substr( 0, responce_str.find_last_of('\n') );

    if( responce_str.compare( "ping" ) == 0 )
    {
        std::cout << "ping ok" << std::endl;

        std::string changeList = clientChangeList();
        std::cout << "write: " << (changeList.empty() ? "ping:ok\n" : "clients:"+changeList+"\n") << std::endl;
        mSocket.write_some( buffer( changeList.empty() ? "ping:ok\n" : "clients:"+changeList+"\n" ) );

        mActiveTime = std::chrono::high_resolution_clock::now();
    }
    else
    {
        std::cout << "incorret ping format: \"" << responce_str << "\"" << std::endl;
    }

    return;
}

bool ClientProcessor::isOverTime()
{
    auto timeDiff = std::chrono::duration<double, std::milli>( std::chrono::high_resolution_clock::now() - mActiveTime );
    return timeDiff.count() > mTimeout;
}

ip::tcp::socket& ClientProcessor::socket()
{
    return mSocket;
}

std::string ClientProcessor::clientChangeList()
{
    std::string changeList;
    std::vector < std::string > newClients, delClients;

    std::vector < std::string > newClientList = mClientStorage.getClientsNames();
    std::set_difference( newClientList.begin(), newClientList.end(),
        mClientList.begin(), mClientList.end(), std::back_inserter(newClients) );
    std::set_difference( mClientList.begin(), mClientList.end(),
        newClientList.begin(), newClientList.end(), std::back_inserter(delClients) );

    for( auto& s: newClients )
        changeList += "(add:"+s+")";
    for( auto& s: delClients )
        changeList += "(del:"+s+")";

    mClientList = newClientList;
    return changeList;
}