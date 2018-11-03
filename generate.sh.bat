#/bin/bash 2>nul || goto :windows

# bash
echo [bash env]

mkdir -p build
cd build
cmake .. -DM2O_SERVER=1

exit

:windows
@echo off

cd code

if "%1" == "clean" (
    :: Invoke premake5's clean action
    echo Performing clean action...
    build\win\premake5 clean
) else (
    :: Invoke premake5 with specified args and VS2017 action
    build\win\premake5 %* vs2017
)

:: Pause for 5 seconds and auto-close the command window
:end
timeout /t 3 /nobreak
