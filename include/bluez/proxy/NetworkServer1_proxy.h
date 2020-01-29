//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __sdbuscpp_____proxy_NetworkServer1_proxy_h__proxy__H__
#define __sdbuscpp_____proxy_NetworkServer1_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace bluez {

class NetworkServer1_proxy
{
public:
    static constexpr const char* INTERFACE_NAME = "org.bluez.NetworkServer1";

protected:
    NetworkServer1_proxy(sdbus::IProxy& proxy)
        : proxy_(proxy)
    {
    }

    ~NetworkServer1_proxy() = default;

public:
    void Register(const std::string& uuid, const std::string& bridge)
    {
        proxy_.callMethod("Register").onInterface(INTERFACE_NAME).withArguments(uuid, bridge);
    }

    void Unregister(const std::string& uuid)
    {
        proxy_.callMethod("Unregister").onInterface(INTERFACE_NAME).withArguments(uuid);
    }

private:
    sdbus::IProxy& proxy_;
};

}} // namespaces

#endif
