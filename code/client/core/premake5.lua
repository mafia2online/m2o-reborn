
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
        "../../vendor/m2framework",
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

        "m2adhesive",
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


    -- additional data
    local additional = function()
        print(" - generating additional client data")

        local steam_id = "50130"
        local fxconfig = "[fxconfig]\r\n"

        fxconfig = fxconfig .. "maf2_path=C://Program Files//Steam//steamapps//common//Mafia II//pc\r\n"

        local dpath = "../../../bin/Debug/"
        local rpath = "../../../bin/Release/"

        local function file_exists(name)
           local f=io.open(name,"r")
           if f~=nil then io.close(f) return true else return false end
        end

        io.writefile(dpath .. "steam_appid.txt", steam_id)
        io.writefile(rpath .. "steam_appid.txt", steam_id)

        if not file_exists(dpath .. "fxconfig.ini") then io.writefile(dpath .. "fxconfig.ini", fxconfig) end
        if not file_exists(rpath .. "fxconfig.ini") then io.writefile(rpath .. "fxconfig.ini", fxconfig) end
    end

    additional();
