:: Premake5 Invoker util
:: Author(s):       Force67
::
:: Copyright (c) 2015-2018 Dev-Force
::
:: Distributed under the MIT license (See accompanying file LICENSE or copy at
:: https://opensource.org/licenses/MIT)

@echo off

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
