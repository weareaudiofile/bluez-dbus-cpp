#!/bin/bash

#
# Copyright Audiofile LLC 2019 - 2020.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
#

# output paths
PROXY=../proxy
ADAPTOR=../adaptor

# Clients
sdbus-c++-xml2cpp Adapter1.xml --proxy=${PROXY}/Adapter1_proxy.h
sdbus-c++-xml2cpp Media1.xml --proxy=${PROXY}/Media1_proxy.h
sdbus-c++-xml2cpp NetworkServer1.xml --proxy=${PROXY}/NetworkServer1_proxy.h
sdbus-c++-xml2cpp SimAccess1.xml --proxy=${PROXY}/SimAccess1_proxy.h
sdbus-c++-xml2cpp AgentManager1.xml --proxy=${PROXY}/AgentManager1_proxy.h
sdbus-c++-xml2cpp ProfileManager1.xml --proxy=${PROXY}/ProfileManager1_proxy.h
sdbus-c++-xml2cpp HealthManager1.xml --proxy=${PROXY}/HealthManager1_proxy.h

# Services
sdbus-c++-xml2cpp GattService1.xml --adaptor=${ADAPTOR}/GattService1_adaptor.h
sdbus-c++-xml2cpp GattCharacteristic1.xml --adaptor=${ADAPTOR}/GattCharacteristic1_adaptor.h
sdbus-c++-xml2cpp GattDescriptor1.xml --adaptor=${ADAPTOR}/GattDescriptor1_adaptor.h

## Mesh
# Clients
sdbus-c++-xml2cpp mesh/Management1.xml --proxy=${PROXY}/mesh/Management1_proxy.h
sdbus-c++-xml2cpp mesh/Network1.xml --proxy=${PROXY}/mesh/Network1_proxy.h
sdbus-c++-xml2cpp mesh/Node1.xml --proxy=${PROXY}/mesh/Node1_proxy.h
sdbus-c++-xml2cpp mesh/ProvisionAgent1.xml --proxy=${PROXY}/mesh/ProvisionAgent1_proxy.h
sdbus-c++-xml2cpp mesh/Provisioner1.xml --proxy=${PROXY}/mesh/Provisioner1_proxy.h

# Services
sdbus-c++-xml2cpp mesh/Application1.xml --adaptor=${ADAPTOR}/mesh/Application1_adaptor.h
sdbus-c++-xml2cpp mesh/Element1.xml --adaptor=${PROXY}/mesh/Element1_adaptor.h