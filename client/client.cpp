#define NOMINMAX // std::numeric_limits min&max

#include <stdio.h>
#include <stdint.h>
#include <windows.h>

#if !defined(Address)
#define Address unsigned long
#define Byte    unsigned char
#define Pointer unsigned int
#endif

// bla bla
#include <memory>
#include <limits>

// steam stuff
#include <iostream>
#include <fstream>

#include <algorithm>

// container stuff
#include <string>
#include <vector>
#include <list>

#include <librg/librg.h>
#include <m2sdk.h>

// shared stuff
#include <shared_defines.h>
#include <messages.h>

// public interface definitions
void game_init();
void game_exit();
void game_on_init();
void game_on_tick();
bool game_on_wnd_proc();

HMODULE dll_module;
std::ofstream _debug_stream;

// super global var for our player
librg::entity_t local_player;
M2::C_Player2 *dwLocalPlayer = nullptr;
M2::C_Human2 *ent = nullptr;
float ztime = 0;

#define corelog librg::core::log

// tool stuff
#include "tools/patcher.h"
#include "tools/steam_drm.h"
#include "tools/game_hooks.h"
#include "tools/file_patcher.h"

// actual client stuff
#include "callbacks/tick.h"
#include "callbacks/other.h"
#include "callbacks/entity_create.h"
#include "callbacks/entity_update.h"
#include "callbacks/entity_interpolate.h"
#include "callbacks/entity_remove.h"
#include "callbacks/clientstream_update.h"
#include "game.h"

void mod_on_attach(HMODULE module)
{
    dll_module = module;

    char run_path[MAX_PATH] = { '\0' };
    GetModuleFileName(module, run_path, MAX_PATH);

    auto startup_dir = std::string(run_path);
    size_t pos = startup_dir.rfind("\\");
    startup_dir.erase(pos, std::string::npos);

    auto files_dir = std::string(startup_dir + "\\files");

    // if (m_graphicsmanager.Init() == false) {
    //     ExitGame("Unable to init Graphics Manager");
    // }

    game_init();

    // if (ExceptionHandler::Install() == false)
    //     ExitGame("Unable to install exception handler");

    // if (m_clientSettings.LoadFile(CClientSettings::DEFAULT_SETTINGS_FILENAME) == false) {
    //     ExitGame("Unable to parse config file");
    // }

    // if (CNetworkManager::Instance().Init() == false) {
    //     ExitGame("Unable to init network manager");
    // }

    CDirectInput8Hook::Install();

    // m_statemanager.AddState(States::Menu, new CTitleState);
    // m_statemanager.AddState(States::MPGame, new CGameState);
    // m_statemanager.ActivateState(States::Menu);
}

void game_exit(std::string strreason)
{
    librg::core_terminate();
    MessageBoxA(nullptr, strreason.c_str(), "Well.. Something went wrong!", MB_OK);
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

