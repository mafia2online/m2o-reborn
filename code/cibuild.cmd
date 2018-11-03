:: Script to build MafiaPLUS
:: Author(s):       Force67
::
:: Copyright (c) 2015-2018 Dev-Force
::
:: Distributed under the MIT license (See accompanying file LICENSE or copy at
:: https://opensource.org/licenses/MIT)

@echo off

echo "You are creating a ci build of client & server"

:: Check if 'vswhere' util from SxS installer can be found.
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" (
    set VSWhereDir="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
    echo Found VSWhere from VS SxS Installer: "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
) else (
    set VSWhereDir=buildtools\win\vswhere.exe
    echo Couldn't find VSWhere from VS SxS Installer, falling back to vswhere in buildtools\win
)

:: Now use VSWhere to determine VS install's root directory
for /f "usebackq delims=" %%i in (`%VSWhereDir% -latest -requires Microsoft.VisualStudio.Workload.NativeDesktop Microsoft.Component.MSBuild -property installationPath`) do (
  set InstallDir=%%i
)

:: See if we can find devenv.exe to ensure base VS is installed or not
if exist "%InstallDir%\Common7\IDE\devenv.exe" (
    echo Found VS install: "%InstallDir%"
) else (
	echo Couldn't find a valid VS SxS install. Make sure you have Visual Studio 2017 with required toolchains installed properly.
    goto end
)

:: Do extra validation to ensure the existence of required toolchains
set MSBuildPath=%InstallDir%\MSBuild\15.0\Bin
if not exist "%MSBuildPath%\MSBuild.exe" (
    echo MSBuild not found. Make sure you have Visual Studio 2017 with required toolchains installed properly.
    goto end
)

set vcvars32Path=%InstallDir%\VC\Auxiliary\Build
if not exist "%vcvars32Path%\vcvars32.bat" (
    echo vcvars32.bat not found. Make sure you have Visual Studio 2017 with required toolchains installed properly.
    goto end
)

set vcvars64Path=%InstallDir%\VC\Auxiliary\Build
if not exist "%vcvars64Path%\vcvars64.bat" (
    echo vcvars64.bat not found. Make sure you have Visual Studio 2017 with required toolchains installed properly.
    goto end
)

set vcvarsallPath=%InstallDir%\VC\Auxiliary\Build
if not exist "%vcvarsallPath%\vcvarsall.bat" (
    echo vcvarsall.bat not found. Make sure you have Visual Studio 2017 with required toolchains installed properly.
    goto end
)

set VsDevCmdPath=%InstallDir%\Common7\Tools
if not exist "%VsDevCmdPath%\VsDevCmd.bat" (
    echo VsDevCmd.bat not found. Make sure you have Visual Studio 2017 with required toolchains installed properly.
    goto end
)

echo.

:: Install/Update CEF
::buildtools\win\premake5 install_cef

:: Build solutions..
..\tools\win\premake5 %* vs2017 --is_ci_build=true

:: Build the solutions
"%MSBuildPath%\MSBuild.exe" ../build/MPLUS.sln /v:minimal /m /p:PreferredToolArchitecture=x64 /p:Configuration=Release

:: TODO: Run deployment scripts here

:: Pause for 5 seconds and auto-close the command window
:end
timeout /t 5 /nobreak
