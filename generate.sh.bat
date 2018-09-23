#/bin/bash 2>nul || goto :windows

# bash
echo [bash env]

mkdir -p build
cd build
cmake .. -DM2O_SERVER=1

exit

:windows
@echo off

if not exist build mkdir build
cd build
cmake .. -G"Visual Studio 15 2017" -DM2O_SERVER=1 -DM2O_CLIENT=1 -DM2O_CEF=1 -DM2O_EXPORT_SDK=1
