project "m2framework"
    language "C++"
    kind "StaticLib"

    vpaths
    {
        ["Headers/*"] = "**.hpp",
        ["Headers/*"] = "**.h",
        ["Sources/*"] = "**.cpp",
        ["Sources/*"] = "**.c",
        ["*"] = "premake5.lua"
    }

    files
    {
        "premake5.lua",
        "**.cpp",
        "**.hpp",
        "**.h",
        "**.c"
    }

    includedirs
    {
        "."
    }
