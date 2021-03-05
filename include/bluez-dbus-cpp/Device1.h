//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "proxy/Device1_proxy.h"

namespace org {
namespace bluez {

using namespace sdbus;

class Device1 : public ProxyInterfaces<org::bluez::Device1_proxy>
{
public:
    Device1( IConnection& connection, std::string destination, std::string objectPath )
        : ProxyInterfaces{ connection, std::move(destination), std::move(objectPath) }
    {
        registerProxy();
    }

    ~Device1()
    {
        unregisterProxy();
    }
};

}}
