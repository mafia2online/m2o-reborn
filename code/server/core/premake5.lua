project "server-core"
    language "C++"
    kind "ConsoleApp"

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
