//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "GattCharacteristicBuilder.h"

namespace org {
namespace bluez {

class ReadOnlyCharacteristic :
    public GattCharacteristicBuilder<ReadOnlyCharacteristic>
{
public:
    ReadOnlyCharacteristic( std::shared_ptr<GattService1> service, std::string uuid, std::string value )
        : GattCharacteristicBuilder{ move(service), move(uuid), false, false, true }
    {
        addFlag( "read" );
        addValue( move(value) );
    }

    static std::shared_ptr<ReadOnlyCharacteristic> createFinal(std::shared_ptr<GattService1> service, std::string uuid, std::string value )
    {
        auto self = new ReadOnlyCharacteristic{ move(service), move(uuid), move(value) };
        auto shared = std::shared_ptr<ReadOnlyCharacteristic>( self );
        self->registerWithService( shared );
        return shared;
    }
};

}}
