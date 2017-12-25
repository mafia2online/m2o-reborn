namespace tools {

    /**
     * Callback type
     * describes function needed for pathcing the CreateFileA method
     */
    typedef HANDLE(__stdcall* filepatcher__callback_t) (LPCTSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);

    /**
     * Array of files to patch
     */
    std::string filepatcher_sds_files[] = {
        "sdsconfig.bin",
        "StreamMapa.bin",
        "tables.sds",
        "gui.sds",
        "gui-main.sds"
    };

    /**
     * Store for our pathched method
     * @private
     */
    filepatcher__callback_t filepatcher__original = nullptr;

    /**
     * Our own callback, that we will use to
     * patch and replace CreateFileA with
     */
    HANDLE WINAPI filepatcher__handler(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
    {
        if (lpFileName == NULL) {
            return INVALID_HANDLE_VALUE;
        }

        HANDLE hFile = INVALID_HANDLE_VALUE;
        bool file_found = false;

        // get and trim the paths
        std::string temp_filename(lpFileName);
        std::string filename = temp_filename.substr(
            temp_filename.find_last_of("/\\") + 1
        );

        // mod_log("[GAME]%s\n", temp_filename.c_str());

        for (auto sds_file : filepatcher_sds_files) {
            if (filename == sds_file) file_found = true;
        }

        if (file_found) {
            std::string newfilename = mod.paths.game_files + "\\" + filename;

            // substiture the file
            mod_log("file_patcher: redirecting game file %s to %s\n", filename.c_str(), newfilename.c_str());
            hFile = filepatcher__original(newfilename.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

            if (hFile == INVALID_HANDLE_VALUE) {
                mod_exit(std::string("file_patcher: couldn't patch file: ") + filename);
            }
        }
        else {
            hFile = filepatcher__original(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
        }

        return hFile;
    }

    /**
     * Method for patching
     */
    void filepatcher_install() {
        filepatcher__original = (filepatcher__callback_t)Mem::Hooks::InstallDetourPatch(
            "kernel32.dll", "CreateFileA", (uint32_t)filepatcher__handler
        );
    }
}
