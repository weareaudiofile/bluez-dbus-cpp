//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <bluez-dbus-cpp/GattCharacteristicBuilder.h>
#include <bluez-dbus-cpp/Client.h>
#include "EllSocket.h"

#include <iostream>

namespace org {
namespace bluez {

class SerialClient : public Client
{
public:
    SerialClient(sdbus::ObjectPath path, uint16_t usable_mtu, std::vector<uint8_t>&& initialValue) :
        Client{path, usable_mtu},
        value_{ initialValue }
    {

    }

    void setData( std::vector<uint8_t>&& value )
    {
        this->value_ = std::move(value);
    }

    void setData( const std::vector<uint8_t>& value )
    {
        this->value_.assign( value.begin(), value.end() );
    }

    std::vector<uint8_t>&& getData()
    {
        return std::move(value_);
    }

protected:
    std::vector<uint8_t> value_;
};

constexpr const int WRITE_SOCKET{ 1 };
constexpr const int NOTIFY_SOCKET{ 2 };

class SerialCharacteristic :
    public GattCharacteristicBuilder<GenericCharacteristic>
{
public:
    SerialCharacteristic( std::shared_ptr<GattService1> service, std::string uuid )
        : GattCharacteristicBuilder{ move(service), move(uuid), true, true }
    {
        flags_ = { "read", "notify", "write-without-response", "write" };
    }

    static SerialCharacteristic& create(std::shared_ptr<GattService1> service, std::string uuid)
    {
        auto self = new SerialCharacteristic( move(service), move(uuid) );
        return *self;
    }

protected:
    virtual std::vector<uint8_t> ReadValue(const std::map<std::string, sdbus::Variant>& options)
    {
        std::cout << "ReadValue: " << std::string( value_.begin(), value_.end() ) << std::endl;
        auto client = getClient( options );
        return client->getData();
    }

    virtual void WriteValue(const std::vector<uint8_t>& value, const std::map<std::string, sdbus::Variant>& options)
    {
        std::cout << "WriteValue: " << std::string( value.begin(), value.end() ) << std::endl;
        auto client = getClient( options );
        client->setData( std::move(value) );
        Notify( client );
    }

    void Write( std::shared_ptr<Client> client, const std::vector<uint8_t>& value )
    {
        std::cout << "Write " << std::string( value.begin(), value.end() ) << std::endl;
        auto sclient = std::static_pointer_cast<SerialClient>( client );
        sclient->setData( std::move(value) );
        Notify( sclient );
    }

    void Notify( std::shared_ptr<SerialClient> client )
    {
        if( notifyingSessions_ > 0 )
        {
            auto socket = client->getSocket( NOTIFY_SOCKET );
            socket->send( client->getData() );
        }
    }

    std::shared_ptr<SerialClient> getClient(const std::map<std::string, sdbus::Variant>& options)
    {
        uint16_t real_mtu, usable_mtu;
        std::tie( real_mtu, usable_mtu ) = Util::getMTUFromOptions( options );

        auto path = Util::getObjectPathFromOptions( options );
        auto iter = clients_.find( path );
        if( iter == clients_.end() )
        {
            auto client = std::make_shared<SerialClient>( path, usable_mtu, std::vector<uint8_t>() );
            iter = clients_.insert( std::make_pair( path, client ) ).first;
        }
        return iter->second;
    }

    void WriteDisconnectHandler( std::shared_ptr<Client> client )
    {
        client->removeSocket( WRITE_SOCKET );
        writeAcquired_ = false;
        emitPropertyChangedSignal( "WriteAcquired" );
    }

    virtual std::tuple<sdbus::UnixFd, uint16_t> AcquireWrite(const std::map<std::string, sdbus::Variant>& options)
    {
        std::cout << "AcquireWrite() " << Util::optionsListToString( options ) << std::endl;

        auto client = getClient( options );

        auto socket = client->getSocket( WRITE_SOCKET );
        if( socket == nullptr )
        {
            socket = std::make_shared<EllSocket>( 
                true,
                client,
                std::bind( &SerialCharacteristic::WriteDisconnectHandler, this, std::placeholders::_1 ),
                std::bind( &SerialCharacteristic::Write, this, std::placeholders::_1, std::placeholders::_2 )
            );
            client->addSocket( WRITE_SOCKET, socket );
        }

        if( writeAcquired_ == false )
        {
            writeAcquired_ = true;
            emitPropertyChangedSignal( "WriteAcquired" );
        }

        return socket->acquireResponse();
    }

    void NotifyDisconnectHandler( std::shared_ptr<Client> client )
    {
        client->removeSocket( NOTIFY_SOCKET );
        notifyAcquired_ = false;
        emitPropertyChangedSignal( "NotifyAcquired" );
    }

    virtual std::tuple<sdbus::UnixFd, uint16_t> AcquireNotify(const std::map<std::string, sdbus::Variant>& options)
    {
        std::cout << "AcquireNotify() " << Util::optionsListToString( options ) << std::endl;
        
        auto client = getClient( options );

        auto socket = client->getSocket( NOTIFY_SOCKET );
        if( socket == nullptr )
        {
            socket = std::make_shared<EllSocket>( 
                false,
                client,
                std::bind( &SerialCharacteristic::NotifyDisconnectHandler, this, std::placeholders::_1 ),
                nullptr
            );
            client->addSocket( NOTIFY_SOCKET, socket );
        }

        if( notifyAcquired_ == false )
        {
            notifyAcquired_ = true;
            emitPropertyChangedSignal( "NotifyAcquired" );
        }

        return socket->acquireResponse();
    }

protected:
    std::map<sdbus::ObjectPath, std::shared_ptr<SerialClient>> clients_;

};

}}
