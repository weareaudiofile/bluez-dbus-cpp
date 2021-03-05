//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <bluez-dbus-cpp/GattCharacteristicBuilder.h>
#include <bluez-dbus-cpp/Client.h>
#include <bluez-dbus-cpp/Device1.h>

#include <iostream>
#include <queue>
#include <set>

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

class SerialCharacteristic :
    public GattCharacteristicBuilder<GenericCharacteristic>
{
public:
    SerialCharacteristic( std::shared_ptr<GattService1> service, std::shared_ptr<IConnection> connection, std::string uuid )
        : GattCharacteristicBuilder{ move(service), move(uuid), false, false, true, true },
          connection_{ std::move(connection) }
    {
        flags_ = { "notify", "write-without-response" };
    }

    static SerialCharacteristic& create(std::shared_ptr<GattService1> service, std::shared_ptr<IConnection> connection, std::string uuid)
    {
        auto self = new SerialCharacteristic( move(service), move(connection), move(uuid) );
        return *self;
    }

protected:
    virtual std::vector<uint8_t> ReadValue(const std::map<std::string, sdbus::Variant>& options)
    {
        std::cerr << "Serial RX: " << std::string( value_.begin(), value_.end() ) << std::endl;
        throw sdbus::Error("org.bluez.Error.NotSupported", "'read' not supported on 'SerialCharacteristic', use 'notify' instead");
    }

    virtual void WriteValue(const std::vector<uint8_t>& value, const std::map<std::string, sdbus::Variant>& options)
    {
        std::cout << "Serial TX: " << std::string( value.begin(), value.end() ) << std::endl;
        auto client = getClient( options );
        if( client == lastClient_ )
        {
            if(lastDevice_
                && value.size() > 0 
                && value[0] == 'd' )
            {
                std::cout << "Disconnecting from client" << std::endl;
                lastDevice_->Disconnect();
            }
        }
        client->setData( std::move(value) );
        Notify( client );
    }

    void StartNotify(const std::map<std::string, sdbus::Variant>& options) override
    {
        // insert client into our list
        auto client = getClient( options );
        notifying_.insert( client );
        std::cout << "SerialCharacteristic::StartNotify '" << client->getPath() << "'" << std::endl;
    }

    void StopNotify(const std::map<std::string, sdbus::Variant>& options) override
    {
        // remove client from our list
        if (options.size()!=0){         //When disconnecting from device, StopNotify is called with an empty options map
            auto client = getClient( options );
            auto iter = notifying_.find( client );
            notifying_.erase( iter );
            std::cout << "SerialCharacteristic::StopNotify '" << client->getPath() << "'" << std::endl;
        }
    }

    void Notify( std::shared_ptr<SerialClient> client )
    {
        // in this example we always append a 'k' message to test the multi-packet aspect
        // that is part of the 'DirectedValue' BlueZ patch rev2 feature
        directedQueue_.insert( std::make_pair( client->getPath(), std::vector<std::vector<uint8_t>>{ client->getData(), std::vector<uint8_t>{ 'k' } } ) );

        if( notifying_.find( client ) != notifying_.end() )
        {
            emitPropertyChangedSignal( "DirectedValue" );
        }
    }

    std::map<sdbus::ObjectPath, std::vector<std::vector<uint8_t>>> DirectedValue() override
    {
        std::cout << "DirectedValue()" << std::endl;
        return std::move(directedQueue_);
    }

protected:
    std::shared_ptr<SerialClient> getClient(const std::map<std::string, sdbus::Variant>& options)
    {
        uint16_t real_mtu, usable_mtu;
        std::tie( real_mtu, usable_mtu ) = Util::getMTUFromOptions( options );

        auto path = Util::getObjectPathFromOptions( options );
        auto iter = clients_.find( path );
        if( iter == clients_.end() )
        {
            std::cout << "SerialCharacteristic::getClient - creating client '" << path << "'" << std::endl;
            auto client = std::make_shared<SerialClient>( path, usable_mtu, std::vector<uint8_t>() );
            iter = clients_.insert( std::make_pair( path, client ) ).first;
            lastClient_ = iter->second;
            lastDevice_ = std::make_shared<Device1>( *connection_, "org.bluez", path );
        }
        return iter->second;
    }

    void removeClient(const std::map<std::string, sdbus::Variant>& options)
    {
        auto path = Util::getObjectPathFromOptions( options );
        auto iter = clients_.find( path );
        if( iter != clients_.end() )
        {
            std::cout << "SerialCharacteristic::removeClient '" << path << "'" << std::endl;
            clients_.erase( iter );
        }
    }

    std::map<sdbus::ObjectPath, std::vector<std::vector<uint8_t>>> directedQueue_;
    std::map<sdbus::ObjectPath, std::shared_ptr<SerialClient>> clients_;
    std::set<std::shared_ptr<SerialClient>> notifying_;
    std::shared_ptr<SerialClient> lastClient_;
    std::shared_ptr<Device1> lastDevice_;
    std::shared_ptr<IConnection> connection_;
};

}}
