
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
        nio::nop(0x00401D63, 5);

        nio::nop(0x00401CFB, 6);
        nio::put_call(0x00401CFB, no);

        nio::return_function(0x1872780);

        LoadLibraryA("C:\\Program Files (x86)\\NVIDIA Corporation\\PhysX\\Engine\\73BE2921551D\\ImplicitLibs\\cudart32_65.dll");

        // Set this application as not being debugged
        PPEB peb = (PPEB)__readfsdword(0x30);
        peb->BeingDebugged = false;
        //*(DWORD*)((char*)peb + 0xBC) &= ~0x70;

        HMODULE hNtDll = LoadLibraryW(L"ntdll.dll");
        uint8_t* ptr = (uint8_t*)GetProcAddress(hNtDll, "KiRaiseUserExceptionDispatcher");
        DWORD oldProtect;
        VirtualProtect(ptr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
        *ptr = 0xC3;

        static auto fuckyou = []() -> void {
            printf("FUCK YOU");
            __debugbreak();
        };

        static auto fuckyoubool = []() -> boolean {
            printf("FUCK YOU #2");
            __debugbreak();
            return false;
        };

        HMODULE hKernel32 = LoadLibraryW(L"kernel32.dll");
        uint8_t* ptr2 = (uint8_t*)GetProcAddress(hKernel32, "TerminateProcess");
        uint8_t* ptr2_orig = nullptr;

        uint8_t* ptr3 = (uint8_t*)GetProcAddress(hKernel32, "ExitProcess");
        uint8_t* ptr3_orig = nullptr;

        uint8_t* ptr4 = (uint8_t*)GetProcAddress(hKernel32, "IsDebuggerPresent");
        uint8_t* ptr4_orig = nullptr;

        auto faggot = MH_Initialize();
        if (faggot != MH_OK)
            __debugbreak();

        /*VirtualProtect(ptr2, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
        VirtualProtect(ptr3, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
        VirtualProtect(ptr4, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

        *ptr2 = 0xC3CC;
        *ptr3 = 0xC3CC;
        *ptr4 = 0xC3CC;*/

        auto statuscr = MH_CreateHook(ptr2, (LPVOID)&fuckyou, (LPVOID*)&ptr2_orig);

        if (statuscr != MH_OK)
            __debugbreak();

        auto status = MH_EnableHook(ptr2);

        if (status != MH_OK)
            __debugbreak();

        MH_CreateHook(ptr3, (LPVOID)&fuckyou, (LPVOID*)&ptr3_orig);
        MH_EnableHook(ptr3);
        
        MH_CreateHook(ptr4, (LPVOID)&fuckyoubool, (LPVOID*)&ptr4_orig);
        MH_EnableHook(ptr4);

        nio::return_function(0x00450520);
    }

    // invoke EP
    entry_point();

    __debugbreak();
}
