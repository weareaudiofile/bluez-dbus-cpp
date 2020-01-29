//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __sdbuscpp_____adaptor_GattService1_adaptor_h__adaptor__H__
#define __sdbuscpp_____adaptor_GattService1_adaptor_h__adaptor__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace bluez {

class GattService1_adaptor
{
public:
    static constexpr const char* INTERFACE_NAME = "org.bluez.GattService1";

protected:
    GattService1_adaptor(sdbus::IObject& object)
        : object_(object)
    {
        object_.registerProperty("UUID").onInterface(INTERFACE_NAME).withGetter([this](){ return this->UUID(); });
        object_.registerProperty("Primary").onInterface(INTERFACE_NAME).withGetter([this](){ return this->Primary(); });
        object_.registerProperty("Includes").onInterface(INTERFACE_NAME).withGetter([this](){ return this->Includes(); });
    }

    ~GattService1_adaptor() = default;

private:
    virtual std::string UUID() = 0;
    virtual bool Primary() = 0;
    virtual std::vector<sdbus::ObjectPath> Includes() = 0;

private:
    sdbus::IObject& object_;
};

}} // namespaces

#endif
