return {
    include = function()

    defines
    {
        'USING_CEF_SHARED',
        'WRAPPING_CEF_SHARED',
        'WIN32_LEAN_AND_MEAN',
        'NOMINMAX',
        'WIN32'
    }

    end,

    run = function()
        targetname "cefwrapper"
        language "C++"
        kind "StaticLib"

        -- dont question this output path!
        targetdir "../bin/vendor/%{cfg.buildcfg}"

        local cef_base = "vendor/cef/" .. CEF_VERSION .. "/libcef_dll/"

        includedirs {
            cef_base,
            cef_base .. "../",
        }

        -- dont question those paths.
        files {
            cef_base .. "/**.hpp",
            cef_base .. "/**.h",
            cef_base .. "/cerwrapper.cc",
        }

        -- single source cef wrapper (unity build)
        local cnt = ""
        local p = os.matchfiles(cef_base .. "/**.cc")
        for _, v in pairs(p) do
            local name = v:gsub(cef_base, "")
            if name ~= "cerwrapper.cc" then
                cnt = cnt .. "#include \"" .. name .. "\"\n"
            end
        end
        io.writefile(cef_base .. "/cerwrapper.cc", cnt)

    end
}
