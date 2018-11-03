
-- MafiaPLUS build configuration script
-- Author(s): Force67
--
-- Copyright (c) 2015-2018 Dev-Force

project "core-client"
    language "C++"
    kind "SharedLib"
    targetname "core-client"
    linkoptions {"/DELAYLOAD:libcef.dll"}
    disablewarnings { "4099" }
	--flags { "StaticRuntime" }

    libdirs
	{
         "../../../bin/vendor/%{cfg.buildcfg}",
	}

    defines
    {
        "WIN32_LEAN_AND_MEAN",
    }

    vpaths
    {
        ["Headers/*"] = { "**.hpp", "**.h" },
        ["Sources/*"] = "**.cpp",
        ["Resources/*"] = "**.rc",
        ["*"] = "premake5.lua"
    }
		
    includedirs
    {
        ".",
		"../../shared",
        "../../vendor/minhook",
		"../../vendor/m2framework",
		"../../vendor/cef/" .. CEF_VERSION, 
    }

    links
    {
		"ws2_32",
		"crypt32", 
        "delayimp",

        "shared",
        "minhook",

	    "m2framework",

		"cefwrapper",
        "libcef",
    }

    filter "system:windows"
        links "Winmm"

    files
    {
        "premake5.lua",
        "**.h",
        "**.hpp",
        "**.cpp",
        --"**.rc"
    }

    filter "system:not windows"
        flags { "ExcludeFromBuild" }
