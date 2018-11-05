-- Host build configuration script
-- Author(s): Force67
--
-- Copyright (c) 2015-2018 Dev-Force
--
-- Distributed under the MIT license (See accompanying file LICENSE or copy at
-- https://opensource.org/licenses/MIT)

project "client-host"
    language "C++"
    kind "WindowedApp"

    targetname "client-host"
    flags { "NoIncrementalLink" }
    editandcontinue "Off"

    vpaths { ["*"] = "*" }

    linkoptions "/IGNORE:4254 /ENTRY:wmainCRTStartup /OPT:NOLBR /SAFESEH:NO /DYNAMICBASE:NO /LARGEADDRESSAWARE /LAST:.zdata"

    includedirs {
        "."
    }

    links {
        "shared",
    }

    files {
        "premake5.lua",
        "**.h",
        "**.hpp",
        "**.cpp",
        "**.rc"
    }

    filter "files:host.cpp"
        flags { "ExcludeFromBuild" }

    filter "architecture:x64"
        flags { "ExcludeFromBuild" }

    filter "system:not windows"
        flags { "ExcludeFromBuild" }
