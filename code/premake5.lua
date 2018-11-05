-- SAP build configuration script
-- Author(s): Force67
--
-- Copyright (c) 2015-2018 Dev-Force
--
-- Distributed under the MIT license (See accompanying file LICENSE or copy at
-- https://opensource.org/licenses/MIT)

-- This is the root build file for Premake. Premake will start processing by loading this
-- file, and recursively load all dependencies until all dependencies are either
-- resolved or known not to exist (which will cause the build to fail). So if
-- you add a new build file, there must be some path of dependencies from this
-- file to your new one or Premake won't know about it.

premake.path = premake.path .. ";build"

if os.target() == "windows" then
    --x86 !
    CEF_VERSION = "cef_binary_3.3440.1806.g65046b7_windows32"

    --dofile('../tools/premake/helpers/type_select.lua')
    dofile('../tools/premake/helpers/cef_setup.lua')

    verifycef(CEF_VERSION)
end

FX_NAME = "MAFIA PLUS"

dofile('../tools/premake/vendor/vendorfiles.lua')

workspace "mplus"
    configurations { "Debug", "Release" }
    platforms { "x86" }

    if os.istarget('windows') then
        buildoptions "/std:c++latest"
    else
        buildoptions "-std=c++17"
    end

    symbols "On"
    targetprefix ""
    characterset "Unicode"

    -- Enable position-independent-code generation
    pic "On"
    startproject "launcher"

    defines { "FXNAME=\"" .. FX_NAME .. "\""}
    defines { "FXNAME_WIDE=L\"" .. FX_NAME .. "\""}

    location "../build/"
    targetdir "../bin/%{cfg.buildcfg}/"
    os.mkdir"../build/symbols"

    defines
    {
        "NOMINMAX",
        --"WIN32_LEAN_AND_MEAN"
    }

    includedirs
    {
        ".",
        "./shared",
    }

    filter "platforms:x64"
         architecture "x86_64"

    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG", "GC_DBG" }
        optimize "Off"
        runtime "Debug"

    filter "configurations:Release"
        -- staticruntime "On"
        optimize "Speed"

    filter {"system:windows", "configurations:Release", "kind:not StaticLib"}
        linkoptions "/PDB:\"symbols\\$(ProjectName).pdb\""

    filter { "system:windows", "kind:not StaticLib" }
        linkoptions "/manifestdependency:\"type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\""

    -- Disable deprecation warnings and errors
    -- disabling as less warnings as possible
    filter "action:vs*"
        defines
        {
            "_CRT_SECURE_NO_WARNINGS",
            -- "_CRT_SECURE_NO_DEPRECATE",
            -- "_CRT_NONSTDC_NO_WARNINGS",
            -- "_CRT_NONSTDC_NO_DEPRECATE",
            -- "_SCL_SECURE_NO_WARNINGS",
            -- "_SCL_SECURE_NO_DEPRECATE",

            "_WINSOCK_DEPRECATED_NO_WARNINGS",
        }

    --
    -- Source subprojects
    --

    if os.target() == "windows" then
        group "client"
        include "client/host"
        include "client/worker"
    end

    group "server"
    include "server/host"
    include "server/core"

    -- not really a shared tho, needs some other name
    if os.target() == "windows" then
        group ""
        include "./shared"
    end

    group "vendor"
    if os.target() == "windows" then
        include "vendor/minhook"
    end

    do_vendor()

-- Cleanup
if _ACTION == "clean" then
    os.rmdir("../bin");
    os.rmdir("../build");
end
