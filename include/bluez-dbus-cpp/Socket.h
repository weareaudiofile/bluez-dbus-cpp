//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <sdbus-c++/sdbus-c++.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstring>

namespace org {
namespace bluez {

class Client;

/**
 * @brief Interface class for Socket pair for "acquire" functions
 *
 *        write an implementation using the socket listener that best suits
 *        your project
 * 
 * see https://github.com/bluez/bluez/blob/82714e3b64f2e240bc7a32b671fc9f0f88313a21/client/gatt.c#L2293
 * for how this is done in BlueZ own commandline client
 */
class Socket
{
public:
    using DisconnectCallback = std::function<void(std::shared_ptr<Client>)>;
    using DataCallback = std::function<void(std::shared_ptr<Client>,const std::vector<uint8_t>&)>;

protected:
    Socket(
            bool writeDirection,
            uint16_t mtu,
            std::shared_ptr<Client> client,
            DisconnectCallback disconn_cb,
            DataCallback data_cb = nullptr) :
        writeDirection_{ writeDirection },
        mtu_{ mtu },
        client_{ client },
        disconnect_cb_{ disconn_cb },
        data_cb_{ data_cb }
    {      

    }

public:
    virtual ~Socket()
    {
        close(fileDescriptors_[0]);
        close(fileDescriptors_[1]);
    }

    virtual void send( std::vector<uint8_t> data ) = 0;

    int getRemoteFileDescriptor() const
    {
        return fileDescriptors_[writeDirection_];
    }

    int getLocalFileDescriptor() const
    {
        return fileDescriptors_[!writeDirection_];
    }

    std::tuple<sdbus::UnixFd, uint16_t> acquireResponse()
    {
        return std::make_tuple( sdbus::UnixFd( getRemoteFileDescriptor() ), mtu_ );
    }

protected:
    static void createSocketPair( bool writeDirection, int (&fds)[2] )
    {
        if ( socketpair(AF_LOCAL, SOCK_SEQPACKET | SOCK_NONBLOCK | SOCK_CLOEXEC, 0, fds) < 0 )
        {
            throw sdbus::Error( "org.bluez.Error.Failed", strerror(errno) );
        }
    }

protected:
    bool writeDirection_;
    int fileDescriptors_[2];
    uint16_t mtu_;
    std::shared_ptr<Client> client_;
    DisconnectCallback disconnect_cb_;
    DataCallback data_cb_;
};

}}
