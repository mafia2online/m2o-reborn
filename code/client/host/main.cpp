#include <Windows.h>
#include <shellscalingapi.h>
#include <cstdio>

#include <utility/PathUtils.h>
#include <utility/WinRegistry.h>

#include "plusgame.h"
#include "plusgame.cpp"

#define EXE L"//mafia2.exe"
#define KEY L"maf2_path"

extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

extern "C"
{
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

void OpenConsole()
{
    // Console
    AllocConsole();
    AttachConsole(GetCurrentProcessId());

    SetConsoleTitleW(FXNAME_WIDE " Dev Console");

    freopen("CON", "w", stdout);
    freopen("CONIN$", "r", stdin);
}

int wmain()
{
    OpenConsole();

    auto localpath = Utility::MakeAbsolutePathW(L"");

    auto check = [=](const std::wstring& str)
    {
        if (GetFileAttributesW((localpath + str).c_str()) == INVALID_FILE_ATTRIBUTES)
        {
            CreateDirectoryW((localpath + str).c_str(), nullptr);
        }
    };

    const std::wstring file_structure[2] = {
        L"data",
        L"bin"
    };

    for (const auto& file : file_structure)
    {
        check(file);
    }

    // readout path and fx config
    static wchar_t game_dir[MAX_PATH + 20] = { 0 };

    std::wstring fullpath = localpath + L"fxconfig.ini";
    GetPrivateProfileStringW(L"fxconfig", KEY, nullptr, game_dir, MAX_PATH, fullpath.c_str());

    if (!game_dir[0])
    {
        MessageBoxW(nullptr, L"Failed to read FX game path!", FXNAME_WIDE, MB_ICONERROR);
        return 0;
    }

    auto addDllDirectory = (decltype(&AddDllDirectory))GetProcAddress(
        GetModuleHandle(L"kernel32.dll"), "AddDllDirectory");
    auto setDefaultDllDirectories = (decltype(&SetDefaultDllDirectories))GetProcAddress(
        GetModuleHandle(L"kernel32.dll"), "SetDefaultDllDirectories");

    if (addDllDirectory && setDefaultDllDirectories)
    {
        setDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | LOAD_LIBRARY_SEARCH_USER_DIRS);

        // additional search directories
        AddDllDirectory(game_dir);
        AddDllDirectory(localpath.c_str());

        localpath += L"bin";
        AddDllDirectory(localpath.c_str());

        // to prevent a) current directory DLL search being disabled and b) xlive.dll being taken from system if not overridden
     //   SetDllDirectoryW(localpath.c_str());

        // game could expect relative pathing
       // SetCurrentDirectoryW(game_dir);
        SetCurrentDirectoryW(game_dir);
    }

    // use real scaling
    {
        auto shcore = LoadLibrary(L"shcore.dll");

        if (shcore)
        {
            auto SetProcessDpiAwareness = (decltype(&::SetProcessDpiAwareness))GetProcAddress(shcore, "SetProcessDpiAwareness");

            if (SetProcessDpiAwareness)
            {
                SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
            }
        }
    }

    // handle path variable
    {
        static wchar_t pathBuf[32768];
        GetEnvironmentVariable(L"PATH", pathBuf, sizeof(pathBuf));

        // append bin & game directorys
        std::wstring newPath = localpath + L";" + game_dir + L";" + std::wstring(pathBuf);
        SetEnvironmentVariable(L"PATH", newPath.c_str());
    }

    // fix some PhysX bug
    {
        // place "cudart32_65.dll" in your /bin/ directory, if not present through nvidia installation!
        wchar_t buf[512];
        if (Utility::ReadStringW(HKEY_LOCAL_MACHINE, L"Software\\AGEIA Technologies\\", L"PhysXCore Path", L"cudart32_65.dll", buf, sizeof(buf)))
        {
            auto ptr = wcsstr(buf, L"Engine");
            wcscpy(ptr, L"Common\\cudart32_65.dll");
        }

        wprintf(L"[Host] Nvidia fix (%s)\n", buf);
        LoadLibraryW(buf);
    }

    lstrcatW(game_dir, EXE);
    wprintf(L"[Host] Game dir (%s)\n", game_dir);

    plusgame::launch(game_dir);

    return 0;
}
