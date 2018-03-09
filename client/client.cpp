#define MOD_CLIENT

// =======================================================================//
// !
// ! Generic, library, and system includes
// !
// =======================================================================//

// common shared stuff
#include <includes.h>

// system libs
#include <stdio.h>
#include <stdint.h>

 /* TODO: remove some of the deps */
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

// =======================================================================//
// !
// ! Generic types and intefaces
// !
// =======================================================================//

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

librg_ctx_t *ctx;

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
    int z; /* wheel */

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
        state_callback_t *prerender;
    };

    state_callback_t *callbacks[4];
} mod_state_t;
#undef STATE_CB

static f32 test_pos_t = 0.0f;
static f32 test_rot_t = 0.0f;

// base mod data structure
struct mod_t {
    // win stuff
    HMODULE module;
    HWND    window;

    // storage
    mod_path_t  paths;
    mod_state_t state;

    mod_mouse_t     mouse;
    mod_graphics_t  graphics;
    librg_entity_t  *player;

    // game tick props
    f64 last_update;
    f64 last_delta;

    // game related states
    bool input_blocked = true;
    bool spawned;

    // collection of mutexes
    struct {
        zpl_mutex_t log;
        zpl_mutex_t wnd_msg;
    } mutexes;

    struct {
        u32 limit;
        bool enabled;
        std::queue<std::string> queue;
    } console;

    struct {
        u32 streamed_entities;
    } stats;

    // other
    zpl_file_t debug_log;
    std::queue<mod_wndmsg_t> wnd_msg;
};

// public interface definitions
static mod_t mod = {0};

void mod_game_init();
void mod_game_tick();
void mod_log(const char* format, ...);
void mod_exit(std::string);
bool mod_wndproc(HWND, UINT, WPARAM, LPARAM);

// graphics stuff, including dx callbacks
bool graphics_init();
void graphics_terminate();
void graphics_dimensions(int *w, int *h);
void graphics_world_to_screen(vec3_t *, vec3_t);
void graphics_device_create(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
void graphics_device_prerender();
void graphics_device_render();
void graphics_device_lost(IDirect3DDevice9*);
void graphics_device_reset(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

// =======================================================================//
// !
// ! Mod level includes and modules
// !
// =======================================================================//

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

#include "helpers/cubic_hermite.h"

// shared
#include <components.h>
#include <extensions.h>

// actual client stuff
#include "states/title.h"
#include "states/debug.h"

// core stuff
#include "core/graphics.h"
#include "core/pedestrian.h"
#include "core/vehicle.h"
#include "core/discord.h"
#include "core/mod.h"

// NOTE(zaklaus): Tell the OS to prefer dedicated video card.
DWORD NvOptimusEnablement = 0x00000001; // NVIDIA
int AmdPowerXpressRequestHighPerformance = 1; // ATI/AMD

// =======================================================================//
// !
// ! Mod helper methods
// !
// =======================================================================//

/**
 * Register all current mod paths
 * @param module
 */
void mod_path_register(HMODULE module) {
    char temp_path_raw[MAX_PATH] = { '\0' };
    GetModuleFileName(module, temp_path_raw, MAX_PATH);

    auto temp_path = std::string(temp_path_raw);
    auto temp_pos  = temp_path.rfind("\\");

    temp_path.erase(temp_pos, std::string::npos);

    mod.paths.index = temp_path;
    mod.paths.files = temp_path + "\\files";
    mod.paths.debug = temp_path + "\\debug.log";
    mod.paths.game_files = temp_path + "\\game_files";
}

/**
 * Trigger exiting from the mod
 * @param reason
 */
void mod_exit(std::string reason) {
    mod_log("exiting %s\n", reason.c_str());
    mod_game_stop();

    librg_free(ctx);
    delete ctx;

    zpl_file_close(&mod.debug_log);

    zpl_mutex_destroy(&mod.mutexes.log);
    zpl_mutex_destroy(&mod.mutexes.wnd_msg);

    // MessageBoxA(nullptr, reason.c_str(), "Well.. Something went wrong!", MB_OK);

    exit(0);
}

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

    zpl_mutex_lock(&mod.mutexes.log); {
        zpl_printf(message);
        zpl_file_write(&mod.debug_log, message, zpl_strlen(message));

        message[len - 1] = '\0';
        mod.console.queue.push(std::string(message));

        if (mod.console.queue.size() > mod.console.limit) {
            mod.console.queue.pop();
        }

    }
    zpl_mutex_unlock(&mod.mutexes.log);
}


// =======================================================================//
// !
// ! Generic mod entrance methods
// !
// =======================================================================//

/**
 * Trigger attachment to process
 * @param module
 */
void mod_main(HMODULE module) {
    zpl_mutex_init(&mod.mutexes.log);
    zpl_mutex_init(&mod.mutexes.wnd_msg);

    // console, yay
    tools::console_attach();
    tools::console_color_fg(3);
    {
        mod_log("the\nm2o-reborn\n");
        mod_log("starting...\n");
    }
    tools::console_color_fg(7);

    // path n basics
    mod_path_register(module);

    zpl_file_remove(mod.paths.debug.c_str());
    zpl_file_create(&mod.debug_log, mod.paths.debug.c_str());
    zpl_file_seek(&mod.debug_log, 0);

    mod.console.limit   = 256;
    mod.console.enabled = true;

    mod.module = module;

    // allocate ctx
    ctx = new librg_ctx_t;
    zpl_zero_item(ctx);

    // setup manual client mode
    ctx->tick_delay     = 32;
    ctx->mode           = LIBRG_MODE_CLIENT;
    ctx->world_size     = zplm_vec3f(5000.0f, 5000.0f, 0);
    ctx->max_entities   = MOD_ENTITY_LIMIT;

    librg_init(ctx);

    if (graphics_init() == false) {
        return mod_exit("Unable to init Graphics Manager");
    }

    Mem::Initialize();

    mod_log("[info] attaching to thread (%x) ...\n", GetCurrentThreadId());

    tools::filepatcher_install();
    tools::steam_drm_install();
    tools::gamehooks_install();

    // Disable loading screen
    Mem::Utilites::PatchAddress(0x08CA820, 0xC300B0); // mov al, 0; retn

    // Disable DLC loadings (NONO, WE NEED DLCs !)
    //Mem::Utilites::PatchAddress(0x11A62C0, 0xC300B0); // mov al, 0; retn

    if (ExceptionHandler::Install() == false) {
        return mod_exit("Unable to install exception handler");
    }

    // if (m_clientSettings.LoadFile(CClientSettings::DEFAULT_SETTINGS_FILENAME) == false) {
    //     mod_exit("Unable to parse config file");
    // }

    CDirectInput8Hook::Install();
    mod.state = MOD_TITLE_STATE;
}

/**
 * Our main process function
 */
BOOL APIENTRY DllMain(HMODULE module, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(module);
            mod_main(module);
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}
