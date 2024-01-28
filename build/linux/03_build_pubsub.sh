#!/bin/bash

cd "$(dirname "$0")/../.."

CMAKE_OPTIONS="-DCMAKE_INSTALL_PREFIX=_install \
-DECAL_CORE_BUILD_SAMPLES=ON \
-DECAL_CORE_BUILD_SAMPLES_PROTOBUF=OFF \
-DECAL_CORE_BUILD_TESTS=ON \
-DECAL_CORE_BUILD_TESTS_PROTOBUF=OFF \
-DECAL_CORE_CONFIG_INIFILE=OFF \
-DECAL_CORE_COMMAND_LINE=OFF \
-DECAL_CORE_REGISTRATION=ON \
-DECAL_CORE_REGISTRATION_SHM=ON \
-DECAL_CORE_PUBLISHER=ON \
-DECAL_CORE_SUBSCRIBER=ON \
-DECAL_CORE_SERVICE=OFF \
-DECAL_CORE_TIMEPLUGIN=OFF \
-DECAL_CORE_TRANSPORT_UDP=ON \
-DECAL_CORE_TRANSPORT_TCP=ON \
-DECAL_CORE_TRANSPORT_SHM=ON \
-DECAL_CORE_NPCAP_SUPPORT=OFF"

mkdir -p _03_build_pubsub
cd _03_build_pubsub

cmake .. -DCMAKE_BUILD_TYPE=Release ${CMAKE_OPTIONS}
make -j

# Run tests if available
make test

