
# The eCAL core only project

[![License](https://img.shields.io/github/license/continental/ecal.svg?style=flat)](LICENSE.txt)

The **e**nhanced **C**ommunication **A**bstraction **L**ayer (eCAL) is a middleware that enables scalable, high performance interprocess communication on a single computer node or between different nodes in a computer network. 

This eCAL core version has an reduced functionality compared to [Eclipse-eCAL](https://github.com/eclipse-ecal/ecal) (see section Differences to Eclipse-eCAL). The goal of this reduced approach is to create a core with as few dependencies as possible, which should facilitate integration into embedded environments or smart sensors.

Visit the eCAL Documentation at 🌐 https://ecal.io for more information.

## Differences to Eclipse-eCAL
  - communication core only, no additional eco system tools
  - C++ and C language support only
  - udp transport layer only (no shared memory, no tcp)
  - no monitoring API
  - no log messaging over udp

## How to build

```bash
mkdir _build
cd _build
cmake .. -A x64
cmake --build . --parallel --config Release
```
