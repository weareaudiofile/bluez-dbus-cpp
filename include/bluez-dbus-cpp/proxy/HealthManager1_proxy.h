
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp_____include_bluez_dbus_cpp_proxy_HealthManager1_proxy_h__proxy__H__
#define __sdbuscpp_____include_bluez_dbus_cpp_proxy_HealthManager1_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace bluez {

class HealthManager1_proxy
{
public:
    static constexpr const char* INTERFACE_NAME = "org.bluez.HealthManager1";

protected:
    HealthManager1_proxy(sdbus::IProxy& proxy)
        : proxy_(proxy)
    {
    }

    ~HealthManager1_proxy() = default;

public:
    sdbus::ObjectPath CreateApplication(const std::map<std::string, sdbus::Variant>& config)
    {
        sdbus::ObjectPath result;
        proxy_.callMethod("CreateApplication").onInterface(INTERFACE_NAME).withArguments(config).storeResultsTo(result);
        return result;
    }

    void DestroyApplication(const sdbus::ObjectPath& application)
    {
        proxy_.callMethod("DestroyApplication").onInterface(INTERFACE_NAME).withArguments(application);
    }

private:
    sdbus::IProxy& proxy_;
};

}} // namespaces

#endif
