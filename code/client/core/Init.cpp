#include <Windows.h>

#include <stdio.h>
#include <chrono>

#include <MinHook.h>
#include <Hooking.h>

extern "C" __declspec(dllexport) void RunClient()
{
    MH_Initialize();

    // run init function registery
    {
        auto now = std::chrono::steady_clock::now();

        nomad::base_function::run_all();

        auto then = std::chrono::steady_clock::now();

        printf("[Core] Basefunctions -> %lld ms\n",
            std::chrono::duration_cast<std::chrono::milliseconds>(then - now).count());
    }
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(instance);
    }

    return TRUE;
}

