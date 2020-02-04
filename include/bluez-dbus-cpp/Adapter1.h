//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "proxy/Adapter1_proxy.h"

namespace org {
namespace bluez {

using namespace sdbus;

class Adapter1 : public ProxyInterfaces<org::bluez::Adapter1_proxy>
{
public:
    Adapter1( IConnection& connection, std::string destination, std::string objectPath )
        : ProxyInterfaces{ connection, std::move(destination), std::move(objectPath) }
    {
        registerProxy();
    }

    ~Adapter1()
    {
        unregisterProxy();
    }
};

}}
