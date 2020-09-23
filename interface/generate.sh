#!/bin/bash

#
# Copyright Audiofile LLC 2019 - 2020.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
#

# output paths
PROXY=../include/bluez-dbus-cpp/proxy
ADAPTOR=../include/bluez-dbus-cpp/adaptor
XML2CPP=sdbus-c++-xml2cpp

# Clients
${XML2CPP} Adapter1.xml --proxy=${PROXY}/Adapter1_proxy.h
${XML2CPP} Device1.xml --proxy=${PROXY}/Device1_proxy.h
${XML2CPP} Media1.xml --proxy=${PROXY}/Media1_proxy.h
${XML2CPP} NetworkServer1.xml --proxy=${PROXY}/NetworkServer1_proxy.h
${XML2CPP} SimAccess1.xml --proxy=${PROXY}/SimAccess1_proxy.h
${XML2CPP} AgentManager1.xml --proxy=${PROXY}/AgentManager1_proxy.h
${XML2CPP} ProfileManager1.xml --proxy=${PROXY}/ProfileManager1_proxy.h
${XML2CPP} HealthManager1.xml --proxy=${PROXY}/HealthManager1_proxy.h

# Services
${XML2CPP} GattService1.xml --adaptor=${ADAPTOR}/GattService1_adaptor.h
${XML2CPP} GattCharacteristic1.xml --adaptor=${ADAPTOR}/GattCharacteristic1_adaptor.h
${XML2CPP} GattDescriptor1.xml --adaptor=${ADAPTOR}/GattDescriptor1_adaptor.h
