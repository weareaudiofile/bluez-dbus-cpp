//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "LEAdvertisingManager1.h"

#include <sdbus-c++/sdbus-c++.h>

namespace org {
namespace bluez {

using namespace sdbus;

class LEAdvertisement1 :
    public std::enable_shared_from_this<LEAdvertisement1>
{
    std::unique_ptr<sdbus::IObject> object_;
    std::string path_;
    std::shared_ptr<LEAdvertisingManager1> manager_;
    std::function<void()> onReleaseCallback_;
    bool registered_;
    static constexpr const char* INTERFACE_NAME = "org.bluez.LEAdvertisement1";

public:
    LEAdvertisement1( IConnection& connection, std::string objectPath )
        : object_{ createObject(connection, objectPath) },
          path_{ std::move(objectPath) }
    {
        object_->registerMethod("Release").onInterface(INTERFACE_NAME).implementedAs([this](){ return this->Release(); });
    }

    ~LEAdvertisement1()
    {
        unregister();
    }

    const std::string& getPath() const
    {
        return path_;
    }

    static LEAdvertisement1& create( IConnection& connection, std::string objectPath )
    {
        auto self = new LEAdvertisement1{ connection, std::move(objectPath) };
        return *self;
    }

    LEAdvertisement1& onReleaseCall( std::function<void()>&& callback )
    {
        onReleaseCallback_ = callback;
        return *this;
    }

    std::shared_ptr<LEAdvertisement1> registerWith( std::shared_ptr<LEAdvertisingManager1> manager, std::function<void(const sdbus::Error*)> onRegistrationCallback = nullptr )
    {
        // make sure we at least have "Type"
        if( !type_.length() )
            withType( "peripheral" );

        if( !manager )
            throw std::invalid_argument("'manager' must exist");

        object_->finishRegistration();

        manager_ = std::move(manager);

        if( onRegistrationCallback )
            manager_->RegisterAdvertisementAsync( getPath() ).uponReplyInvoke( onRegistrationCallback );
        else
            manager_->RegisterAdvertisement( getPath() );

        registered_ = true;

        auto shared = std::shared_ptr<LEAdvertisement1>( this );
        return shared;
    }

    void unregister()
    {
        if( registered_ )
        {
            manager_->UnregisterAdvertisement( getPath() );
            registered_ = false;
        }
        object_->unregister();
    }

public:
    /**
     * @brief Determines the type of advertising packet requested.
     * 
     * @param type Possible values: "broadcast" or "peripheral"
     */
    LEAdvertisement1& withType( std::string type )
    {
        type_ = std::move(type);
        object_->registerProperty("Type").onInterface(INTERFACE_NAME).withGetter([this](){ return this->Type(); });
        return *this;
    }

    /**
     * @brief List of UUIDs to include in the "Service UUID" field of the Advertising Data.
     */
    LEAdvertisement1& withServiceUUIDs( std::vector<std::string> uuids )
    {
        serviceUUIDs_ = std::move( uuids );
        object_->registerProperty("ServiceUUIDs").onInterface(INTERFACE_NAME).withGetter([this](){ return this->ServiceUUIDs(); });
        return *this;
    }

    /**
     * @brief Service Data elements to include. The keys are the UUID to associate with the data.
     */
    LEAdvertisement1& withServiceData( std::map<std::string, sdbus::Variant> data )
    {
        serviceData_ = std::move(data);
        object_->registerProperty("ServiceData").onInterface(INTERFACE_NAME).withGetter([this](){ return this->ServiceData(); });
        return *this;
    }

    /**
     * @brief Manufacturer Data fields to include in the Advertising Data. Keys are the Manufacturer ID to associate with the data.
     */
    LEAdvertisement1& withManufacturerData( std::map<uint16_t, sdbus::Variant> manufacturerData )
    {
        manufacturerData_ = std::move(manufacturerData);
        object_->registerProperty("ManufacturerData").onInterface(INTERFACE_NAME).withGetter([this](){ return this->ManufacturerData(); });
        return *this;
    }

    /**
     * @brief   Advertising Type to include in the Advertising Data.
     *          Key is the advertising type and value is the data as byte array.
     *
     * @note    Types already handled by other properties shall not be used.
     *
     *          Possible values:
     *              <type> <byte array>
     *              ...
     *
     *          Example:
     *              <Transport Discovery> <Organization Flags...>
     *              0x26                   0x01         0x01...
     */
    LEAdvertisement1& withData( std::map<uint8_t, sdbus::Variant> data )
    {
        data_ = std::move(data);
        object_->registerProperty("Data").onInterface(INTERFACE_NAME).withGetter([this](){ return this->Data(); });
        return *this;
    }

    /**
     * @brief Array of UUIDs to include in "Service Solicitation" Advertisement Data.
     */
    LEAdvertisement1& withSolicitUUIDs( const std::vector<std::string>& uuids )
    {
        solicitUUIDs_ = std::move(uuids);
        object_->registerProperty("SolicitUUIDs").onInterface(INTERFACE_NAME).withGetter([this](){ return this->SolicitUUIDs(); });
        return *this;
    }

    /**
     * @brief Advertise as general discoverable. When present this will adapter Discoverable property.
     * 
     * @note This property shall not be set when Type is set to broadcast.
     */
    LEAdvertisement1& withDiscoverable( bool discoverable )
    {
        discoverable_ = discoverable;
        object_->registerProperty("Discoverable").onInterface(INTERFACE_NAME).withGetter([this](){ return this->Discoverable(); });
        return *this;
    }

    /**
     * @brief The discoverable timeout in seconds. A value of zero means that the
     *        timeout is disabled and it will stay in discoverable/limited mode forever.
     * @note  This property shall not be set when Type is set to broadcast.
     */
    LEAdvertisement1& withDiscoverableTimeout( uint16_t timeout )
    {
        discoverableTimeout_ = true;
        object_->registerProperty("DiscoverableTimeout").onInterface(INTERFACE_NAME).withGetter([this](){ return this->DiscoverableTimeout(); });
        return *this;
    }

    /**
     * @brief List of features to be included in the advertising packet.
     *        Possible values: as found on LEAdvertisingManager.SupportedIncludes
     */
    LEAdvertisement1& withIncludes( std::vector<std::string> includes )
    {
        includes_ = std::move(includes);
        object_->registerProperty("Includes").onInterface(INTERFACE_NAME).withGetter([this](){ return this->Includes(); });
        return *this;
    }

    /**
     * @brief Local name to be used in the advertising report.
     *        If the string is too big to fit into the packet it will be truncated.
     * @note  If this property is available 'local-name' cannot be present in the Includes.
     */
    LEAdvertisement1& withLocalName( std::string name )
    {
        if( name.size() )
        {
            localName_ = std::move(name);
            object_->registerProperty("LocalName").onInterface(INTERFACE_NAME).withGetter([this](){ return this->LocalName(); });
        }
        return *this;
    }

    /**
     * @brief Appearance to be used in the advertising report.
     *        Possible values: as found on GAP Service.
     */
    LEAdvertisement1& withAppearance( uint16_t appearance )
    {
        appearance_ = appearance;
        object_->registerProperty("Appearance").onInterface(INTERFACE_NAME).withGetter([this](){ return this->Appearance(); });
        return *this;
    }

    /**
     * @brief Duration of the advertisement in seconds. If there are other
     *        applications advertising no duration is set the default is 2 seconds.
     */
    LEAdvertisement1& withDuration( uint16_t duration )
    {
        duration_ = duration;
        object_->registerProperty("Duration").onInterface(INTERFACE_NAME).withGetter([this](){ return this->Duration(); });
        return *this;
    }

    /**
     * @brief Timeout of the advertisement in seconds. This defines the lifetime
     *        of the advertisement.
     */
    LEAdvertisement1& withTimeout( uint16_t timeout )
    {
        timeout_ = timeout;
        object_->registerProperty("Timeout").onInterface(INTERFACE_NAME).withGetter([this](){ return this->Timeout(); });
        return *this;
    }

    /**
     * @brief Secondary channel to be used. Primary channel is
	 *	      always set to "1M" except when "Coded" is set.
     *
	 *	      Possible value: "1M" (default)
	 *	                      "2M"
	 *	                      "Coded"
     */
    LEAdvertisement1& withSecondaryChannel( std::string secondaryChannel )
    {
        secondaryChannel_= secondaryChannel;
        object_->registerProperty("SecondaryChannel").onInterface(INTERFACE_NAME).withGetter([this](){ return this->SecondaryChannel(); });
        return *this;
    }

protected:
    std::string Type() { return type_; }
    std::vector<std::string> ServiceUUIDs() { return serviceUUIDs_; }
    std::map<std::string, sdbus::Variant> ServiceData() { return serviceData_; }
    std::map<uint16_t, sdbus::Variant> ManufacturerData() { return manufacturerData_; }
    std::map<uint8_t, sdbus::Variant> Data() { return data_; }
    std::vector<std::string> SolicitUUIDs() { return solicitUUIDs_; }
    bool Discoverable() { return discoverable_; }
    uint16_t DiscoverableTimeout() { return discoverableTimeout_; }
    std::vector<std::string> Includes() { return includes_; }
    std::string LocalName() { return localName_; }
    uint16_t Appearance() { return appearance_; }
    uint16_t Duration() { return duration_; }
    uint16_t Timeout() { return timeout_; }
    std::string SecondaryChannel() { return secondaryChannel_; }

    void Release()
    {
        registered_ = false;
        if( onReleaseCallback_ )
        {
            onReleaseCallback_();
        }
    }

    std::string type_;
    std::vector<std::string> serviceUUIDs_;
    std::map<std::string, sdbus::Variant> serviceData_;
    std::map<uint16_t, sdbus::Variant> manufacturerData_;
    std::map<uint8_t, sdbus::Variant> data_;
    std::vector<std::string> solicitUUIDs_;
    bool discoverable_;
    uint16_t discoverableTimeout_;
    std::vector<std::string> includes_;
    std::string localName_;
    uint16_t appearance_;
    uint16_t duration_;
    uint16_t timeout_;
    std::string secondaryChannel_;
};

}}
