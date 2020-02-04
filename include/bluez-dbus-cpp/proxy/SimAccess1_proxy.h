//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __sdbuscpp_____proxy_SimAccess1_proxy_h__proxy__H__
#define __sdbuscpp_____proxy_SimAccess1_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace bluez {

class SimAccess1_proxy
{
public:
    static constexpr const char* INTERFACE_NAME = "org.bluez.SimAccess1";

protected:
    SimAccess1_proxy(sdbus::IProxy& proxy)
        : proxy_(proxy)
    {
    }

    ~SimAccess1_proxy() = default;

public:
    void Disconnect()
    {
        proxy_.callMethod("Disconnect").onInterface(INTERFACE_NAME);
    }

public:
    bool Connected()
    {
        return proxy_.getProperty("Connected").onInterface(INTERFACE_NAME);
    }

private:
    sdbus::IProxy& proxy_;
};

}} // namespaces

#endif
