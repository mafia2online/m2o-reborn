# ReGuider Framework
This framework offers a \*complete\* suite to host cross-platform multiplayer games and applications.

# Warning
This is an **archived** version of librg. New one is **still under development**.
I **wouldn't suggest you to use it** for your projects. It's **not supported** anymore.

## Pre-install
Before starting, make sure you have installed [CMake](https://cmake.org/). It will be used to generate make files for your system.
Also make sure you have installed a working compiler for your system.

## Installation

```sh
$ git clone --recursive git@gitlab.com:reguider/reguider.git
$ mkdir -p build && cd build
$ cmake ..
```

## Building

Go inside the build folder, there you can find either your generated project files.

Unix:

```sh
cd build
make -j<numcores>
```

Windows:
If you are using Visual Studio, proper project/solution files should be generated for your visual studio.
Run build, and enjoy.

## Used dependencies
* enet
* entityx
* libuv
* sqrat
* squirrel-lang
* tiny-xml

## Compilation tested on
* macos 10.12.2 with g++ 4.2.1
* ubuntu 16.04 with g++
* windows 10 with msvc 14.0
