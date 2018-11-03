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

    if os.target() ~= "windows" then
        links { "m", "pthread", "dl" }
        removeplatforms { "x86" }
    end
