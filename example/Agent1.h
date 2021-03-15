//
// Copyright Audiofile LLC 2019 - 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "bluez-dbus-cpp/adaptor/Agent1_adaptor.h"

#include <sdbus-c++/sdbus-c++.h>

#include <iostream>

namespace org {
namespace bluez {

using namespace sdbus;

class Agent1 :
    public AdaptorInterfaces<Agent1_adaptor>,
    public std::enable_shared_from_this<Agent1>
{
    static constexpr const char* INTERFACE_NAME = "org.bluez.Agent1";

public:
    Agent1( std::shared_ptr<IConnection> connection, std::string objectPath )
        : AdaptorInterfaces{ *connection, objectPath },
          connection_{ connection },
          path_{ std::move(objectPath) }
    {
        registerAdaptor();
    }

    ~Agent1()
    {
        unregisterAdaptor();
    }

protected:
    void Release() override
    {
        std::cout << "Agent1::Release()" << std::endl;
    }
    
    std::string RequestPinCode(const sdbus::ObjectPath& device) override
    {
        std::cout << "Agent1::RequestPinCode(" << device << ") returning 0000" << std::endl;
        return "0000";
    }

    void DisplayPinCode(const sdbus::ObjectPath& device, const std::string& pincode) override
    {
        std::cout << "Agent1::DisplayPinCode(" << device << "," << pincode << ")" << std::endl;
    }

    uint32_t RequestPasskey(const sdbus::ObjectPath& device) override
    {
        std::cout << "Agent1::RequestPasskey(" << device << ") returning 0" << std::endl;
        return 0;
    }

    void DisplayPasskey(const sdbus::ObjectPath& device, const uint32_t& passkey, const uint16_t& entered) override
    {
        std::cout << "Agent1::DisplayPasskey(" << device << "," << passkey << "," << entered << ")" << std::endl;
    }

    void RequestAuthorization(const sdbus::ObjectPath& device) override
    {
        std::cout << "Agent1::RequestPasskey(" << device << ") returning 0" << std::endl;
    }

    void AuthorizeService(const sdbus::ObjectPath& device, const std::string& uuid) override
    {
        std::cout << "Agent1::AuthorizeService(" << device << "," << uuid << ")" << std::endl;
    }

    void Cancel() override
    {
        std::cout << "Agent1::Cancel()" << std::endl;
    }

protected:
    std::string path_;
    std::shared_ptr<IConnection> connection_;
};

}}
