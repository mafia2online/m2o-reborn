
#include <PlusGame.h>
#include <Ldr/ExeLDR.h>

#include <winternl.h>
#include <mutex>
#include <string>

#include <Utility/PathUtils.h>
#include <Hooking.h>

#include <MinHook.h>

inline void InitializeModules()
{   
    auto mod = LoadLibraryW(Utility::MakeAbsolutePathW(L"core-client.dll").c_str());

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
    return (exceptionInfo->ExceptionRecord->ExceptionCode == STATUS_INVALID_HANDLE) ? EXCEPTION_CONTINUE_EXECUTION : EXCEPTION_CONTINUE_SEARCH;
}


void InvokeEP(void(*ep)())
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

LPWSTR WINAPI GetCommandLineW_Hook()
{
    static std::once_flag only_once;

    std::call_once(only_once, InitializeModules);

    return GetCommandLineW();
}

void WINAPI RaiseException_Hook(DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments, const ULONG_PTR *lpArguments)
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

static bool yes()
{
    printf("yes stub!\n");

    return true;
}

static bool no()
{
    printf("no stuB!\n");

    return false;
}

static const wchar_t *g_img_path;

DWORD WINAPI GetModuleFileNameW_Hook(HMODULE hModule, LPWSTR lpFilename, DWORD nSize)
{
    if (!hModule)
    {
        wcscpy_s(lpFilename, nSize, g_img_path);

        return (DWORD)wcslen(g_img_path);
    }

    return GetModuleFileNameW(hModule, lpFilename, nSize);
}

void PlusGame::Launch(const wchar_t *game_dir)
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

    printf("Loaded game with size %ld\n", length);

    auto base = GetModuleHandle(nullptr);
    ExecutableLoader loader(data);

    // as no stub memory is required
    loader.SetLoadLimit(0x30000000);

    loader.SetLibraryLoader([](const char* library) -> HMODULE
    {
        return LoadLibraryA(library);
    });

    loader.SetFunctionResolver([](HMODULE hmod, const char* exportFn) -> LPVOID
    {
        // pre crt init hook
        if (!_strcmpi(exportFn, "GetCommandLineW"))
        {
            return static_cast<LPVOID>(&GetCommandLineW_Hook);
        }

        // *temp hack*
        if (!_strcmpi(exportFn, "RaiseException"))
        {
            return static_cast<LPVOID>(&RaiseException_Hook);
        }

        // *fix pathing*
        if (!_strcmpi(exportFn, "GetModuleFileNameW"))
        {
            return static_cast<LPVOID>(&GetModuleFileNameW_Hook);
        }

        return static_cast<LPVOID>(GetProcAddress(hmod, exportFn));
    });

    loader.LoadIntoModule(base);

    // store entry
    auto entry_point = static_cast<void(*)()>(loader.GetEntryPoint());

    // dont need this anymore
    delete[] data;

    // initialize nio
    nio::set_base(reinterpret_cast<uintptr_t>(base));

    // this is here *temporarily* and needs to be replaced with patterns
    {
        nio::put_call(0x00401D15, yes);
    //    nio::nop(0x00401D63, 5);

        nio::nop(0x00401CFB, 6);
        nio::put_call(0x00401CFB, no);

        nio::return_function(0x1872780);

        MH_Initialize();
    }

    // invoke EP
    entry_point();
}
