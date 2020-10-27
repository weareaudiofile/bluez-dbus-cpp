//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#include <bluez-dbus-cpp/bluez.h>
#include <bluez-dbus-cpp/GenericCharacteristic.h>
#include <bluez-dbus-cpp/ReadOnlyCharacteristic.h>
#include "SerialCharacteristic.h"

#include <iostream>
#include <signal.h>

#include <ell/main.h>

#include <thread>

using namespace org::bluez;

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

    constexpr const char* NAME = "ExampleBlueZ";

    std::shared_ptr<IConnection> connection{ std::move( sdbus::createSystemBusConnection() ) };

    // ---- Boost Info ----
    std::string output;
    #if defined(BOOST_ASIO_HAS_IOCP)
        output = "iocp" ;
    #elif defined(BOOST_ASIO_HAS_EPOLL)
        output = "epoll" ;
    #elif defined(BOOST_ASIO_HAS_KQUEUE)
        output = "kqueue" ;
    #elif defined(BOOST_ASIO_HAS_DEV_POLL)
        output = "/dev/poll" ;
    #else
        output = "select" ;
    #endif
    std::cout << output << std::endl;

    // ---- Adapter Info -----------------------------------------------------------------------------------------------

    {
        Adapter1 adapter1{ *connection, BLUEZ_SERVICE, DEVICE0 };

        adapter1.Powered( true );
        adapter1.Discoverable( true );
        adapter1.Pairable( true );
        adapter1.Alias( NAME );

        std::cout << "Found adapter '" << DEVICE0 << "'" << std::endl;
        std::cout << "  Name: " << adapter1.Name() << std::endl;
        std::cout << "  Address: " << adapter1.Address() << " type: " << adapter1.AddressType() << std::endl;
        std::cout << "  Powered: " << adapter1.Powered() << std::endl;
        std::cout << "  Discoverable: " << adapter1.Discoverable() << std::endl;
        std::cout << "  Pairable: " << adapter1.Pairable() << std::endl;
    }

    std::cout << std::endl;

    // ---- Services ---------------------------------------------------------------------------------------------------
    GattManager1 gattMgr{ connection, BLUEZ_SERVICE, DEVICE0 };
    auto app =  std::make_shared<GattApplication1>( connection, APP_PATH );
    auto srv1 = std::make_shared<GattService1>( app, "deviceinfo", "180A" );
    ReadOnlyCharacteristic::createFinal( srv1, "2A24", NAME ); // model name
    ReadOnlyCharacteristic::createFinal( srv1, "2A25", "333-12345678-888" ); // serial number
    ReadOnlyCharacteristic::createFinal( srv1, "2A26", "1.0.1" ); // fw rev
    ReadOnlyCharacteristic::createFinal( srv1, "2A27", "rev A" ); // hw rev
    ReadOnlyCharacteristic::createFinal( srv1, "2A28", "5.0" ); // sw rev
    ReadOnlyCharacteristic::createFinal( srv1, "2A29", "ACME Inc." ); // manufacturer

    auto srv2 = std::make_shared<GattService1>( app, "serial", "368a3edf-514e-4f70-0000-2d0a5a62bc8c" );
    SerialCharacteristic::create( srv2, "368a3edf-514e-4f70-0001-2d0a5a62bc8c" )
        .finalize();

    auto register_app_callback = [](const sdbus::Error* error)
    {
        if( error == nullptr )
        {
            std::cout << "Application registered." << std::endl;
        }
        else
        {
            std::cerr << "Error registering application " << error->getName() << " with message " << error->getMessage() << std::endl;
        }
    };

    gattMgr.RegisterApplicationAsync( app->getPath(), {} )
        .uponReplyInvoke(register_app_callback);

    // ---- Advertising ------------------------------------------------------------------------------------------------

    auto mgr = std::make_shared<LEAdvertisingManager1>( connection, BLUEZ_SERVICE, DEVICE0 );
    std::cout << "LEAdvertisingManager1" << std::endl;
    std::cout << "  ActiveInstances: " << mgr->ActiveInstances() << std::endl;
    std::cout << "  SupportedInstances: " << mgr->SupportedInstances() << std::endl;
    {
        std::cout << "  SupportedIncludes: ";
        auto includes = mgr->SupportedIncludes();
        for( auto include : includes )
        {
            std::cout << "\"" << include <<"\",";
        }
        std::cout << std::endl;
    }

    auto register_adv_callback = [](const sdbus::Error* error) -> void
    {
        if( error == nullptr )
        {
            std::cout << "Advertisement registered." << std::endl;
        }
        else
        {
            std::cerr << "Error registering advertisment " << error->getName() << " with message " << error->getMessage() << std::endl;
        }
    };

    auto ad = LEAdvertisement1::create( *connection, ADV_PATH )
        .withLocalName( NAME )
        .withServiceUUIDs( std::vector{ std::string{"368a3edf-514e-4f70-ba8f-2d0a5a62bc8c"} } )
        .withIncludes( std::vector{ std::string{ "tx-power" }} )
        .onReleaseCall( [](){ std::cout << "advertisement released" << std::endl; } )
        .registerWith( mgr, register_adv_callback );

    std::cout << "Loading complete." << std::endl;

    connection->enterProcessingLoopAsync();

    l_main_init();
    std::thread ell_thread( l_main_run );

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

    l_main_quit();
}
