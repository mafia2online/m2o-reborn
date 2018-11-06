#include <host.h>
#include <Ldr/ExeLDR.h>

#include <winternl.h>
#include <mutex>
#include <string>

#include <utility/PathUtils.h>
#include <Hooking.h>

inline void InitializeModules()
{
    auto mod = LoadLibraryW(Utility::MakeAbsolutePathW(L"client-core.dll").c_str());

    if (mod)
    {
        auto init = reinterpret_cast<void*(*)()>(GetProcAddress(mod, "RunClient"));

        if (init)
        {
            init();
        }
    }
}

static LONG NTAPI HandleVariant(PEXCEPTION_POINTERS exceptionInfo)
{
    return (exceptionInfo->ExceptionRecord->ExceptionCode == STATUS_INVALID_HANDLE)
               ? EXCEPTION_CONTINUE_EXECUTION
               : EXCEPTION_CONTINUE_SEARCH;
}


void InvokeEP(void (*ep)())
{
    __try
    {
        // and call the entry point
        ep();
    }
    __except (HandleVariant(GetExceptionInformation()))
    {
    }
}

LPSTR WINAPI GetCommandLineA_Hook()
{
    static bool init = false;

    if (!init)
    {
        InitializeModules();
        init = true;
    }

    return GetCommandLineA();
}

void WINAPI RaiseException_Hook(DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments,
                                const ULONG_PTR* lpArguments)
{
#if 1
    if (dwExceptionCode == 0x406D1388 && !IsDebuggerPresent())
    {
        //printf("THREAD EX BY %p\n", _ReturnAddress());
        return; // thread naming
    }

#endif

    RaiseException(dwExceptionCode, dwExceptionFlags, nNumberOfArguments, lpArguments);
}

static const wchar_t* g_img_path;

DWORD WINAPI GetModuleFileNameW_Hook(HMODULE hModule, LPWSTR lpFilename, DWORD nSize)
{
    if (!hModule)
    {
        wcscpy_s(lpFilename, nSize, g_img_path);

        return (DWORD)wcslen(g_img_path);
    }

    return GetModuleFileNameW(hModule, lpFilename, nSize);
}

void ApplyRunPatch()
{
    char* loc = nio::pattern("81 EC ? ? ? ? 53 33 DB 68 ? ? ? ? 53").first();
    nio::put_ljump(loc, loc + 0x1AC);
}

void Host::Launch(const wchar_t* game_dir)
{
    FILE* file = _wfopen(game_dir, L"rb");

    if (!file)
    {
        MessageBoxA(nullptr, "Failed to find executable image", FXNAME, MB_ICONERROR);
        return;
    }

    g_img_path = game_dir;

    // load the game into a buffer
    fseek(file, 0, SEEK_END);
    auto length = ftell(file);
    auto data = new uint8_t[length];
    fseek(file, 0, SEEK_SET);
    fread(data, 1, length, file);
    fclose(file);

    printf("[Host] Loaded Game (%ld MB)\n", length / 1024 / 1024);

    auto base = GetModuleHandle(nullptr);
    ExecutableLoader loader(data);

    // as no stub memory is required
    loader.SetLoadLimit(0x2500000);

    loader.SetLibraryLoader([](const char* library) -> HMODULE
    {
        return LoadLibraryA(library);
    });

    loader.SetFunctionResolver([](HMODULE hmod, const char* exportFn) -> LPVOID
    {
        // early init hook
        if (!_strcmpi(exportFn, "GetCommandLineA"))
        {
            return static_cast<LPVOID>(GetCommandLineA_Hook);
        }

        // *temp hack*
        if (!_strcmpi(exportFn, "RaiseException"))
        {
            return static_cast<LPVOID>(RaiseException_Hook);
        }

        // *fix pathing*
        if (!_strcmpi(exportFn, "GetModuleFileNameW"))
        {
            return static_cast<LPVOID>(GetModuleFileNameW_Hook);
        }

        return static_cast<LPVOID>(GetProcAddress(hmod, exportFn));
    });

    loader.LoadIntoModule(base);

    // store entry
    auto entry_point = static_cast<void(*)()>(loader.GetEP());

    // dont need this anymore
    delete[] data;

    // initialize nio
    nio::set_base(reinterpret_cast<uintptr_t>(base));

    // dont create a mutex, dont check for launcher
    ApplyRunPatch();

    // invoke EP
    entry_point();
}
