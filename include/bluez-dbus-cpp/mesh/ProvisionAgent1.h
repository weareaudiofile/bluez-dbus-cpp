//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "../adaptor/PropertiesExt_adaptor.h"
#include "adaptor/ProvisionAgent1_adaptor.h"

#include <sdbus-c++/sdbus-c++.h>

namespace org {
namespace bluez {
namespace mesh {

using namespace sdbus;

class ProvisionAgent1 :
    public AdaptorInterfaces<ProvisionAgent1_adaptor, PropertiesExt_adaptor>,
    public std::enable_shared_from_this<ProvisionAgent1>
{
    ProvisionAgent1( const ProvisionAgent1& service ) = delete;

public:
    ProvisionAgent1( std::shared_ptr<IConnection> connection) :
        AdaptorInterfaces{ *connection, "/example/mesh/agent"},
        path_{ "/example/mesh/agent" }
    {
        
    }

public:
    // static std::shared_ptr<ProvisionAgent1> createWith( std::shared_ptr<IConnection> connection, std::string serviceName);
    // std::shared_ptr<ProvisionAgent1> andRegister();
    ~ProvisionAgent1() { }

public:
    virtual const std::string& getPath() const final { return path_; }
    virtual std::shared_ptr<IConnection> getConnection() const final { return connection_; }

private:
    /**
	*	This method is called during provisioning if the Provisioner
	*	has requested Out-Of-Band ECC key exchange. The Private key is
	*	returned to the Daemon, and the Public Key is delivered to the
	*	remote Provisioner using a method that does not involve the
	*	Bluetooth Mesh system. The Private Key returned must be 32
	*	octets in size, or the Provisioning procedure will fail and be
	*	canceled.
    *
	*	This function will only be called if the Provisioner has
	*	requested pre-determined keys to be exchanged Out-of-Band, and
	*	the local role is Unprovisioned device.
    */
    std::vector<uint8_t> PrivateKey() override
    {
        ;
    }

    /**

    */
    std::vector<uint8_t> PublicKey() override
    {
        ;
    }

    /**

    */
    void DisplayString(const std::string& value) override
    {
        ;
    }

    /**
    *   This method is called when the Daemon has something important
	*	for the Agent to Display, but does not require any additional
	*	input locally. For instance: "Enter 14939264 on remote device".
    *
	*	The type parameter indicates the display method. Allowed values
	*	are:
	*		"blink" - Locally blink LED
	*		"beep" - Locally make a noise
	*		"vibrate" - Locally vibrate
	*		"out-numeric" - Display value to enter remotely
	*		"push" - Request pushes on remote button
	*		"twist" - Request twists on remote knob
    *
	*	The number parameter is the specific value represented by the
	*	Prompt.
    *
    */
    void DisplayNumeric(const std::string& type, const uint32_t& number) override
    {
        std::cout << "Display Numeric( " << type << " ) number = " << number << std::endl;
    }

    /**

    */
    uint32_t PromptNumeric(const std::string& type) override
    {
        ;
    }

    /**
    *   This method is called when the Daemon requires a 16 octet byte
	*	array, as an Out-of-Band authentication.
    *
	*	The type parameter indicates the input method. Allowed values
	*	are:
	*		"static-oob" - return 16 octet array
	*		"in-alpha" - return 16 octet alpha array
    *
	*	The Static data returned must be 16 octets in size, or the
	*	Provisioning procedure will fail and be canceled. If input type
	*	is "in-alpha", the printable characters should be
	*	left-justified, with trailing 0x00 octets filling the remaining
	*	bytes.
    *
    */
    std::vector<uint8_t> PromptStatic(const std::string& type) override
    {
        std::vector<uint8_t> StaticKey = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        std::cout << "Prompt Static( " << type << " ) " << std::endl;
        std::cout << "Enter 16 octet key on remote device: ";
        for (uint8_t x: StaticKey){
            std::cout << x <<" ";
        }
        std::cout << std::endl;

        return StaticKey;
    }

    /**
    *
    */
    void Cancel() override
    {
       std::cout << "Cancel" << std::endl;
    }

   
private:
    std::vector<std::string> Capabilities() override
    {
        return Capabilities_;
    }

    std::vector<std::string> OutOfBandInfo() override 
    {
        return OutOfBandInfo_;
    }

    std::string URI() override 
    {
        return URI_;
    }

protected:
    void emitPropertyChangedSignal( const std::string& property )
    {
        PropertiesExt_adaptor::emitPropertyChangedSignal( ProvisionAgent1_adaptor::INTERFACE_NAME, property );
    }

    std::vector<std::string> Capabilities_;
    std::vector<std::string> OutOfBandInfo_;
    std::string URI_;

    std::string path_;
    std::shared_ptr<IConnection> connection_;
};

}}}
