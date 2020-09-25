//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "proxy/Network1_proxy.h"

namespace org {
namespace bluez {
namespace mesh {

using namespace sdbus;

class Network1 : public ProxyInterfaces<org::bluez::mesh::Network1_proxy>
{
public:
    Network1( std::shared_ptr<IConnection> connection )
        : ProxyInterfaces{ *connection, "org.bluez.mesh", "/org/bluez/mesh" }
    {
        registerProxy();
    }

    ~Network1()
    {
        unregisterProxy();
    }
};

}}}
