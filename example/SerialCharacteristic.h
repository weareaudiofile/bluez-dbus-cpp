//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "bluez-ext/GattCharacteristicBuilder.h"

#include <iostream>

namespace org {
namespace bluez {

class SerialCharacteristic :
    public GattCharacteristicBuilder<GenericCharacteristic>
{
public:
    SerialCharacteristic( std::shared_ptr<GattService1> service, std::string uuid )
        : GattCharacteristicBuilder{ move(service), move(uuid) }
    {
        flags_ = { "read", "write", "indicate" };
    }

    static SerialCharacteristic& create(std::shared_ptr<GattService1> service, std::string uuid)
    {
        auto self = new SerialCharacteristic( move(service), move(uuid) );
        return *self;
    }

protected:
    // virtual std::vector<uint8_t> ReadValue(const std::map<std::string, sdbus::Variant>& options)
    // {
    //     // TODO handle options
    //     return value_;
    // }

    // virtual void WriteValue(const std::vector<uint8_t>& value, const std::map<std::string, sdbus::Variant>& options)
    // {
    //     // TODO handle options
    //     std::cout << "Serial TX: " << std::string( value.begin(), value.end() ) << std::endl;
    //     value_ = value;
    // }
};

}}
