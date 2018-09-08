if not exist build mkdir build
cd build
cmake .. -G"Visual Studio 15 2017" -DM2O_CLIENT=1 -DM2O_SERVER=1 -DM2O_EXPORT_SDK=1
pause
