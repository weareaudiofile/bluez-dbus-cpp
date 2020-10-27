//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <sdbus-c++/sdbus-c++.h>
#include <bluez-dbus-cpp/Socket.h>

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include <ell/io.h>

namespace org {
namespace bluez {

namespace ell {
namespace io {
    bool read_handler( struct l_io* io, void* user_data );
    void disconnect_handler( struct l_io* io, void* user_data );
}}

/**
 * @brief Socket implementation using the 'ell' library to watch the file descriptor
 * 
 */
class EllSocket : public Socket
{
public:
    EllSocket(
            bool writeDirection,
            std::shared_ptr<Client> client,
            Socket::DisconnectCallback disconn_cb,
            Socket::DataCallback data_cb = nullptr ) :
        Socket{ writeDirection, client->getMtu(), client, disconn_cb, data_cb }
    {
        createSocketPair( writeDirection, fileDescriptors_ );

        l_io_ = l_io_new( getLocalFileDescriptor() );
        if(!l_io_)
            throw sdbus::Error( "org.bluez.Error.Failed", strerror(EIO) );

        if (!l_io_set_close_on_destroy(l_io_, true))
            throw sdbus::Error( "org.bluez.Error.Failed", strerror(errno) );

        if (!l_io_set_disconnect_handler(l_io_, ell::io::disconnect_handler, this, NULL))
            throw sdbus::Error( "org.bluez.Error.Failed", strerror(errno) );

        if( writeDirection )
        {
            buffer_.reserve( client_->getMtu() );
            l_io_set_read_handler( l_io_, ell::io::read_handler, this, NULL );
        }
    }

    void send( std::vector<uint8_t> data ) override
    {
        struct msghdr msg;
        struct iovec iov;

        if( data.size() > mtu_ )
            throw sdbus::Error( "org.bluez.Error.Failed", "EllSocket::write() data.size() exeeds MTU" );

        iov.iov_base = data.data();
        iov.iov_len= data.size();

        memset(&msg, 0, sizeof(msg));
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;

        int ret = sendmsg( getLocalFileDescriptor(), &msg, MSG_NOSIGNAL);
        if (ret < 0)
            throw sdbus::Error( "org.bluez.Error.Failed", strerror(errno) );
    }

    bool read_handler()
    {
        struct msghdr msg;
        struct iovec iov;
        uint8_t buffer[ mtu_ ];
        
        iov.iov_base = buffer;
        iov.iov_len= sizeof(buffer);

        memset(&msg, 0, sizeof(msg));
	    msg.msg_iov = &iov;
	    msg.msg_iovlen = 1;

        ssize_t bytes_read = recvmsg( getLocalFileDescriptor(), &msg, MSG_DONTWAIT);
        if( bytes_read < 0 )
        {
            std::cerr << "EllSocket::read_handler() ERROR: " << strerror(errno) << std::endl;
            return false;
        }

        if( data_cb_ != nullptr )
        {
            buffer_.assign( buffer, buffer + bytes_read );
            data_cb_( client_, buffer_ );
        }

        return true;
    }

    void disconnect_handler()
    {
        if( disconnect_cb_ != nullptr )
        {
            disconnect_cb_( client_ );
        }
    }

protected:
    bool writeDirection_;
    std::vector<uint8_t> buffer_;
    struct l_io* l_io_;
};

namespace ell {
namespace io {
    bool read_handler( struct l_io* io, void* user_data )
    {
        EllSocket* sock = (EllSocket*) user_data;
        return sock->read_handler();
    }
    void disconnect_handler( struct l_io* io, void* user_data )
    {
        std::cout << "ell::io::disconnect_handler" << std::endl;
        EllSocket* sock = (EllSocket*) user_data;
        return sock->disconnect_handler();
    }
}}

}}
