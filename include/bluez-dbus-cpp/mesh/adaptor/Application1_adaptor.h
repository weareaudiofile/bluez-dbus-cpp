
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp_____include_bluez_dbus_cpp_mesh_adaptor_Application1_adaptor_h__adaptor__H__
#define __sdbuscpp_____include_bluez_dbus_cpp_mesh_adaptor_Application1_adaptor_h__adaptor__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace bluez {
namespace mesh {

class Application1_adaptor
{
public:
    static constexpr const char* INTERFACE_NAME = "org.bluez.mesh.Application1";

protected:
    Application1_adaptor(sdbus::IObject& object)
        : object_(object)
    {
        object_.registerMethod("JoinComplete").onInterface(INTERFACE_NAME).implementedAs([this](const uint64_t& token){ return this->JoinComplete(token); });
        object_.registerMethod("JoinFailed").onInterface(INTERFACE_NAME).implementedAs([this](const std::string& reason){ return this->JoinFailed(reason); });
        object_.registerProperty("CompanyID").onInterface(INTERFACE_NAME).withGetter([this](){ return this->CompanyID(); });
        object_.registerProperty("ProductID").onInterface(INTERFACE_NAME).withGetter([this](){ return this->ProductID(); });
        object_.registerProperty("VersionID").onInterface(INTERFACE_NAME).withGetter([this](){ return this->VersionID(); });
        object_.registerProperty("CRPL").onInterface(INTERFACE_NAME).withGetter([this](){ return this->CRPL(); });
    }

    ~Application1_adaptor() = default;

private:
    virtual void JoinComplete(const uint64_t& token) = 0;
    virtual void JoinFailed(const std::string& reason) = 0;

private:
    virtual uint16_t CompanyID() = 0;
    virtual uint16_t ProductID() = 0;
    virtual uint16_t VersionID() = 0;
    virtual uint16_t CRPL() = 0;

private:
    sdbus::IObject& object_;
};

}}} // namespaces

#endif
