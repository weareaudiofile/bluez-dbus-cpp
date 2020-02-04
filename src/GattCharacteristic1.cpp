//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#include "bluez-dbus-cpp/GattCharacteristic1.h"
#include "bluez-dbus-cpp/GattService1.h"

#include <set>
#include <cstring>

namespace org {
namespace bluez {

static std::string makePath( const GattService1& service )
{
    std::string path{ service.getPath() };
    path.append( "/char" ).append( std::to_string( service.nextCharacteristicIndex() ) );
    return path;
}

GattCharacteristic1::GattCharacteristic1( std::shared_ptr<GattService1> service, std::string uuid )
    : AdaptorInterfaces{ *(service->getConnection()), makePath( *service ) },
      service_{ service },
      path_{ makePath( *service ) },
      uuid_{ std::move(uuid) },
      value_{},
      notifying_{ false },
      flags_{},
      writeAcquired_{ false },
      notifyAcquired_{ false }
{
}

GattCharacteristic1::~GattCharacteristic1()
{
    service_->removeCharacteristic( shared_from_this() );
    unregisterAdaptor();
}

// ---- Add Properties ---------------------------------------------------------

void GattCharacteristic1::addValue( std::string value )
{
    value_.assign( value.begin(), value.end() );
}

void GattCharacteristic1::addValue( std::vector<uint8_t> value )
{
    value_ = move(value);
}

void GattCharacteristic1::addFlag( std::string flag )
{
    static const std::set<std::string> valid{
        "broadcast",
        "read",
        "write-without-response",
        "write",
        "notify",
        "indicate",
        "authenticated-signed-writes",
        "reliable-write",
        "writable-auxiliaries",
        "encrypt-read",
        "encrypt-write",
        "encrypt-authenticated-read",
        "encrypt-authenticated-write",
        "secure-read", // (Server only)
        "secure-write", // (Server only)
        "authorize"
    };

    if( valid.find( flag ) == valid.end() )
        throw std::invalid_argument("'flag' not valid");

    flags_.push_back( std::move(flag) );
}

// ---- Default Handlers -------------------------------------------------------

std::vector<uint8_t> GattCharacteristic1::ReadValue(const std::map<std::string, sdbus::Variant>& options)
{
    std::cout << "Method 'ReadValue' called: " << Util::optionsListToString( options ) << std::endl;
    return value_;
}

void GattCharacteristic1::WriteValue(const std::vector<uint8_t>& value, const std::map<std::string, sdbus::Variant>& options)
{
    std::cout << "Method 'WriteValue' called: " << Util::optionsListToString( options ) << std::endl;
    value_ = value;
}

void GattCharacteristic1::AcquireWrite(const std::map<std::string, sdbus::Variant>& options)
{
    std::cout << "Method 'AcquireWrite' called: " << Util::optionsListToString( options ) << std::endl;
}

void GattCharacteristic1::AcquireNotify(const std::map<std::string, sdbus::Variant>& options)
{
    std::cout << "AcquireNotify: " << Util::optionsListToString( options ) << std::endl;
}

void GattCharacteristic1::StartNotify()
{
    notifying_ = true;
    std::cerr << "Method 'StartNotify' default handler" << std::endl;
}

void GattCharacteristic1::StopNotify()
{
    notifying_ = false;
    std::cerr << "Method 'StopNotify' default handler" << std::endl;
}

void GattCharacteristic1::Confirm()
{
    std::cerr << "Method 'Confirm' default handler" << std::endl;
}

// ---- for Friends ------------------------------------------------------------
void GattCharacteristic1::addDescriptor( std::shared_ptr<GattDescriptor1> descriptor )
{
    const std::string& descPath = descriptor->getPath();

    for( auto path : includes_ )
    {
        if( path == descPath )
            return; // already registered
    }

    descriptors_.push_back( std::move(descriptor) );
    includes_.push_back( sdbus::ObjectPath{ descPath } );
}

void GattCharacteristic1::removeDescriptor( std::shared_ptr<GattDescriptor1> descriptor )
{
    const std::string& descPath = descriptor->getPath();

    for( auto iter{ includes_.cbegin() }; iter != includes_.cend(); iter++ )
    {
        if( *iter == descPath )
        {
            includes_.erase( iter );
            break;
        }
    }

    for( auto iter{ descriptors_.cbegin() }; iter != descriptors_.cend(); iter++ )
    {
        if( *iter == descriptor )
        {
            descriptors_.erase( iter );
            break;
        }
    }
}

const std::string& GattCharacteristic1::getPath() const
{
    return path_;
}

const std::shared_ptr<IConnection> GattCharacteristic1::getConnection() const
{
    return service_->getConnection();
}

sdbus::ObjectPath GattCharacteristic1::Service()
{
    return service_->getPath();
}

void GattCharacteristic1::emitPropertyChangedSignal( const std::string& property )
{
    PropertiesExt_adaptor::emitPropertyChangedSignal( INTERFACE_NAME, property );
}

void GattCharacteristic1::registerWithService( std::shared_ptr<GattCharacteristic1> self )
{
    registerAdaptor();
    service_->addCharacteristic( self );
}

}}
