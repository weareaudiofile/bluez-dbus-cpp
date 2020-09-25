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
    ProvisionAgent1( std::shared_ptr<IConnection> connection, uint16_t companyID, uint16_t productID ) :
        AdaptorInterfaces{ *connection, "/example/mesh/agent" },
        path_{ "/example/mesh/agent" },
        companyID_{ companyID },
        productID_{ productID }
    {

    }

public:
    // static std::shared_ptr<ProvisionAgent1> createWith( std::shared_ptr<IConnection> connection, std::string serviceName, uint16_t companyID, uint16_t productID );
    // std::shared_ptr<ProvisionAgent1> andRegister();
    ~ProvisionAgent1() { }

public:
    virtual const std::string& getPath() const final { return path_; }
    virtual std::shared_ptr<IConnection> getConnection() const final { return connection_; }

private:
    std::vector<uint8_t> PrivateKey() override
    {
        return std::vector<uint8_t>();
    }

    std::vector<uint8_t> PublicKey() override
    {
        return std::vector<uint8_t>();
    }

    void DisplayString(const std::string& value) override
    {
        std::cout << "DisplayString: " << value << std::endl;
    }
    
    void DisplayNumeric(const std::string& type, const uint32_t& number) override
    {
        std::cout << "DisplayNumeric: " << number << std::endl;
    }
    
    uint32_t PromptNumeric(const std::string& type) override
    {
        
    }

    std::vector<uint8_t> PromptStatic(const std::string& type) override
    {
        return std::vector<uint8_t>{ /* TODO enter 16 byte key */ };
    }

    void Cancel() override
    {
        std::cout << "Provisioning Cancel called" << std::endl;
    }

private:
    std::vector<std::string> Capabilities() override
    {
        return std::vector<std::string>{ "out-numeric", "static-oob" };
    }

    std::vector<std::string> OutOfBandInfo() override
    {
        return std::vector<std::string>{ "other" };
    }

    std::string URI() override
    {
        return "";
    }

protected:
    void emitPropertyChangedSignal( const std::string& property )
    {
        PropertiesExt_adaptor::emitPropertyChangedSignal( ProvisionAgent1_adaptor::INTERFACE_NAME, property );
    }

    std::string path_;
    std::shared_ptr<IConnection> connection_;
};

}}}
