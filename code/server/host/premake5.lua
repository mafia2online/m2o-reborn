-- MPLUS configuration script
-- Author(s): Force67
--
-- Copyright (c) 2015-2018 Dev-Force
--
-- Distributed under the MIT license (See accompanying file LICENSE or copy at
-- https://opensource.org/licenses/MIT)

-- This is the root build file for Premake. Premake will start processing by loading this
-- file, and recursively load all dependencies until all dependencies are either
-- resolved or known not to exist (which will cause the build to fail). So if
-- you add a new build file, there must be some path of dependencies from this
-- file to your new one or Premake won't know about it.

project "server-host"
    language "C++"
    kind "ConsoleApp"

    targetname "server-host"

    vpaths { ["*"] = "*" }

    -- why like this tho
    if os.target() == "linux" then
        linkoptions { "-ldl -pthread -fvisibility=hidden" }
    	targetdir ("../../../bin/_sv_linux/%{cfg.buildcfg}/")
	else
        targetdir ("../../../bin/_sv/%{cfg.buildcfg}/")
	end

    libdirs {}

    includedirs {
        "../../shared",
        "./include",
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
    }

	if os.target() == "windows" then
		files "**.rc"
	end
