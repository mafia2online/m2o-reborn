// =======================================================================//
// !
// ! Hooking
// !
// =======================================================================//

typedef HANDLE(__stdcall* vfs_createfile_cb)(LPCTSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);

struct vfs_state {
    vfs_createfile_cb method;
    std::unordered_map<std::string, std::string> map;
    bool dump_all;
};

static vfs_state _vfs_state;

HANDLE WINAPI vfs_createfile_hook(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
    if (lpFileName == NULL) { return INVALID_HANDLE_VALUE; }
    HANDLE hFile = INVALID_HANDLE_VALUE;

    if (_vfs_state.dump_all) {
        mod_log("[info] vfs: %s\n", lpFileName);
    }

    if (vfs_override_get(lpFileName)) {
        // substiture the file
        mod_log("[info] patching file %s\n", vfs_override_get(lpFileName));
        hFile = _vfs_state.method(vfs_override_get(lpFileName), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

        if (hFile == INVALID_HANDLE_VALUE) {
            mod_log("[error] couldn't patch file %s\n", vfs_override_get(lpFileName));
        }
    }
    else {
        hFile = _vfs_state.method(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    }

    return hFile;
}

// =======================================================================//
// !
// ! Control methods
// !
// =======================================================================//

void vfs_init() {
    _vfs_state.method = (vfs_createfile_cb)Mem::Hooks::InstallDetourPatch("kernel32.dll", "CreateFileA", (uint32_t)vfs_createfile_hook);
}

void vfs_free() {
    Mem::Hooks::UninstallDetourPatch(_vfs_state.method, (DWORD)vfs_createfile_hook);
}

void vfs_dump_all(bool value) {
    _vfs_state.dump_all = value;
}

void vfs_override_set(const char *src, const char *dst) {
    _vfs_state.map[std::string(src)] = std::string(dst);
}

char *vfs_override_get(const char *src) {
    std::string temp_filename(src);
    std::string filename = temp_filename.substr(
        temp_filename.find_last_of("/\\") + 1
    );

    auto pair = _vfs_state.map.find(filename);
    return pair != _vfs_state.map.end() ? (char *)pair->second.c_str() : nullptr;
}
