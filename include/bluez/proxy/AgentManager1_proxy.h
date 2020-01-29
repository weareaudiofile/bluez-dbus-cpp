//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __sdbuscpp_____proxy_AgentManager1_proxy_h__proxy__H__
#define __sdbuscpp_____proxy_AgentManager1_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace bluez {

class AgentManager1_proxy
{
public:
    static constexpr const char* INTERFACE_NAME = "org.bluez.AgentManager1";

protected:
    AgentManager1_proxy(sdbus::IProxy& proxy)
        : proxy_(proxy)
    {
    }

    ~AgentManager1_proxy() = default;

public:
    void RegisterAgent(const sdbus::ObjectPath& agent, const std::string& capability)
    {
        proxy_.callMethod("RegisterAgent").onInterface(INTERFACE_NAME).withArguments(agent, capability);
    }

    void UnregisterAgent(const sdbus::ObjectPath& agent)
    {
        proxy_.callMethod("UnregisterAgent").onInterface(INTERFACE_NAME).withArguments(agent);
    }

    void RequestDefaultAgent(const sdbus::ObjectPath& agent)
    {
        proxy_.callMethod("RequestDefaultAgent").onInterface(INTERFACE_NAME).withArguments(agent);
    }

private:
    sdbus::IProxy& proxy_;
};

}} // namespaces

#endif
