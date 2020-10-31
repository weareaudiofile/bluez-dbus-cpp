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

GattCharacteristic1::GattCharacteristic1(
        std::shared_ptr<GattService1> service,
        std::string uuid,
        bool hasAcquireWrite,
        bool hasAcquireNotify,
        bool hasValue,
        bool valueIsDirected )
    : AdaptorInterfaces{ *(service->getConnection()), makePath( *service ) },
      service_{ service },
      path_{ makePath( *service ) },
      uuid_{ std::move(uuid) },
      value_{},
      notifyingSessions_{ 0 },
      flags_{},
      writeAcquired_{ false },
      notifyAcquired_{ false }
{
    if( hasAcquireWrite )
    {
        getCharacteristicObject().registerMethod("AcquireWrite").onInterface(INTERFACE_NAME).implementedAs([this](const std::map<std::string, sdbus::Variant>& options){ return this->AcquireWrite(options); });    
        getCharacteristicObject().registerProperty("WriteAcquired").onInterface(INTERFACE_NAME).withGetter([this](){ return this->WriteAcquired(); });
    }
    if( hasAcquireNotify )
    {
        getCharacteristicObject().registerMethod("AcquireNotify").onInterface(INTERFACE_NAME).implementedAs([this](const std::map<std::string, sdbus::Variant>& options){ return this->AcquireNotify(options); });
        getCharacteristicObject().registerProperty("NotifyAcquired").onInterface(INTERFACE_NAME).withGetter([this](){ return this->NotifyAcquired(); });
    }
    if( hasValue )
    {
        if( valueIsDirected )
        {
            getCharacteristicObject().registerProperty("DirectedValue").onInterface(INTERFACE_NAME).withGetter([this](){ return this->DirectedValue(); }); 
        }
        else
        {
            getCharacteristicObject().registerProperty("Value").onInterface(INTERFACE_NAME).withGetter([this](){ return this->Value(); });       
        }
    }
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
    return value_;
}

void GattCharacteristic1::WriteValue(const std::vector<uint8_t>& value, const std::map<std::string, sdbus::Variant>& options)
{
    value_ = value;
}

std::map<sdbus::ObjectPath, std::vector<std::vector<uint8_t>>> GattCharacteristic1::DirectedValue()
{
    throw sdbus::Error("org.bluez.Error.NotSupported", "Property 'GattCharacteristic1::DirectedValue' not overloaded!");
}

std::tuple<sdbus::UnixFd, uint16_t> GattCharacteristic1::AcquireWrite(const std::map<std::string, sdbus::Variant>& options)
{
    throw sdbus::Error("org.bluez.Error.NotSupported", "Method 'GattCharacteristic1::AcquireWrite' not overloaded!");
}

std::tuple<sdbus::UnixFd, uint16_t> GattCharacteristic1::AcquireNotify(const std::map<std::string, sdbus::Variant>& options)
{
    throw sdbus::Error("org.bluez.Error.NotSupported", "Method 'GattCharacteristic1::AcquireNotify' not overloaded!");
}

void GattCharacteristic1::StartNotify(const std::map<std::string, sdbus::Variant>& options)
{
    notifyingSessions_ += 1;
}

void GattCharacteristic1::StopNotify(const std::map<std::string, sdbus::Variant>& options)
{
    notifyingSessions_ -= 1;
}

void GattCharacteristic1::Confirm()
{
    std::cerr << "WARNING: Method 'GattCharacteristic1::Confirm' default handler" << std::endl;
}

// ---- for Friends ------------------------------------------------------------
void GattCharacteristic1::addDescriptor( std::shared_ptr<GattDescriptor1> descriptor )
{
    const std::string& descPath = descriptor->getPath();

    for( auto path : includes_ )
    {
        if( path == descPath )
            throw std::invalid_argument(std::string("GattCharacteristic1::addDescriptor '") + descPath + std::string("' already registered!"));
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
