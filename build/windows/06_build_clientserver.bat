@echo off

pushd "%~dp0\..\.."

set CMAKE_OPTIONS=-DCMAKE_INSTALL_PREFIX=_install ^
-DBUILD_SAMPLES=ON ^
-DBUILD_SAMPLES_PROTOBUF=OFF ^
-DBUILD_ECAL_TESTS=ON ^
-DBUILD_ECAL_TESTS_PROTOBUF=OFF ^
-DECAL_CORE_CONFIG_INIFILE=OFF ^
-DECAL_CORE_COMMAND_LINE=OFF ^
-DECAL_CORE_REGISTRATION=ON ^
-DECAL_CORE_PUBLISHER=OFF ^
-DECAL_CORE_SUBSCRIBER=OFF ^
-DECAL_CORE_SERVICE=ON ^
-DECAL_CORE_TIMEPLUGIN=OFF ^
-DECAL_CORE_REGISTRATION_SHM=ON ^
-DECAL_CORE_TRANSPORT_UDP=OFF ^
-DECAL_CORE_TRANSPORT_TCP=OFF ^
-DECAL_CORE_TRANSPORT_SHM=OFF ^
-DECAL_CORE_NPCAP_SUPPORT=OFF

mkdir _build_clientserver
cd _build_clientserver

cmake .. -A x64 %CMAKE_OPTIONS%
cmake --build . --parallel --config Release
ctest -C Release -V

popd
