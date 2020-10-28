//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "GattCharacteristic1.h"

namespace org {
namespace bluez {

// Forward declarations
class GattService1;

using namespace sdbus;


template<typename _Derived>
class GattCharacteristicBuilder :
    public GattCharacteristic1
{
protected:
    GattCharacteristicBuilder( std::shared_ptr<GattService1> service,
        std::string uuid,
        bool hasAcquireWrite = false,
        bool hasAcquireNotify = false,
        bool hasValue = false,
        bool valueIsDirected = false )
        : GattCharacteristic1{ move(service), move(uuid), hasAcquireWrite, hasAcquireNotify, hasValue, valueIsDirected }
    {
    }

public:
    _Derived& withValue( std::string value )
    {
        addValue( move(value) );
        return static_cast<_Derived&>( *this );
    }

    _Derived& withValue( std::vector<uint8_t> value )
    {
        addValue( move(value) );
        return static_cast<_Derived&>( *this );
    }

    _Derived& withFlag( std::string flag )
    {
        addFlag( move(flag) );
        return static_cast<_Derived&>( *this );
    }

    std::shared_ptr<_Derived> finalize()
    {
        auto self = std::shared_ptr<GattCharacteristicBuilder>( this );
        registerWithService( self );
        return std::static_pointer_cast<_Derived>(self);
    }
};

}}
