// common shared stuff
#include "includes.h"

// system libs
#include <stdio.h>
#include <stdint.h>

// bla bla
#include <memory>
#include <limits>
#include <algorithm>
#include <tchar.h>
#include <mutex>
#include <iomanip>

// stream stuff
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <io.h>

// container stuff
#include <list>
#include <vector>
#include <unordered_map>

// dx stuff
#define DIRECTINPUT_VERSION 0x0800
#include <detours.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <TlHelp32.h>

// nuklear
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_D3D9_IMPLEMENTATION
#include <nuklear.h>
#include <nuklear_d3d9.h>

/**
 * Internal dependencies
 * and definitions (mod)
 */
#if !defined(Address)
#define Address unsigned long
#define Pointer unsigned int
#define Byte    unsigned char
#endif

// global vars for nk
struct nk_context*      nk_ctx;
struct nk_font_atlas*   nk_atlas;

// tools
struct mod_path_t {
    std::string index;
    std::string files;
    std::string debug;
    std::string game_files;
};

struct mouse_state_t {
    int x;
    int y;
    short flags;
    struct raw { BYTE x, y; } raw;
};

struct mod_graphics_t {
    IDirect3DDevice9 *device;
    D3DPRESENT_PARAMETERS present_params;
    void *font_manager;
};

// simple FSM system
#define STATE_CB(name) void name()
typedef STATE_CB(state_callback_t);

typedef union mod_state_t {
    struct {
        state_callback_t *init;
        state_callback_t *tick;
        state_callback_t *render;
    };

    state_callback_t *callbacks[3];
} mod_state_t;
#undef STATE_CB

// base mod data structure
struct mod_t {
    // win stuff
    HMODULE module;
    HWND    window;

    // storage
    mod_path_t  paths;
    mod_state_t state;

    mouse_state_t   mouse;
    mod_graphics_t  graphics;
    librg_entity_t  player;

    b32 input_blocked;
    b32 spawned;

    struct {
        zpl_mutex_t log;
    } mutexes;

    // other
    std::ofstream   debug_stream;
};

// public interface definitions
static mod_t mod = {0};

bool mod_init();
void mod_exit(std::string);
bool mod_wndproc(HWND, UINT, WPARAM, LPARAM);

void mod_log(const char* format, ...) {
    va_list ap;
    char message[1024] = { 0 };
    va_start(ap, format);
    vsprintf(message, format, ap);
    va_end(ap);

    zpl_mutex_lock(&mod.mutexes.log);
    zpl_printf(message);
    zpl_mutex_unlock(&mod.mutexes.log);
}

// game events
void game_init();
void game_tick();

// graphics stuff, including dx callbacks
bool graphics_init();
void graphics_terminate();
void graphics_dimensions(int *w, int *h);
void graphics_device_create(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
void graphics_device_prerender();
void graphics_device_render();
void graphics_device_lost(IDirect3DDevice9*);
void graphics_device_reset(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

// hooks
#include "hooks/memory.h"
#include "hooks/steamdrm.h"
#include "hooks/gamehooks.h"
#include "hooks/filepatcher.h"
#include "hooks/dx/CDirect3DDevice9Proxy.h"
#include "hooks/dx/CDirect3D9Proxy.h"
#include "hooks/dx/CDirect3D9Hook.h"
#include "hooks/dx/CDirectInputDevice8Proxy.h"
#include "hooks/dx/CDirectInput8Proxy.h"
#include "hooks/dx/CDirectInput8Hook.h"

// tool stuff
#include "tools/console.h"
#include "tools/file_logger.h"
#include "tools/font_manager.h"
#include "tools/exception_handler.h"

// we need to include sdk
// after memory hooks :C
#include <m2sdk.h>

// shared stuff
#include "components.h"
#include "messages.h"

// actual client stuff
#include "states/title.h"
#include "states/debug.h"
#include "entities/components.h"
#include "entities/vehicle.h"
#include "entities/ped.h"
#include "core/callbacks.h"
#include "core/graphics.h"
#include "core/game.h"
#include "core/mod.h"

/**
 * Our main process function
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);
            mod_attach(hModule);
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}
