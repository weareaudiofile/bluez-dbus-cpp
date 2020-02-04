//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __sdbuscpp_____src_bluez_adaptor_ObjectManagerExt_adaptor_h__adaptor__H__
#define __sdbuscpp_____src_bluez_adaptor_ObjectManagerExt_adaptor_h__adaptor__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace bluez {

class ObjectManagerExt_adaptor : public ObjectManager_adaptor
{
public:
    static constexpr const char* INTERFACE_NAME = "org.freedesktop.DBus.ObjectManager";

protected:
    ObjectManagerExt_adaptor(sdbus::IObject& object)
        : ObjectManager_adaptor(object)
    {
        object.registerMethod("GetManagedObjects").onInterface(INTERFACE_NAME).implementedAs([this](){ return this->GetManagedObjects(); });
    }

    ~ObjectManagerExt_adaptor() = default;

private:
    virtual std::map<sdbus::ObjectPath, std::map<std::string, std::map<std::string, sdbus::Variant>>> GetManagedObjects() = 0;
};

}} // namespaces

#endif
