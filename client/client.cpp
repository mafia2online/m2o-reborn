#define NOMINMAX // std::numeric_limits min&max

#include <stdio.h>
#include <stdint.h>

// bla bla
#include <memory>
#include <limits>
#include <algorithm>

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

// public interface definitions
void game_init();
void game_exit(std::string reason);
void game_on_init();
void game_on_tick();
bool game_on_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

std::string mod_dir;

#define corelog librg::core::log

// dx stuff
#define DIRECTINPUT_VERSION 0x0800
#include <detours.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

// gwen
#include <Gwen/Gwen.h>
#include <Gwen/Skins/Simple.h>
#include <Gwen/Skins/TexturedBase.h>
#include <Gwen/Controls/Base.h>
#include <Gwen/Controls/ListBox.h>
#include <Gwen/Controls/TextBox.h>
#include <Gwen/Controls/WindowControl.h>
#include <Gwen/Input/Windows.h>
#include <Gwen/Renderers/DirectX9.h>


// tool stuff
#include "tools/console.h"
#include "tools/patcher.h"
#include "tools/steam_drm.h"
#include "tools/game_hooks.h"
#include "tools/file_patcher.h"

#include <m2sdk.h>

// todo: refactor
M2::C_Player2 *dwLocalPlayer = nullptr;
M2::C_Human2 *ent = nullptr;

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
#include "gfx/dx/CDirect3DDevice9Proxy.h"
#include "gfx/dx/CDirect3D9Proxy.h"
#include "gfx/dx/CDirect3D9Hook.h"
#include "gfx/dx/CDirectInputDevice8Proxy.h"
#include "gfx/dx/CDirectInput8Proxy.h"
#include "gfx/dx/CDirectInput8Hook.h"
#include "gfx/CMPStateManager.h"
#include "gfx/CDebugConsole.h"
#include "gfx/CFontManager.h"
#include "gfx/GwenManager.h"
#include "gfx/CGraphicsManager.h"
#include "game.h"

HMODULE dll_module;
std::ofstream _debug_stream;

// super global var for our player
librg::entity_t local_player;
float ztime = 0;

CGraphicsManager m_graphicsmanager;

void mod_on_attach(HMODULE module)
{
    dll_module = module;

    ConsoleAttach();
    SetTextFGColor(3);
    printf("the\n");
    printf("m2o-reborn\n");
    SetTextFGColor(7);
    printf("starting...\n");

    auto files_dir = std::string(mod_dir + "\\files");

    if (m_graphicsmanager.Init() == false) {
        game_exit("Unable to init Graphics Manager");
    }

    game_init();

    // if (ExceptionHandler::Install() == false)
    //     game_exit("Unable to install exception handler");

    // if (m_clientSettings.LoadFile(CClientSettings::DEFAULT_SETTINGS_FILENAME) == false) {
    //     game_exit("Unable to parse config file");
    // }

    // if (CNetworkManager::Instance().Init() == false) {
    //     game_exit("Unable to init network manager");
    // }

    CDirectInput8Hook::Install();

    // m_statemanager.AddState(States::Menu, new CTitleState);
    // m_statemanager.AddState(States::MPGame, new CGameState);
    // m_statemanager.ActivateState(States::Menu);
}

void game_exit(std::string reason)
{
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
        game_exit("deatch");
        break;
    }
    return TRUE;
}
