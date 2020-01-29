//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#include "bluez/Util.h"
#include <algorithm>

namespace org {
namespace bluez {

std::string Util::getDeviceMAC( const sdbus::ObjectPath& path )
{
    std::string mac;
    size_t pLen = path.size();

    if( pLen > 17 &&
        path.at( pLen - 18 ) == '_' )
    {
        mac = path.substr( pLen - 17 );
        std::replace( mac.begin(), mac.end(), '_', ':' );
    }

    return move(mac);
}

std::string Util::optionsListToString( const std::map<std::string, sdbus::Variant>& options )
{
    std::string res;
    
    for( auto iter{ options.cbegin() }; iter != options.cend(); iter++ )
    {
        if( iter->first == "device" )
        {
            res.append("device=").append(Util::getDeviceMAC( iter->second.get<sdbus::ObjectPath>() )).append(", ");
        }
        else if( iter->first == "MTU" )
        {
            res.append("MTU=").append(std::to_string( iter->second.get<uint16_t>() )).append(", ");
        }
        else if( iter->first == "link" )
        {
            res.append("link=").append(iter->second.get<std::string>()).append(", ");
        }
        else
        { 
            res.append("'").append(iter->first).append("',");
        }
    }

    return move(res);
}

}}
