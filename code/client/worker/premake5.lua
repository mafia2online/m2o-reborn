-- NomadFX build configuration script
-- Author(s): Force67
--
-- Copyright (c) 2015-2018 Dev-Force
--
-- Distributed under the MIT license (See accompanying file LICENSE or copy at
-- https://opensource.org/licenses/MIT)

project "CefWorker"
    language "C++"
    kind "WindowedApp"
    targetname "MPII_Worker"
	flags "NoManifest"

	filter "system:Windows"
		linkoptions '/MANIFESTDEPENDENCY:"NFX" /MANIFESTDEPENDENCY:"Compatability"'

    vpaths
    {
        ["Headers/*"] = { "**.hpp", "**.h" },
        ["Sources/*"] = "**.cpp",
        ["Resources/*"] =  { "**.rc", "**.manifest" },
        ["*"] = "premake5.lua"
    }

    libdirs
    {
		"../../../bin/",
		"../../../bin/vendor/%{cfg.buildcfg}/"
    }
	
    includedirs
    {
		"./include",
        ".",
		
		"../../shared",
		"../../vendor/cef/" .. CEF_VERSION,
    }

	filter { "system:windows" }
		links
		{
			"minhook",
			"delayimp"
		}
		
		linkoptions "/DELAYLOAD:libcef.dll"

    links
	{
		"libcef",
		"cefwrapper",
	}

    files
    {
		"premake5.lua", "**.h", "**.hpp", "**.cpp", "**.rc", "**.manifest"
    }
