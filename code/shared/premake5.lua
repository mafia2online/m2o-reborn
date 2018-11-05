project "shared"
    language "C++"
    kind "StaticLib"

    vpaths { ["*"] = "*" }

    includedirs
    {
        "./nomad-common",
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

    filter "files:Hooking**.cpp"
        flags {"ExcludeFromBuild"}

    filter "files:nomad**.cpp"
        flags {"ExcludeFromBuild"}

    filter "files:utility**.cpp"
        flags {"ExcludeFromBuild"}
