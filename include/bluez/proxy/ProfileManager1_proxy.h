//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __sdbuscpp_____proxy_ProfileManager1_proxy_h__proxy__H__
#define __sdbuscpp_____proxy_ProfileManager1_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace bluez {

class ProfileManager1_proxy
{
public:
    static constexpr const char* INTERFACE_NAME = "org.bluez.ProfileManager1";

protected:
    ProfileManager1_proxy(sdbus::IProxy& proxy)
        : proxy_(proxy)
    {
    }

    ~ProfileManager1_proxy() = default;

public:
    void RegisterProfile(const sdbus::ObjectPath& profile, const std::string& UUID, const std::map<std::string, sdbus::Variant>& options)
    {
        proxy_.callMethod("RegisterProfile").onInterface(INTERFACE_NAME).withArguments(profile, UUID, options);
    }

    void UnregisterProfile(const sdbus::ObjectPath& profile)
    {
        proxy_.callMethod("UnregisterProfile").onInterface(INTERFACE_NAME).withArguments(profile);
    }

private:
    sdbus::IProxy& proxy_;
};

}} // namespaces

#endif
