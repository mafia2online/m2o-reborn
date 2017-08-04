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

// mod-level definition stuff
#if !defined(Address)
#define Address unsigned long
#define Pointer unsigned int
#endif

using Byte = unsigned char;

// proxy some stuff
typedef hmm_vec2 Vector2;
typedef hmm_vec3 Vector3;

struct mouse_pos {
    BYTE x, y;
};

// public interface definitions
void game_init();
void game_exit(std::string reason);
void game_on_init();
void game_on_tick();
bool game_on_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND global_window;

std::string mod_dir;
std::string mod_files_dir;

#define corelog librg::core::log

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
#include "tools/steam_drm.h"
#include "tools/game_hooks.h"
#include "tools/file_patcher.h"
#include "tools/file_logger.h"
#include "tools/raw_input.h"
#include "tools/singleton.h" // ohhh nooo
#include "tools/exception_handler.h"

#include <m2sdk.h>

// todo: refactor
M2::C_Player2 *dwLocalPlayer = nullptr;
M2::C_Human2 *ent = nullptr;
librg::entity_t local_player;
std::ofstream _debug_stream;
float ztime = 0;
HMODULE dll_module;

struct mouse_state {
    int x;
    int y;
    short flags;
};

static mouse_state global_mouse_state;

// shared stuff
#include "shared_defines.h"
#include "messages.h"

// actual client stuff
#include "callbacks/tick.h"
#include "callbacks/other.h"
#include "callbacks/entity_create.h"
#include "callbacks/entity_update.h"
#include "callbacks/entity_interpolate.h"
#include "callbacks/entity_remove.h"
#include "callbacks/clientstream_update.h"
#include "dx/CDirect3DDevice9Proxy.h"
#include "dx/CDirect3D9Proxy.h"
#include "dx/CDirect3D9Hook.h"
#include "dx/CDirectInputDevice8Proxy.h"
#include "dx/CDirectInput8Proxy.h"
#include "dx/CDirectInput8Hook.h"
#include "gfx/CMPStateManager.h"
#include "gfx/CDebugConsole.h"
#include "gfx/CFontManager.h"
#include "gfx/CGraphicsManager.h"
#include "gfx/CTitleState.h"
#include "proxies.h"
#include "game.h"


void mod_onlog(librg::events::event_t* evt) {
    auto event = (librg::events::event_log_t*) evt;
    _debug_stream << event->output;
    printf("%s", event->output.c_str());
}

void mod_on_attach(HMODULE module)
{
    dll_module = module;

    ConsoleAttach();
    SetTextFGColor(3);
    printf("the\n");
    printf("m2o-reborn\n");
    SetTextFGColor(7);
    printf("starting...\n");

    char szRunPath[MAX_PATH] = { '\0' };
    GetModuleFileName(module, szRunPath, MAX_PATH);
    mod_dir = std::string(szRunPath);
    size_t pos = mod_dir.rfind("\\");
    mod_dir.erase(pos, std::string::npos);

    mod_files_dir = std::string(mod_dir + "\\files");
    _debug_stream.open(mod_dir + "\\m2o_debug.log");

    printf("attaching librg\n");

    // setup manual client mode
    librg::core_initialize(librg::mode_client_manual);

    // setup callbacks
    librg::events::add(librg::events::on_log, mod_onlog);
    librg::events::add(librg::events::on_tick, ontick);
    librg::events::add(librg::events::on_inter, entity_inter);
    librg::events::add(librg::events::on_create, entity_create);
    librg::events::add(librg::events::on_update, entity_update);
    librg::events::add(librg::events::on_remove, entity_remove);
    librg::events::add(librg::events::on_connect, client_connect);
    librg::events::add(librg::events::on_disconnect, client_disconnect);
    librg::events::add(librg::events::on_client_stream_entity, clientstream_update);

    auto cfg = librg::config_t{};
    cfg.ip = "localhost";
    cfg.port = 27010;
    cfg.world_size = HMM_Vec3(5000.00, 5000.00, 5000.00);
    cfg.tick_delay = 64;
    cfg.max_connections = 8;
    cfg.platform_id = 1;
    cfg.proto_version = 1;
    cfg.build_version = 1;

    // start the client (network connection)
    librg::core::start(cfg);

    if (global_gfx.Init() == false) {
        corelog("Unable to init Graphics Manager");
        game_exit("Unable to init Graphics Manager");
    }

    game_init();

    if (ExceptionHandler::Install() == false)
        game_exit("Unable to install exception handler");

    // if (m_clientSettings.LoadFile(CClientSettings::DEFAULT_SETTINGS_FILENAME) == false) {
    //     game_exit("Unable to parse config file");
    // }

    rawinput::on_mousemove = [&](RAWMOUSE me) {
        global_mouse_state.x += me.lLastX;
        global_mouse_state.y += me.lLastY;
        global_mouse_state.flags = me.usButtonFlags;
    };

    CDirectInput8Hook::Install();
    rawinput::hook();

    global_state.AddState(States::Menu, new CTitleState);
    // global_state.AddState(States::MPGame, new CGameState);
    global_state.ActivateState(States::Menu);
}

void game_exit(std::string reason)
{
    corelog("exiting %s", reason.c_str());
    librg::core_terminate();
    MessageBoxA(nullptr, reason.c_str(), "Well.. Something went wrong!", MB_OK);
    _debug_stream.close();
    exit(0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        mod_on_attach(hModule);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        // game_exit("deatch");
        break;
    }
    return TRUE;
}

