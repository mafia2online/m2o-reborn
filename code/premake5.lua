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

--x86 !
CEF_VERSION = "cef_binary_3.3440.1806.g65046b7_windows32"

--dofile('build/helpers/type_select.lua')
dofile('build/helpers/cef_setup.lua')

verifycef(CEF_VERSION)

FX_NAME = "MAFIA PLUS"

dofile('build/vendor/vendorfiles.lua')
--dofile('build/modules/modules.lua')

workspace "MPLUS"
    configurations { "Debug", "Release" }

	if _OPTIONS['is_ci_build'] == 'true' then
		
	   defines "CI_BUILD"
	end
	
	if os.istarget('windows') then
		buildoptions "/std:c++latest"
		-- systemversion "10.0.15063.0"
	else
		buildoptions "-std=c++17"
	end

    targetprefix ""
    symbols "On"
    characterset "Unicode"
	
    -- Enable position-independent-code generation
    pic "On"
    startproject "launcher"
	platforms { "x86" }
	
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
	
	libdirs
	{
		"./shared/Lib",
	}

    filter "platforms:x64"
         architecture "x86_64"

    filter "configurations:Debug"
        defines { "GC_DBG" }

    filter "configurations:Release"
        --flags { "StaticRuntime" }
        optimize "Speed"

    filter {"system:windows", "configurations:Release", "kind:not StaticLib"}
        linkoptions "/PDB:\"symbols\\$(ProjectName).pdb\""

    filter { "system:windows", "kind:not StaticLib" }
        linkoptions "/manifestdependency:\"type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\""

    -- Disable deprecation warnings and errors
    filter "action:vs*"
        defines
        {
            "_CRT_SECURE_NO_WARNINGS",
            "_CRT_SECURE_NO_DEPRECATE",
            "_CRT_NONSTDC_NO_WARNINGS",
            "_CRT_NONSTDC_NO_DEPRECATE",
            "_SCL_SECURE_NO_WARNINGS",
            "_SCL_SECURE_NO_DEPRECATE",
            
            "_WINSOCK_DEPRECATED_NO_WARNINGS",
        }

	if os.target() == "windows" then
    	group "Client"
		include "client/host"
		include "client/worker"
	end
	
	include "client/core"
	
	group "Server"
	include "server/host"
	--include "server/core"
	
	--group "Modules"
	--do_modules()
	
	group "Shared"
	include "./shared"

    group "Vendor"
	if os.target() == "windows" then
		--include "vendor/nomad-common"
		include "vendor/minhook"
		--include "vendor/imgui"
	end
	include "vendor/m2framework"
	
	--include "vendor/enet"
	do_vendor()
	
-- Cleanup
if _ACTION == "clean" then
    os.rmdir("../bin");
    os.rmdir("../build");
end
