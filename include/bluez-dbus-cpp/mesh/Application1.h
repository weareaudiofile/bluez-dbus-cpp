//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "../adaptor/PropertiesExt_adaptor.h"
#include "adaptor/Application1_adaptor.h"

#include <sdbus-c++/sdbus-c++.h>

namespace org {
namespace bluez {
namespace mesh {

using namespace sdbus;

class Application1 :
    public AdaptorInterfaces<Application1_adaptor, PropertiesExt_adaptor>,
    public std::enable_shared_from_this<Application1>
{
    Application1( const Application1& service ) = delete;

public:
    Application1( std::shared_ptr<IConnection> connection, uint16_t companyID, uint16_t productID ) :
        AdaptorInterfaces{ *connection, "/example/mesh/app" },
        path_{ "/example/mesh/app" },
        companyID_{ companyID },
        productID_{ productID }
    {

    }

public:
    // static std::shared_ptr<Application1> createWith( std::shared_ptr<IConnection> connection, std::string serviceName, uint16_t companyID, uint16_t productID );
    // std::shared_ptr<Application1> andRegister();
    ~Application1() { }

public:
    // virtual void addCharacteristic( std::shared_ptr<GattCharacteristic1> characteristic );
    // virtual void removeCharacteristic( std::shared_ptr<GattCharacteristic1> characteristic );
    virtual const std::string& getPath() const final { return path_; }
    virtual std::shared_ptr<IConnection> getConnection() const final { return connection_; }

private:
    /**
     * This method is called when the node provisioning initiated
     * by a Join() method call successfully completed.
     *
     * The token parameter serves as a unique identifier of the
     * particular node. The token must be preserved by the application
     * in order to authenticate itself to the mesh daemon and attach to
     * the network as a mesh node by calling Attach() method or
     * permanently remove the identity of the mesh node by calling
     * Leave() method.
     *
     * If this method returns an error, the daemon will assume that the
     * application failed to preserve the token, and will remove the
     * freshly created node.
     */
    void JoinComplete(const uint64_t& token) override
    {
        std::cout << "JoinComplete with token: " << token << std::endl;
    }

    void JoinFailed(const std::string& reason) override
    {
        std::cerr << "JoinFailed with reason: " << reason << std::endl;
    }

private:
    uint16_t CompanyID() override
    {
        return companyID_;
    }
    
    uint16_t ProductID() override
    {
        return productID_;
    }
    
    uint16_t VersionID() override
    {
        return versionID_;
    }
    
    uint16_t CRPL() override
    {
        return crpl_;
    }

protected:
    void emitPropertyChangedSignal( const std::string& property )
    {
        PropertiesExt_adaptor::emitPropertyChangedSignal( Application1_adaptor::INTERFACE_NAME, property );
    }

    uint16_t companyID_;
    uint16_t productID_;
    uint16_t versionID_{ 1 };
    uint16_t crpl_{ 0 };

    std::string path_;
    std::shared_ptr<IConnection> connection_;
};

}}}
