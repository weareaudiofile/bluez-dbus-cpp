#!/bin/bash

#
# Copyright Audiofile LLC 2019 - 2020.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
#

# output paths
PROXY=../include/bluez-dbus-cpp/proxy
ADAPTOR=../include/bluez-dbus-cpp/adaptor
MESH_PROXY=../include/bluez-dbus-cpp/mesh/proxy
MESH_ADAPTOR=../include/bluez-dbus-cpp/mesh/adaptor
XML2CPP=sdbus-c++-xml2cpp

# Clients
${XML2CPP} Adapter1.xml --proxy=${PROXY}/Adapter1_proxy.h
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

## Mesh
# Clients
${XML2CPP} mesh/Management1.xml --proxy=${MESH_PROXY}/Management1_proxy.h
${XML2CPP} mesh/Network1.xml --proxy=${MESH_PROXY}/Network1_proxy.h
${XML2CPP} mesh/Node1.xml --proxy=${MESH_PROXY}/Node1_proxy.h
${XML2CPP} mesh/Provisioner1.xml --proxy=${MESH_PROXY}/Provisioner1_proxy.h

# Services
${XML2CPP} mesh/Application1.xml --adaptor=${MESH_ADAPTOR}/Application1_adaptor.h
${XML2CPP} mesh/Element1.xml --adaptor=${MESH_ADAPTOR}/Element1_adaptor.h
${XML2CPP} mesh/ProvisionAgent1.xml --adaptor=${MESH_ADAPTOR}/ProvisionAgent1_adaptor.h