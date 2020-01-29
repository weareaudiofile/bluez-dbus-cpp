//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <sdbus-c++/sdbus-c++.h>

namespace org {
namespace bluez {

using namespace sdbus;

class LEAdvertisingManager1 :
    public std::enable_shared_from_this<LEAdvertisingManager1>
{
    std::unique_ptr<sdbus::IProxy> proxy_;
    static constexpr const char* INTERFACE_NAME = "org.bluez.LEAdvertisingManager1";

public:
    LEAdvertisingManager1( IConnection& connection, std::string destination, std::string objectPath )
        : proxy_{ createProxy( connection, std::move(destination), std::move(objectPath) ) }
    {
        proxy_->finishRegistration();
    }

    LEAdvertisingManager1( std::shared_ptr<sdbus::IConnection> connection, std::string destination, std::string objectPath )
        : proxy_{ createProxy( *connection, std::move(destination), std::move(objectPath) ) }
    {
        proxy_->finishRegistration();
    }

    ~LEAdvertisingManager1()
    {
        proxy_->unregister();
    }

public:
    void RegisterAdvertisement( const sdbus::ObjectPath& advertisementPath, const std::map<std::string, sdbus::Variant>& options = {} )
    {
        proxy_->callMethod("RegisterAdvertisement").onInterface(INTERFACE_NAME).withArguments( advertisementPath, options );
    }

    AsyncMethodInvoker RegisterAdvertisementAsync( const sdbus::ObjectPath& advertisementPath, const std::map<std::string, sdbus::Variant>& options = {} )
    {
        return proxy_->callMethodAsync("RegisterAdvertisement").onInterface(INTERFACE_NAME).withArguments( advertisementPath, options );
    }

    void UnregisterAdvertisement(const sdbus::ObjectPath& advertisementPath)
    {
        proxy_->callMethod("UnregisterAdvertisement").onInterface(INTERFACE_NAME).withArguments(advertisementPath);
    }

public:
    uint8_t ActiveInstances()
    {
        return proxy_->getProperty("ActiveInstances").onInterface(INTERFACE_NAME);
    }

    uint8_t SupportedInstances()
    {
        return proxy_->getProperty("SupportedInstances").onInterface(INTERFACE_NAME);
    }

    std::vector<std::string> SupportedIncludes()
    {
        return proxy_->getProperty("SupportedIncludes").onInterface(INTERFACE_NAME);
    }
};

}}
