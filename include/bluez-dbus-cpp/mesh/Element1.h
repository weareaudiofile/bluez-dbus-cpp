//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "../adaptor/PropertiesExt_adaptor.h"
#include "adaptor/Element1_adaptor.h"

#include <sdbus-c++/sdbus-c++.h>

namespace org {
namespace bluez {
namespace mesh {

using namespace sdbus;

class Element1 :
    public AdaptorInterfaces<Element1_adaptor, PropertiesExt_adaptor>,
    public std::enable_shared_from_this<Element1>
{
    Element1( const Element1& service ) = delete;

private:

    std::string indexToString(uint8_t index){
        char indexString[3];
        snprintf(indexString, 3, "%.2d", index);
        std::string str(indexString);
        return indexString;
    }

public:
    Element1( std::shared_ptr<IConnection> connection, uint8_t Index ) :
        AdaptorInterfaces{ *connection, "/example/mesh/ele" + indexToString(Index) }, 
        path_{ "/example/mesh/ele" + indexToString(Index)},
        Index_{ Index }
    {
        ;
    }

public:
    // static std::shared_ptr<Element1> createWith( std::shared_ptr<IConnection> connection, std::string serviceName, uint8_t Index );
    // std::shared_ptr<Element1> andRegister();
    ~Element1() { }

public:
    virtual const std::string& getPath() const final { return path_; }
    virtual std::shared_ptr<IConnection> getConnection() const final { return connection_; }

    uint8_t getIndex()
    {
        return Index_;
    }

    std::vector<sdbus::Struct<uint16_t, std::map<std::string, sdbus::Variant>>> getSigModels(){
        return Models_;
    };

    std::vector<sdbus::Struct<uint16_t, uint16_t, std::map<std::string, sdbus::Variant>>> getVndModels(){
        return VendorModels_;
    }


private:
    /**
    * This method is called by bluetooth-meshd daemon when a message
    * arrives addressed to the application.
    * 
    * The source parameter is unicast address of the remote
    * node-element that sent the message.
    * 
    * The key_index parameter indicates which application key has been
    * used to decode the incoming message. The same key_index should
    * be used by the application when sending a response to this
    * message (in case a response is expected).
    * 
    * The destination parameter contains the destination address of
    * received message. Underlying variant types are:
    * 
    * uint16
    * 
    *     Destination is an unicast address, or a well known
    *     group address
    * 
    * array{byte}
    * 
    *     Destination is a virtual address label
    * 
    * The data parameter is the incoming message.
    */
    void MessageReceived(const uint16_t& source, const uint16_t& key_index, const sdbus::Variant& destination, const std::vector<uint8_t>& data) override
    {
        std::cout << "Message Received on element: " << Index_ << "src: " << source << "dest: " << (uint16_t) destination << std::endl;
    }

    /**
    * This method is called by meshd daemon when a message arrives
    * addressed to the application, which was sent with the remote
    * node's device key.
    *
    * The source parameter is unicast address of the remote
    * node-element that sent the message.
    *
    * The remote parameter if true indicates that the device key
    * used to decrypt the message was from the sender. False
    * indicates that the local nodes device key was used, and the
    * message has permissions to modify local states.
    *
    * The net_index parameter indicates what subnet the message was
    * received on, and if a response is required, the same subnet
    * must be used to send the response.
    *
    * The data parameter is the incoming message.
    */

    void DevKeyMessageReceived(const uint16_t& source, const bool& remote, const uint16_t& net_index, const std::vector<uint8_t>& data) override
    {
        std::cout << "DevKey MessageReceived." << std::endl;
    }

    /**
    * This method is called by bluetooth-meshd daemon when a model's
    * configuration is updated.
    * 
    * The model_id parameter contains BT SIG Model Identifier or, if
    * Vendor key is present in config dictionary, a 16-bit
    * vendor-assigned Model Identifier.
    * 
    * The config parameter is a dictionary with the following keys
    * defined:
    * 
    * array{uint16} Bindings
    * 
    *     Indices of application keys bound to the model
    * 
    * uint32 PublicationPeriod
    * 
    *     Model publication period in milliseconds
    * 
    * uint16 Vendor
    * 
    *     A 16-bit Bluetooth-assigned Company Identifier of the
    *     vendor as defined by Bluetooth SIG
    * 
    * array{variant} Subscriptions
    * 
    *     Addresses the model is subscribed to.
    * 
    *     Each address is provided either as uint16 for group
    *     addresses, or as array{byte} for virtual labels.
    */
    void UpdateModelConfiguration(const uint16_t& model_id, const std::map<std::string, sdbus::Variant>& config) override
    {
        std::cout << "Model configuration updated" << std::endl;
    }


public: 

private:

    uint8_t Index() override
    {
        return Index_;
    }

    std::vector<sdbus::Struct<uint16_t, std::map<std::string, sdbus::Variant>>> Models() override
    {
        return Models_;
    }
    std::vector<sdbus::Struct<uint16_t, uint16_t, std::map<std::string, sdbus::Variant>>> VendorModels() override
    {
        return VendorModels_;
    }
    uint16_t Location() override
    {
        return Location_;
    }

protected:
    void emitPropertyChangedSignal( const std::string& property )
    {
        PropertiesExt_adaptor::emitPropertyChangedSignal( Element1_adaptor::INTERFACE_NAME, property );
    }

    uint8_t Index_;
    std::vector<sdbus::Struct<uint16_t, std::map<std::string, sdbus::Variant>>> Models_;
    std::vector<sdbus::Struct<uint16_t, uint16_t, std::map<std::string, sdbus::Variant>>> VendorModels_;
    uint16_t Location_;

    std::string path_;
    std::shared_ptr<IConnection> connection_;
};

}}}
