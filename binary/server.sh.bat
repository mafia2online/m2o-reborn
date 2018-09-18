#/bin/bash 2>nul || goto :windows

# bash
echo [bash env]

cd server
./server

exit

:windows
@echo off


cd server

m2o-server.exe
