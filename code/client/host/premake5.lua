-- Host build configuration script
-- Author(s): Force67
--
-- Copyright (c) 2015-2018 Dev-Force
--
-- Distributed under the MIT license (See accompanying file LICENSE or copy at
-- https://opensource.org/licenses/MIT)

project "Host"
    language "C++"
    kind "WindowedApp"
    targetname "MPII_Host" 
	flags { "NoIncrementalLink" }
	editandcontinue "Off"
    linkoptions "/IGNORE:4254 /ENTRY:wmainCRTStartup /OPT:NOLBR /SAFESEH:NO /DYNAMICBASE:NO /LARGEADDRESSAWARE /LAST:.zdata"

    vpaths
    {
        ["Headers/*"] = { "**.hpp", "**.h" },
        ["Sources/*"] = "**.cpp",
        ["Resources/*"] = "**.rc",
        ["*"] = "premake5.lua"
    }

    includedirs
    {
        "."
    }

    links
    {
		"shared",
    }

    files
    {
        "premake5.lua",
        "**.h",
        "**.hpp",
        "**.cpp",
        "**.rc"
    }

    filter "architecture:x64"
        flags { "ExcludeFromBuild" }

    filter "system:not windows"
        flags { "ExcludeFromBuild" }
