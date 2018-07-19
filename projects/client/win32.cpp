#define M2O_CLIENT
#include "m2o_config.h"

#include <stdint.h>
#include <clocale>
#include <unordered_map>

#include "zpl.h"
#include "zpl_math.h"
#include "nuklear.h"

// instead of including whole m2sdk.h, we can just use this:
namespace M2 {
    void Initialize(void (*)(void));
    void *GetCameraWorldViewProjection();
};
#include "../m2sdk/include/utils/Memory.hpp"

static std::string mod_path;

#include "m2o_client.h"
#include "win32/gfx_impl.hpp"
#include "win32/vfs_impl.hpp"
#include "win32/input_impl.hpp"
#include "win32/exceptions_impl.hpp"

extern "C" {
    // NOTE: Tell the OS to prefer dedicated video card.
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001; // NVIDIA
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1; // ATI/AMD
}

// =======================================================================//
// !
// ! Generic windproc hooking
// !
// =======================================================================//

typedef WNDPROC wnd_wndproc_cb;
wnd_wndproc_cb mod_wndproc_original;

LRESULT __stdcall mod_wndproc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // mod_log("mod_wndproc_hook %x %u %u %u\n", hWnd, uMsg, wParam, lParam);
    return CallWindowProc(mod_wndproc_original, hWnd, uMsg, wParam, lParam);
}

void platform_free() {}
void platform_init() {
    HWND hWnd = *(HWND *)((*(DWORD*)0x1ABFE30) + 28);
    SetWindowText(hWnd, M2O_VERSION_PRETTY);

    // hook the wind proc
    mod_wndproc_original = (wnd_wndproc_cb)SetWindowLongPtr(hWnd, GWL_WNDPROC, (LONG_PTR)mod_wndproc_hook);
    SetWindowLongW(hWnd, GWL_WNDPROC, GetWindowLong(hWnd, GWL_WNDPROC));
}

// =======================================================================//
// !
// ! Logging
// !
// =======================================================================//

zpl_file_t debug_log;
zpl_mutex_t debug_log_mutex;

void mod_log(const char* format, ...) {
    va_list ap;
    char message[2048] = { 0 };
    va_start(ap, format);
    vsprintf(message, format, ap);
    va_end(ap);

    // add auto new line at the end
    usize len = zpl_strlen(message);
    if (message[len - 1] != '\n' && len < 2048) {
        message[len + 0] = '\n';
        message[len + 1] = '\0';
        len++;
    }

    zpl_mutex_lock(&debug_log_mutex); {
        zpl_printf(message);
        zpl_file_write(&debug_log, message, zpl_strlen(message));

        // message[len - 1] = '\0';
        // mod.console.queue.push(std::string(message));

        // if (mod.console.queue.size() > mod.console.limit) {
        //     mod.console.queue.pop();
        // }

    }
    zpl_mutex_unlock(&debug_log_mutex);
}

// =======================================================================//
// !
// ! Platform entry point
// !
// =======================================================================//

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID lpReserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH: {
            #ifdef M2O_DEBUG
            AllocConsole();
            AttachConsole(GetCurrentProcessId());
            DisableThreadLibraryCalls(module);

            // Set our locale to the C locale, as Unicode output only functions in this locale
            std::setlocale(LC_ALL, "C");
            SetConsoleOutputCP(CP_UTF8);

            // Relay Input/Output
            FILE* x;
            freopen_s(&x, "CONOUT$", "w", stdout);
            freopen_s(&x, "CONIN$", "r", stdin);

            // Title
            HWND hConsole = GetConsoleWindow();
            SetConsoleTitle("m2o-reborn-cli");

            // Position
            RECT rect;
            GetWindowRect(hConsole, &rect);
            SetWindowPos(hConsole, NULL, 20, 20, 800, 600, 0);
            #endif

            char temp_path_raw[MAX_PATH] = { '\0' };
            GetModuleFileName(module, temp_path_raw, MAX_PATH);

            auto temp_path = std::string(temp_path_raw);
            auto temp_pos  = temp_path.rfind("\\");

            auto modpath = temp_path.erase(temp_pos, std::string::npos);

            mod_path = modpath;

            zpl_mutex_init(&debug_log_mutex);
            zpl_file_remove((modpath + "\\debug.log").c_str());
            zpl_file_create(&debug_log, (modpath + "\\debug.log").c_str());
            zpl_file_seek(&debug_log, 0);

            mod_log("[info] attaching to thread (%x) ...\n", GetCurrentThreadId());
            mod_log("[info] starting %s ...\n", M2O_VERSION_PRETTY);

            /* attach custom exception handler */
            exceptions_init((modpath + "\\exceptions").c_str());


            /* attach gfx rendering hooks */
            gfx_init();
            gfx_font_add(0, (modpath + "\\files\\Roboto-Regular.ttf").c_str());

            /* init vfs for file overrides */
            vfs_init();
            vfs_override_set("gui.sds",         (modpath + "\\files\\gui.sds").c_str());
            vfs_override_set("gui-main.sds",    (modpath + "\\files\\gui-main.sds").c_str());
            vfs_override_set("tables.sds",      (modpath + "\\files\\tables.sds").c_str());
            vfs_override_set("sdsconfig.bin",   (modpath + "\\files\\sdsconfig.bin").c_str());
            vfs_override_set("StreamMapa.bin",  (modpath + "\\files\\StreamMapa.bin").c_str());

            /* attach input manager hooks */
            input_init();

            M2::Initialize(mod_install);
        }
        break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}
