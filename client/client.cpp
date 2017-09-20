#define MOD_CLIENT

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
#include <queue>
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
#define NK_BUTTON_TRIGGER_ON_RELEASE
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
struct nk_context*    nk_ctx;
struct nk_font_atlas* nk_atlas;

// tools
struct mod_path_t {
    std::string index;
    std::string files;
    std::string debug;
    std::string game_files;
};

typedef struct {
    nk_buttons id;
    int state;
} mod_mousebtn_t;

typedef struct {
    HWND hWnd;
    UINT uMsg;
    WPARAM wParam;
    LPARAM lParam;
} mod_wndmsg_t;

typedef struct {
    int x;
    int y;

    struct _DIMOUSESTATE state;

    union {
        struct {
            mod_mousebtn_t left;
            mod_mousebtn_t right;
            mod_mousebtn_t middle;
        } btn;

        mod_mousebtn_t buttons[3];
    };
} mod_mouse_t;

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

    mod_mouse_t     mouse;
    librg_entity_t  player;
    mod_graphics_t  graphics;

    // game tick props
    u64 last_update;
    f32 last_delta;

    // game related states
    b32 input_blocked;
    b32 spawned;

    // collection of mutexes
    struct {
        zpl_mutex_t log;
        zpl_mutex_t wnd_msg;
    } mutexes;

    // other
    std::ofstream   debug_stream;
    std::queue<mod_wndmsg_t> wnd_msg;
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
    mod.debug_stream << message;
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

#include "hooks/messages.h"

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

#include "core/model.h"

// actual client stuff
#include "states/title.h"
#include "states/debug.h"

// "modules"
#include "modules/module_ped.h"
#include "modules/module_car.h"

#include "core/graphics.h"
#include "core/game.h"
#include "core/mod.h"

//#include "hacks.h"

// NOTE(zaklaus): Tell the OS to prefer dedicated video card.
DWORD NvOptimusEnablement = 0x00000001; // NVIDIA
int AmdPowerXpressRequestHighPerformance = 1; // ATI/AMD

/**
 * Our main process function
 */
BOOL APIENTRY DllMain(HMODULE module, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(module);
            mod_attach(module);
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}
