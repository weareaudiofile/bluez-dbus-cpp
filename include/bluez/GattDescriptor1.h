//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "adaptor/PropertiesExt_adaptor.h"
#include "adaptor/GattDescriptor1_adaptor.h"

#include <sdbus-c++/sdbus-c++.h>

namespace org {
namespace bluez {

// Forward declarations
class GattCharacteristic1;

using namespace sdbus;

class GattDescriptor1 :
    public AdaptorInterfaces<GattDescriptor1_adaptor, PropertiesExt_adaptor>,
    public std::enable_shared_from_this<GattDescriptor1>
{
public:
    GattDescriptor1( std::shared_ptr<GattCharacteristic1> service, std::string name, std::string uuid );
    ~GattDescriptor1();

public:
    const std::string& getPath() const
    {
        return path_;
    }

public:
    virtual std::vector<uint8_t> ReadValue(const std::map<std::string, sdbus::Variant>& options)
    {
        throw sdbus::Error("org.bluez.Error.NotSupported", "Method 'ReadValue' default handler");
    }

    virtual void WriteValue(const std::vector<uint8_t>& value, const std::map<std::string, sdbus::Variant>& options)
    {
        throw sdbus::Error("org.bluez.Error.NotSupported", "Method 'WriteValue' default handler");
    }

public:
    /**
     * @brief 
     * 
     * @return std::string 
     */
    virtual std::string UUID()
    {
        return uuid_;
    }

    virtual sdbus::ObjectPath Characteristic()
    {
        // TODO implement
        return sdbus::ObjectPath();
    }

    virtual std::vector<uint8_t> Value()
    {
        // TODO implement
        return std::vector<uint8_t>();
    }

    virtual std::vector<std::string> Flags()
    {
        // TODO implement
        return std::vector<std::string>();
    }

protected:
    void emitPropertyChangedSignal( const std::string& property )
    {
        PropertiesExt_adaptor::emitPropertyChangedSignal( GattDescriptor1_adaptor::INTERFACE_NAME, property );
    }

    std::map<std::string,sdbus::Variant> dict_;
    std::string path_;
    std::string uuid_;
    std::shared_ptr<GattCharacteristic1> characteristic_;
};

}}
