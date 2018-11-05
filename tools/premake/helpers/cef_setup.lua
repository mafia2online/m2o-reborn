function progress(total, current)
    local ratio = current / total;
    ratio = math.min(math.max(ratio, 0), 1);
    local percent = math.floor(ratio * 100);
    print("Cef Download progress (" .. percent .. "%/100%)")
end

function verifycef(version)
    -- cmd needs absolute pathing
    local abs_path = path.getabsolute('')

    -- ensure path existence
    if os.isdir("vendor/cef") or os.isdir("vendor/cef/cur_dl") then
        print("cef dir allready exists, skipping step")
        1
        return;
    else

        os.mkdir("vendor/cef")
        os.mkdir("vendor/cef/cur_dl")

        print("installing cef.")
    end

    -- finally, do the download
    local target_url = "http://opensource.spotify.com/cefbuilds/" .. version .. ".tar.bz2"
    local result_str, response_code = http.download(target_url, "vendor/cef/cur_dl/cef.tar.bz2", {
        progress = progress,
        headers = { "From: Premake", "Referer: Premake" },
        userpwd = ""
    })

    -- and unpack it
    if os.host() == "windows" then
        local sz_exe = abs_path .. "/build/win/7z.exe"
        local src = abs_path .. "/vendor/cef/cur_dl/cef.tar.bz2"
        local dest = abs_path .. "/vendor/cef/cur_dl"

        os.executef("%s e %s -o%s *.tar", sz_exe, src, dest)

        -- extract the tarball
        src = abs_path .. "/vendor/cef/cur_dl/cef.tar"
        dest = abs_path .. "/vendor/cef"

        os.executef("%s x %s -o%s %s\\*.* -r", sz_exe, src, dest, version)
    end

    os.rmdir("vendor/cef/cur_dl")

    -- now copy the lib files to the bin dir
    local cef_dir = "vendor/cef/" .. version

    os.mkdir("../bin/vendor/Debug")
    os.mkdir("../bin/vendor/Release")

    os.copyfile(cef_dir .. "/Debug/cef_sandbox.lib", "../bin/vendor/Debug/cef_sandbox.lib")
    os.copyfile(cef_dir .. "/Debug/libcef.lib", "../bin/vendor/Debug/libcef.lib")

    os.copyfile(cef_dir .. "/Release/cef_sandbox.lib", "../bin/vendor/Release/cef_sandbox.lib")
    os.copyfile(cef_dir .. "/Release/libcef.lib", "../bin/vendor/Release/libcef.lib")

    return;
end
