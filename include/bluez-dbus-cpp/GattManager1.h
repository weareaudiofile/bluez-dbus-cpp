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

class GattManager1 :
    public std::enable_shared_from_this<GattManager1>
{
    std::unique_ptr<sdbus::IProxy> proxy_;
    static constexpr const char* INTERFACE_NAME = "org.bluez.GattManager1";

public:
    GattManager1( IConnection& connection, std::string destination, std::string objectPath )
        : proxy_{ createProxy( connection, std::move(destination), std::move(objectPath) ) }
    {
        proxy_->finishRegistration();
    }

    GattManager1( std::shared_ptr<sdbus::IConnection> connection, std::string destination, std::string objectPath )
        : proxy_{ createProxy( *connection, std::move(destination), std::move(objectPath) ) }
    {
        proxy_->finishRegistration();
    }

    ~GattManager1()
    {
        proxy_->unregister();
    }

public:
    void RegisterApplication(const sdbus::ObjectPath& application, const std::map<std::string, sdbus::Variant>& options)
    {
        proxy_->callMethod("RegisterApplication").onInterface(INTERFACE_NAME).withArguments(application, options);
    }

    AsyncMethodInvoker RegisterApplicationAsync( const sdbus::ObjectPath& application, const std::map<std::string, sdbus::Variant>& options )
    {
        return proxy_->callMethodAsync("RegisterApplication").onInterface(INTERFACE_NAME).withArguments(application, options);
    }

    void UnregisterApplication(const sdbus::ObjectPath& application)
    {
        proxy_->callMethod("UnregisterApplication").onInterface(INTERFACE_NAME).withArguments(application);
    }
};

}}
