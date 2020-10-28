//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "GattCharacteristicBuilder.h"

namespace org {
namespace bluez {

class GenericCharacteristic :
    public GattCharacteristicBuilder<GenericCharacteristic>
{
public:
    GenericCharacteristic( std::shared_ptr<GattService1> service, std::string uuid )
        : GattCharacteristicBuilder{ move(service), move(uuid), false, false, true }
    {
    }

    static GenericCharacteristic& create(std::shared_ptr<GattService1> service, std::string uuid)
    {
        auto self = new GenericCharacteristic{ move(service), move(uuid) };
        return *self;
    }
};

}}
