-- NomadFX build configuration script
-- Author(s): Force67
--
-- Copyright (c) 2015-2018 Dev-Force
--
-- Distributed under the MIT license (See accompanying file LICENSE or copy at
-- https://opensource.org/licenses/MIT)

project "client-worker"
    language "C++"
    kind "WindowedApp"

    targetname "client-worker"
    flags "NoManifest"

    filter "system:Windows"
        linkoptions '/MANIFESTDEPENDENCY:"NFX" /MANIFESTDEPENDENCY:"Compatability"'

    vpaths { ["*"] = "*" }

    libdirs {
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
        links {
            "minhook",
            "delayimp"
        }

        linkoptions "/DELAYLOAD:libcef.dll"

    links {
        "libcef",
        "cefwrapper",
    }

    files {
        "premake5.lua",
        "**.h",
        "**.hpp",
        "**.cpp",
        "**.rc",
        "**.manifest"
    }

    filter "files:ui/**"
        flags { "ExcludeFromBuild" }
