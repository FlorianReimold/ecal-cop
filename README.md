
# The eCAL Cop (core only project)

[![License](https://img.shields.io/github/license/continental/ecal.svg?style=flat)](LICENSE.txt)

The **e**nhanced **C**ommunication **A**bstraction **L**ayer (eCAL) is a middleware that enables scalable, high performance interprocess communication on a single computer node or between different nodes in a computer network. 

This eCAL core version has an reduced functionality compared to [Eclipse-eCAL](https://github.com/eclipse-ecal/ecal) (see _Differences to Eclipse-eCAL_). The goal of this reduced approach is to create a core with as few dependencies as possible, which should facilitate integration into embedded environments or smart sensors.

Visit the eCAL Documentation at 🌐 https://ecal.io for more information.

## Differences to Eclipse-eCAL
  - communication core only, no additional eco system tools
  - C++ and C language support only
  - udp transport layer only (no shared memory, no tcp)
  - no monitoring API
  - no logging over udp (console and file target still supported)

Apart from these limitations, this functional reduced eCAL core version is fully compatible with Eclipse-eCAL. It uses the UDP layer for local and cross-host communication. As a result, it offers lower performance for local host only setups, but it has no drawbacks for network-based (udp) communication.

## How to build

### Clone the repository and its submodules

```bash
git clone https://github.com/ecal-io/ecal-core-udp.git
cd ecal-core-udp
git submodule init
git submodule update
```

### Set CMake Options

This section provides documentation for the CMake options used in configuring the eCAL library. These options allow you to customize the build and enable or disable specific features. Please adapt the options according to your project requirements.

#### Additional Builds

##### `BUILD_SAMPLES` (Default: ON)
- This option controls whether the eCAL samples should be built. Enabling this option will include the compilation of eCAL sample applications. If not needed, you can disable this option to reduce build time and output size.

##### `BUILD_SAMPLES_PROTOBUF` (Default: ON)
- This option controls whether the eCAL samples should be built. Enabling this option will include the compilation of eCAL sample applications. If not needed, you can disable this option to reduce build time and output size.
- Requires serilization library google::protobuf.

##### `BUILD_ECAL_TESTS` (Default: ON)
- This option determines whether the eCAL Google Tests should be built. If enabled, it includes the compilation of unit tests for eCAL. Disabling this option will skip the build of tests if they are not required in your development environment.
- Requires gtest library.

#### Core Internal Feature Configuration

##### `ECAL_CORE_CONFIG_INIFILE` (Default: ON)
- Enabling this option allows eCAL to be configured via an `ecal.ini` file. This file is used to set various configuration parameters for eCAL, providing flexibility in adjusting its behavior without modifying the source code.
- Requires simpleini library.

##### `ECAL_CORE_COMMAND_LINE` (Default: ON)
- Enabling this option includes support for eCAL application command-line interfaces (cmd line). This allows you to interact with eCAL applications through the command line, providing additional runtime configuration options.
- Requires tclap library.

##### `ECAL_CORE_REGISTRATION` (Default: ON)
- Enabling this option activates the eCAL registration layer. This layer is responsible for managing and registering different components within the eCAL ecosystem, facilitating communication between them.
- Requires serilization library google::protobuf or cereal.

##### `ECAL_CORE_PUBLISHER` (Default: ON)
- Enabling this option includes the eCAL publisher functionality. This is essential for components that need to publish data to the eCAL communication infrastructure.
- Requires serilization library google::protobuf or cereal.

##### `ECAL_CORE_SUBSCRIBER` (Default: ON)
- Enabling this option includes the eCAL subscriber functionality. This is crucial for components that need to subscribe to and receive data from the eCAL communication infrastructure.
- Requires serilization library google::protobuf or cereal.

##### `ECAL_CORE_TIMEPLUGIN` (Default: ON)
Enabling this option includes the eCAL time plugin functionality. This allows for precise synchronization of time across different components using eCAL.

##### `ECAL_CORE_CEREAL` (Default: OFF)
Enabling this option would use the Cereal library for internal serialization within eCAL. Note that this option is exclusive of `ECAL_CORE_PROTOBUF`.

##### `ECAL_CORE_PROTOBUF` (Default: ON)
Enabling this option uses Google Protocol Buffers for internal serialization within eCAL. Note that this option is exclusive of `ECAL_CORE_CEREAL`.

Note: Please adjust these options based on your project's needs, ensuring that the selected features align with your desired functionality and dependencies.

### Build it

```bash
mkdir _build
cd _build
cmake .. -A x64
cmake --build .
```
