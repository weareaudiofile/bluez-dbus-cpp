//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <bluez-dbus-cpp/GattCharacteristicBuilder.h>
#include <bluez-dbus-cpp/Client.h>

#include <iostream>
#include <queue>

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
    SerialCharacteristic( std::shared_ptr<GattService1> service, std::string uuid )
        : GattCharacteristicBuilder{ move(service), move(uuid), false, false, true, true }
    {
        flags_ = { "notify", "write-without-response" };
    }

    static SerialCharacteristic& create(std::shared_ptr<GattService1> service, std::string uuid)
    {
        auto self = new SerialCharacteristic( move(service), move(uuid) );
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
        client->setData( std::move(value) );
        Notify( client );
    }

    void Notify( std::shared_ptr<SerialClient> client )
    {
        // in this example we always append a 'k' message to test the multi-packet aspect
        // that is part of the 'DirectedValue' BlueZ patch rev2 feature
        directedQueue_.insert( std::make_pair( client->getPath(), std::vector<std::vector<uint8_t>>{ client->getData(), std::vector<uint8_t>{ 'k' } } ) );

        if( notifyingSessions_ > 0 )
        {
            emitPropertyChangedSignal( "DirectedValue" );
        }
    }

    std::map<sdbus::ObjectPath, std::vector<std::vector<uint8_t>>> DirectedValue()
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
            auto client = std::make_shared<SerialClient>( path, usable_mtu, std::vector<uint8_t>() );
            iter = clients_.insert( std::make_pair( path, client ) ).first;
        }
        return iter->second;
    }

    std::map<sdbus::ObjectPath, std::vector<std::vector<uint8_t>>> directedQueue_;
    std::map<sdbus::ObjectPath, std::shared_ptr<SerialClient>> clients_;
};

}}
