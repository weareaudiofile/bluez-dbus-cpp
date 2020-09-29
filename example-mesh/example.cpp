//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#include <bluez-dbus-cpp/bluez.h>
#include <bluez-dbus-cpp/mesh/Network1.h>
#include <bluez-dbus-cpp/mesh/Application1.h>
#include <bluez-dbus-cpp/mesh/Element1.h>
#include <bluez-dbus-cpp/mesh/ProvisionAgent1.h>

#include <iostream>
#include <signal.h>

using namespace org::bluez;
using namespace org::bluez::mesh;

constexpr const char* BLUEZ_SERVICE = "org.bluez";
constexpr const char* DEVICE0 = "/org/bluez/hci0";

static int sig = 0;
static void sig_callback(int signum)
{
    exit(signum);
}

int main(int argc, char *argv[])
{
    if( signal(SIGINT, sig_callback) == SIG_ERR )
        std::cerr << std::endl << "error registering signal handler" << std::endl;

    constexpr const char* APP_PATH = "/org/bluez/example";
    constexpr const char* SRV_PATH = "/org/bluez/example/service1";
    constexpr const char* ADV_PATH = "/org/bluez/example/advertisement1";

    constexpr const char* NAME = "Node1";

    std::shared_ptr<IConnection> connection{ std::move( sdbus::createSystemBusConnection() ) };

    // ---- Adapter Info -----------------------------------------------------------------------------------------------

    // {
    //     Adapter1 adapter1{ *connection, BLUEZ_SERVICE, DEVICE0 };

    //     adapter1.Powered( true );
    //     adapter1.Discoverable( true );
    //     adapter1.Pairable( false );
    //     adapter1.Alias( NAME );

    //     std::cout << "Found adapter '" << DEVICE0 << "'" << std::endl;
    //     std::cout << "  Name: " << adapter1.Name() << std::endl;
    //     std::cout << "  Address: " << adapter1.Address() << " type: " << adapter1.AddressType() << std::endl;
    //     std::cout << "  Powered: " << adapter1.Powered() << std::endl;
    //     std::cout << "  Discoverable: " << adapter1.Discoverable() << std::endl;
    //     std::cout << "  Pairable: " << adapter1.Pairable() << std::endl;
    // }

    std::cout << std::endl;

    // ---- Services ---------------------------------------------------------------------------------------------------
    
    auto nwk = std::make_shared<Network1>( connection );
    auto app = std::make_shared<Application1>( connection, 0x05f1, 0x0001 );
    auto agent = std::make_shared<ProvisionAgent1>(connection);
    auto ele = std::make_shared<Element1>(connection, 0x00);

    std::cout << "Loading complete." << std::endl;

    //nwk->Join( app->getPath(), { 0x0a, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F } );

    connection->enterProcessingLoopAsync();

    bool run = true;
    while( run) {
        char cmd;
        std::cout << "commands:" << std::endl;
        std::cout << "  q      quit" << std::endl;
        std::cout << "$> ";
        std::cin >> cmd;

        switch(cmd)
        {
            case 'q':
                run = false;
        }
    } 
}
