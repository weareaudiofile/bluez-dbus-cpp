//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __sdbuscpp_____proxy_Media1_proxy_h__proxy__H__
#define __sdbuscpp_____proxy_Media1_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace bluez {

class Media1_proxy
{
public:
    static constexpr const char* INTERFACE_NAME = "org.bluez.Media1";

protected:
    Media1_proxy(sdbus::IProxy& proxy)
        : proxy_(proxy)
    {
    }

    ~Media1_proxy() = default;

public:
    void RegisterEndpoint(const sdbus::ObjectPath& endpoint, const std::map<std::string, sdbus::Variant>& properties)
    {
        proxy_.callMethod("RegisterEndpoint").onInterface(INTERFACE_NAME).withArguments(endpoint, properties);
    }

    void UnregisterEndpoint(const sdbus::ObjectPath& endpoint)
    {
        proxy_.callMethod("UnregisterEndpoint").onInterface(INTERFACE_NAME).withArguments(endpoint);
    }

    void RegisterPlayer(const sdbus::ObjectPath& player, const std::map<std::string, sdbus::Variant>& properties)
    {
        proxy_.callMethod("RegisterPlayer").onInterface(INTERFACE_NAME).withArguments(player, properties);
    }

    void UnregisterPlayer(const sdbus::ObjectPath& player)
    {
        proxy_.callMethod("UnregisterPlayer").onInterface(INTERFACE_NAME).withArguments(player);
    }

private:
    sdbus::IProxy& proxy_;
};

}} // namespaces

#endif
