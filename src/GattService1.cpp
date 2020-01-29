//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#include "bluez/GattService1.h"
#include "bluez/GattApplication1.h"

namespace org {
namespace bluez {

static std::string makePath( const std::string& characteristicPath, const std::string& name )
{
    std::string path{ characteristicPath };
    path.append( "/" ).append( name );
    return path;
}

// ---- Creation ---------------------------------------------------------------

// std::shared_ptr<GattService1> GattService1::createWith( std::shared_ptr<GattApplication1> app, std::string serviceName, std::string serviceUUID )
// {
//     auto service = std::shared_ptr<GattService1>{ new GattService1{ std::move(app), std::move(serviceName), std::move(serviceUUID) }};
//     return service;
// }

// std::shared_ptr<GattService1> GattService1::andRegister()
// {
//     app_->addService( shared_from_this() );
//     return shared_from_this();
// }

GattService1::GattService1( std::shared_ptr<GattApplication1> app, std::string serviceName, std::string serviceUUID )
    : AdaptorInterfaces{ *(app->getConnection()), makePath( app->getPath(), serviceName ) },
      app_{ app },
      path_{ makePath( app->getPath(), serviceName ) },
      uuid_{ std::move(serviceUUID) },
      primary_{ true }
{
    registerAdaptor();
    std::shared_ptr<GattService1> ptr(this);
    app_->addService( ptr );
}

GattService1::~GattService1()
{
    app_->removeService( shared_from_this() );
    unregisterAdaptor();
}

void GattService1::addCharacteristic( std::shared_ptr<GattCharacteristic1> characteristic )
{
    const std::string& chrcPath = characteristic->getPath();

    for( auto path : includes_ )
    {
        if( path == chrcPath )
            return; // already registered
    }

    characteristics_.push_back( std::move(characteristic) );
    includes_.push_back( sdbus::ObjectPath{ chrcPath } );
}

void GattService1::removeCharacteristic( std::shared_ptr<GattCharacteristic1> characteristic )
{
    const std::string& chrcPath = characteristic->getPath();

    for( auto iter{ characteristics_.cbegin() }; iter != characteristics_.cend(); iter++ )
    {
        if( *iter == characteristic )
        {
            characteristics_.erase( iter );
            break;
        }
    }

    for( auto iter{ includes_.cbegin() }; iter != includes_.cend(); iter++ )
    {
        if( *iter == chrcPath )
        {
            includes_.erase( iter );
            break;
        }
    }
}

const std::string& GattService1::getPath() const
{
    return path_;
}

std::shared_ptr<IConnection> GattService1::getConnection() const
{
    return app_->getConnection();
}

int GattService1::nextCharacteristicIndex() const
{
    return includes_.size();
}

}}
