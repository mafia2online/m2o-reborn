std::string sds_patch_files[] = {
    "sdsconfig.bin",
    "StreamMapa.bin",
    "tables.sds",
    "gui.sds",
    "gui-main.sds"
};


typedef HANDLE(__stdcall* tCreateFileA)(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
tCreateFileA orig_CreateFileA = nullptr;
HANDLE WINAPI CreateFileAHooked(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    if (lpFileName == NULL)
        return INVALID_HANDLE_VALUE;

    HANDLE hFile = INVALID_HANDLE_VALUE;
    bool file_found = false;

    std::string strFileName(lpFileName);
    std::string str(strFileName);
    strFileName = str.substr(str.find_last_of("/\\") + 1);

    for (auto filename : sds_patch_files) {
        if (strFileName == filename) {
            file_found = true;
        }
    }

    if (file_found) {
        std::string strNewDir(CCore::Instance().GetModAppDir());
        strNewDir += "\\game_files\\";
        strNewDir += strFileName;
        corelog("FilePatcher: Redirecting game file %s to %s", strFileName.c_str(), strNewDir.c_str());
        hFile = orig_CreateFileA(strNewDir.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

        if (hFile == INVALID_HANDLE_VALUE) {
            game_exit(std::string("Couldn't patch file: ") + strFileName);
        }
    }
    else
    {
        hFile = orig_CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    }

    return hFile;
}


void FilePatcher_Initialize()
{
    orig_CreateFileA = (tCreateFileA)Mem::Hooks::InstallDetourPatch("kernel32.dll", "CreateFileA", (Address)CreateFileAHooked);
}
