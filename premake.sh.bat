#/bin/bash 2>nul || goto :windows

unameOut="$(uname -s)"
case "${unameOut}" in
    Linux*)     machine=linux;;
    Darwin*)    machine=macos;;
    MINGW*)     machine=mingw;;
    *)          machine="unknown"
esac

cd code

if [[ $1 == "clean" ]]
then
    # Invoke premake5's clean action
    echo "Performing clean action..."
    build\{$machine}\premake5 clean
else
    build/${machine}/premake5 gmake2 "${@}"
fi

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
