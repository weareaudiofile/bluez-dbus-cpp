//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <sdbus-c++/sdbus-c++.h>

namespace org {
namespace bluez {

struct Util
{
    static std::tuple<uint16_t /* real */,uint16_t /* usable */> getMTUFromOptions( const std::map<std::string, sdbus::Variant>& options );
    static sdbus::ObjectPath getObjectPathFromOptions( const std::map<std::string, sdbus::Variant>& options );
    static std::string getDeviceMAC( const sdbus::ObjectPath& path );
    static std::string optionsListToString( const std::map<std::string, sdbus::Variant>& options );
};

}}
