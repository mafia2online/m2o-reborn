project "server-core"
    language "C++"
    kind "ConsoleApp"

    architecture "x86_64"

    includedirs
    {
        "../../vendor",
        "../../shared",
        "."
    }

    files
    {
        "premake5.lua",
        "**.h",
        "**.hpp",
        "**.cpp",
        "**.rc"
    }

    configuration { "macos" }
        links { "m", "pthread", "dl" }

    if os.target() ~= "windows" then
        removeplatforms { "x86" }
    end
