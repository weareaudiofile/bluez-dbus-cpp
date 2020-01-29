##
# Copyright Audiofile LLC 2019 - 2020.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
##

# Minimal version of CMake
cmake_minimum_required (VERSION 3.1 FATAL_ERROR)

# Build type
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to 'Release' as none was specified.")
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
	# Set the possible values of build type for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
		"MinSizeRel" "RelWithDebInfo")
endif ()

# Define project name
project (bluez-test-cxx)

# The version number
set (${PROJECT_NAME}_VERSION_MAJOR 0)
set (${PROJECT_NAME}_VERSION_MINOR 0)
set (${PROJECT_NAME}_PATCH_LEVEL 1)

set (${PROJECT_NAME}_VERSION
	"${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}")

# Set up directory with 3rd party cmake modules
set (CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/build_files/cmake/modules/")


#-------------------------------
# GENERAL COMPILER CONFIGURATION
#-------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#-------------------------------
# SOURCE FILES CONFIGURATION
#-------------------------------
set( BLUEZ_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set( BLUEZ_HEADERS
	"${BLUEZ_HEADER_DIR}/bluez/adaptor/GattCharacteristic1_adaptor.h"
	"${BLUEZ_HEADER_DIR}/bluez/adaptor/GattDescriptor1_adaptor.h"
	"${BLUEZ_HEADER_DIR}/bluez/adaptor/GattService1_adaptor.h"
	"${BLUEZ_HEADER_DIR}/bluez/adaptor/ObjectManagerExt_adaptor.h"
	"${BLUEZ_HEADER_DIR}/bluez/adaptor/PropertiesExt_adaptor.h"
	"${BLUEZ_HEADER_DIR}/bluez/proxy/Adapter1_proxy.h"
	"${BLUEZ_HEADER_DIR}/bluez/proxy/AgentManager1_proxy.h"
	"${BLUEZ_HEADER_DIR}/bluez/proxy/HealthManager1_proxy.h"
	"${BLUEZ_HEADER_DIR}/bluez/proxy/Media1_proxy.h"
	"${BLUEZ_HEADER_DIR}/bluez/proxy/NetworkServer1_proxy.h"
	"${BLUEZ_HEADER_DIR}/bluez/proxy/ProfileManager1_proxy.h"
	"${BLUEZ_HEADER_DIR}/bluez/proxy/SimAccess1_proxy.h"
	"${BLUEZ_HEADER_DIR}/bluez/Adapter1.h"
	"${BLUEZ_HEADER_DIR}/bluez/bluez.h"
	"${BLUEZ_HEADER_DIR}/bluez/GattApplication1.h"
	"${BLUEZ_HEADER_DIR}/bluez/GattCharacteristic1.h"
	"${BLUEZ_HEADER_DIR}/bluez/GattDescriptor1.h"
	"${BLUEZ_HEADER_DIR}/bluez/GattManager1.h"
	"${BLUEZ_HEADER_DIR}/bluez/GattService1.h"
	"${BLUEZ_HEADER_DIR}/bluez/LEAdvertisement1.h"
	"${BLUEZ_HEADER_DIR}/bluez/LEAdvertisingManager1.h"
	"${BLUEZ_HEADER_DIR}/bluez/Util.h"
	"${BLUEZ_HEADER_DIR}/bluez-ext/bluez-ext.h"
	"${BLUEZ_HEADER_DIR}/bluez-ext/GenericCharacteristic.h"
	"${BLUEZ_HEADER_DIR}/bluez-ext/ReadOnlyCharacteristic.h" )

set( BLUEZ_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set( BLUEZ_SOURCES
	"${BLUEZ_SOURCE_DIR}/GattService1.cpp"
	"${BLUEZ_SOURCE_DIR}/GattCharacteristic1.cpp"
	"${BLUEZ_SOURCE_DIR}/GattDescriptor1.cpp"
	"${BLUEZ_SOURCE_DIR}/Util.cpp" )


add_library( bluez-cpp
	${BLUEZ_SOURCES}
	${BLUEZ_HEADERS} )

target_include_directories( bluez-cpp PUBLIC ${BLUEZ_HEADER_DIR} )

# Link up
find_package(sdbus-c++ REQUIRED)
target_link_libraries( bluez-cpp PRIVATE SDBusCpp::sdbus-c++)

if ( BUILD_EXAMPLE )
	add_subdirectory( example )
endif()
