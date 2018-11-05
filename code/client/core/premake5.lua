
-- MafiaPLUS build configuration script
-- Author(s): Force67
--
-- Copyright (c) 2015-2018 Dev-Force

project "client-core"
    language "C++"
    kind "SharedLib"
    targetname "client-core"
    linkoptions {"/DELAYLOAD:libcef.dll"}
    disablewarnings { "4099" }
    --flags { "StaticRuntime" }

    libdirs {
        "../../../bin/vendor/%{cfg.buildcfg}",
        "../../vendor/d3d9",
    }

    vpaths { ["*"] = "*" }

    defines {
        "WIN32_LEAN_AND_MEAN",
    }

    includedirs {
        ".",
        "../../shared",
        "../../vendor",
        "../../vendor/minhook",
        "../../vendor/d3d9/include",
        "../../vendor/m2framework",
        "../../vendor/cef/" .. CEF_VERSION,
        "../../vendor/d3d9/include",
    }

    links {
        "ws2_32",
        "crypt32",
        "delayimp",

        "d3d9",
        "d3dx9",

        "shared",
        "minhook",

        "m2framework",

        "cefwrapper",
        "libcef",
    }

    filter "system:windows"
        links "Winmm"

    files {
        "premake5.lua",
        "**.h",
        "**.hpp",
        "**.cpp",
        --"**.rc"
    }

    filter "system:not windows"
        flags { "ExcludeFromBuild" }
