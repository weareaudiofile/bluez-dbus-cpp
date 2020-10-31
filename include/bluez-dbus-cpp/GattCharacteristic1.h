//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "adaptor/PropertiesExt_adaptor.h"
#include "adaptor/GattCharacteristic1_adaptor.h"
#include "GattDescriptor1.h"

#include <sdbus-c++/sdbus-c++.h>
#include "Util.h"

#include <iostream>

namespace org {
namespace bluez {

// Forward declarations
class GattService1;

using namespace sdbus;

class GattCharacteristic1 :
    public AdaptorInterfaces<GattCharacteristic1_adaptor, PropertiesExt_adaptor>,
    public std::enable_shared_from_this<GattCharacteristic1>
{
    static constexpr const char* INTERFACE_NAME = "org.bluez.GattCharacteristic1";

public:
    GattCharacteristic1( std::shared_ptr<GattService1> service,
        std::string uuid,
        bool hasAcquireWrite = false,
        bool hasAcquireNotify = false,
        bool hasValue = false,
        bool valueIsDirected = false);
    virtual ~GattCharacteristic1();

    void addValue( std::string value );
    void addValue( std::vector<uint8_t> value );
    void addFlag( std::string flag );

protected:
    friend GattDescriptor1;
    friend GattService1;
    void enableAcquireWrite(void);
    void enableAcquireNotify(void);
    virtual void addDescriptor( std::shared_ptr<GattDescriptor1> descriptor );
    virtual void removeDescriptor( std::shared_ptr<GattDescriptor1> descriptor );
    const std::string& getPath() const;
    const std::shared_ptr<IConnection> getConnection() const;

protected:
    /**
     * Issues a request to read the value of the
     * characteristic and returns the value if the
     * operation was successful.
     *
     * Possible options: "offset": uint16 offset
     *          "device": Object Device (Server only)
     *
     * Possible Errors: org.bluez.Error.Failed
     *          org.bluez.Error.InProgress
     *          org.bluez.Error.NotPermitted
     *          org.bluez.Error.NotAuthorized
     *          org.bluez.Error.InvalidOffset
     *          org.bluez.Error.NotSupported
     * 
     */
    virtual std::vector<uint8_t> ReadValue(const std::map<std::string, sdbus::Variant>& options);

    /**
     * Issues a request to write the value of the
     * characteristic.
     *
     * Possible options: "offset": Start offset
     *             "device": Device path (Server only)
     *             "link": Link type (Server only)
     *             "prepare-authorize": boolean Is prepare
     *                         authorization
     *                         request
     *
     * Possible Errors: org.bluez.Error.Failed
     *             org.bluez.Error.InProgress
     *             org.bluez.Error.NotPermitted
     *             org.bluez.Error.InvalidValueLength
     *             org.bluez.Error.NotAuthorized
     *             org.bluez.Error.NotSupported
     */
    virtual void WriteValue(const std::vector<uint8_t>& value, const std::map<std::string, sdbus::Variant>& options);

    /**
     * Acquire file descriptor and MTU for writing. Usage of
     * WriteValue will be locked causing it to return
     * NotPermitted error.
     *
     * For server the MTU returned shall be equal or smaller
     * than the negotiated MTU.
     *
     * For client it only works with characteristic that has
     * WriteAcquired property which relies on
     * write-without-response Flag.
     *
     * To release the lock the client shall close the file
     * descriptor, a HUP is generated in case the device
     * is disconnected.
     *
     * Note: the MTU can only be negotiated once and is
     * symmetric therefore this method may be delayed in
     * order to have the exchange MTU completed, because of
     * that the file descriptor is closed during
     * reconnections as the MTU has to be renegotiated.
     *
     * Possible options: "device": Object Device (Server only)
     *                   "MTU": Exchanged MTU (Server only)
     *                   "link": Link type (Server only)
     *
     * Possible Errors: org.bluez.Error.Failed
     *                  org.bluez.Error.NotSupported
     * 
     * @param options 
     */
    virtual std::tuple<sdbus::UnixFd, uint16_t> AcquireWrite(const std::map<std::string, sdbus::Variant>& options);

    /**
     * Acquire file descriptor and MTU for notify. Usage of
     * StartNotify will be locked causing it to return
     * NotPermitted error.
     *
     * For server the MTU returned shall be equal or smaller
     * than the negotiated MTU.
     *
     * Only works with characteristic that has NotifyAcquired
     * which relies on notify Flag and no other client have
     * called StartNotify.
     *
     * Notification are enabled during this procedure so
     * StartNotify shall not be called, any notification
     * will be dispatched via file descriptor therefore the
     * Value property is not affected during the time where
     * notify has been acquired.
     *
     * To release the lock the client shall close the file
     * descriptor, a HUP is generated in case the device
     * is disconnected.
     *
     * Note: the MTU can only be negotiated once and is
     * symmetric therefore this method may be delayed in
     * order to have the exchange MTU completed, because of
     * that the file descriptor is closed during
     * reconnections as the MTU has to be renegotiated.
     *
     * Possible options: "device": Object Device (Server only)
     *                   "MTU": Exchanged MTU (Server only)
     *                   "link": Link type (Server only)
     *
     * Possible Errors: org.bluez.Error.Failed
     *                  org.bluez.Error.NotSupported
     * 
     */
    virtual std::tuple<sdbus::UnixFd, uint16_t> AcquireNotify(const std::map<std::string, sdbus::Variant>& options);

    /**
     * Starts a notification session from this characteristic
     * if it supports value notifications or indications.
     *
     * Possible Errors: org.bluez.Error.Failed
     *             org.bluez.Error.NotPermitted
     *             org.bluez.Error.InProgress
     *             org.bluez.Error.NotSupported
     * 
     */
    virtual void StartNotify(const std::map<std::string, sdbus::Variant>& options);

    /**
     * This method will cancel any previous StartNotify
     * transaction. Note that notifications from a
     * characteristic are shared between sessions thus
     * calling StopNotify will release a single session.
     *
     * Possible Errors: org.bluez.Error.Failed
     */
    virtual void StopNotify(const std::map<std::string, sdbus::Variant>& options);

    /**
     * This method doesn't expect a reply so it is just a
     * confirmation that value was received.
     *
     * Possible Errors: org.bluez.Error.Failed
     */
    virtual void Confirm();

protected:
    std::string UUID() override { return uuid_; }
    sdbus::ObjectPath Service() override;
    std::vector<uint8_t> Value() override { return value_; }
    std::map<sdbus::ObjectPath, std::vector<std::vector<uint8_t>>> DirectedValue() override;
    bool Notifying() override { return notifyingSessions_ > 0; }
    std::vector<std::string> Flags() override { return flags_; }
    std::vector<sdbus::ObjectPath> Descriptors() override { return includes_; }
    bool WriteAcquired() override { return writeAcquired_; }
    bool NotifyAcquired() override { return notifyAcquired_; }

protected:
    void emitPropertyChangedSignal( const std::string& property );
    void registerWithService( std::shared_ptr<GattCharacteristic1> self );

    std::map<std::string,sdbus::Variant> dict_;
    std::shared_ptr<GattService1> service_;
    std::string path_;
    std::string uuid_;
    std::vector<uint8_t> value_;
    int notifyingSessions_;
    std::vector<std::string> flags_;
    bool writeAcquired_;
    bool notifyAcquired_;

    std::vector<std::shared_ptr<GattDescriptor1>> descriptors_;
    std::vector<sdbus::ObjectPath> includes_;
};



}}
