#define _CRT_SECURE_NO_WARNINGS
//#define WIN32_LEAN_AND_MEAN
#define NOMINMAX // std::numeric_limits min&max

#include <stdio.h>
#include <stdint.h>

// bla bla
#include <memory>
#include <limits>
#include <algorithm>
#include <tchar.h>
#include <mutex>
#include <iomanip>

// steam stuff
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <io.h>

// container stuff
#include <string>
#include <vector>
#include <list>
#include <unordered_map>

// librg
#include <librg/librg.h>
#include <librg/utils/fs.h>

#define define_callback(name, event_type, code) \
    void name(librg::events::event_t* evt) { \
        auto event = (event_type*) evt; \
        code; \
    }

// mod-level definition stuff
#if !defined(Address)
#define Address unsigned long
#define Pointer unsigned int
#define Byte    unsigned char
#endif

// proxy some stuff
typedef hmm_vec2 Vector2;
typedef hmm_vec3 Vector3;

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

#define STATE_CB(name) void name(void *)
typedef STATE_CB(state_callback_t);

struct mod_state_t {
    union {
        struct {
            state_callback_t *init;
            state_callback_t *tick;
            state_callback_t *render;
        };

        state_callback_t *callbacks[3];
    };
};
#undef STATE_CB

// base mod data structure
struct mod_t {
    // win
    HWND            window;
    HMODULE         module;

    // settings
    mod_path_t      paths;

    // containers
    librg::entity_t player;
    mod_state_t     state;

    // other
    mouse_state_t   mouse;
    std::ofstream   debug_steam;
};

// public interface definitions
static mod_t mod;

void mod_init();
void mod_exit(std::string);
bool mod_wndproc(HWND, UINT, WPARAM, LPARAM);

#define mod_log librg::core::log

// game events
void game_init();
void game_tick(librg::events::event_t*);

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

struct nk_font_atlas* nk_atlas;
struct nk_context* nk_ctx;

// tool stuff
#include "tools/console.h"
#include "tools/patcher.h"
#include "tools/file_patcher.h"
#include "tools/steam_drm.h"
#include "tools/game_hooks.h"
#include "tools/file_logger.h"
#include "tools/raw_input.h"
#include "tools/singleton.h" // ohhh nooo
#include "tools/exception_handler.h"
#include "tools/dx/CDirect3DDevice9Proxy.h"
#include "tools/dx/CDirect3D9Proxy.h"
#include "tools/dx/CDirect3D9Hook.h"
#include "tools/dx/CDirectInputDevice8Proxy.h"
#include "tools/dx/CDirectInput8Proxy.h"
#include "tools/dx/CDirectInput8Hook.h"

#include <m2sdk.h>

// todo: refactor
M2::C_Player2 *dwLocalPlayer = nullptr;
M2::C_Human2 *ent = nullptr;
librg::entity_t local_player;
float ztime = 0;

// shared stuff
#include "shared_defines.h"
#include "messages.h"

// actual client stuff
#include "gfx/CDebugConsole.h"
#include "gfx/CFontManager.h"
#include "gfx/CGraphicsManager.h"
#include "states/title.h"
#include "states/game.h"
#include "entities/ped.h"
#include "entities/vehicle.h"
#include "proxies.h" // todo: remove
#include "game.h"
#include "callbacks.h"
#include "mod.h"

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
