//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <sdbus-c++/sdbus-c++.h>

using namespace sdbus;

class PropertiesExt_adaptor : public Properties_adaptor
{
public:
    PropertiesExt_adaptor(sdbus::IObject& object)
        : Properties_adaptor(object)
    {
    }

    ~PropertiesExt_adaptor() = default;

protected:
    void emitPropertyChangedSignal( const std::string& interfaceName, const std::string& property )
    {
        emitPropertiesChangedSignal( interfaceName, std::vector<std::string>{ property } );
    }
};