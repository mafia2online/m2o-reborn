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

// shared stuff
#include <shared_defines.h>
#include <messages.h>

void game_init();
void game_tick();

// tool stuff
#include <tools/patcher.h>
#include <tools/steam_drm.h>
#include <tools/game_hooks.h>

// actual client stuff
#include "init.h"
#include "callbacks/tick.h"
#include "callbacks/other.h"
#include "callbacks/entity_create.h"
#include "callbacks/entity_update.h"
#include "callbacks/entity_interpolate.h"
#include "callbacks/entity_remove.h"
#include "callbacks/clientstream_update.h"


// no
std::ofstream debug;

// super global var for our player
librg::entity_t local_player;

void game_init() {
    debug.open("m2o_debug.log");
    init_librg();
}

void game_tick() {
    librg::core::tick();
}

void OnAttach(HMODULE module)
{
    // plox maybe integrate in some util class latr kthx
    char szRunPath[MAX_PATH] = { '\0' };
    GetModuleFileName(module, szRunPath, MAX_PATH);
    m_strappdir.Set(szRunPath);
    size_t pos = m_strappdir.GetSTLString().rfind("\\");
    m_strappdir.GetSTLString().erase(pos, std::string::npos);

    m_strfilesdir = m_strappdir;
    m_strfilesdir += "\\files";


    CDirectInput8Hook::Install();
}

void ExitGame(std::string strreason)
{
    librg::core_terminate();
    MessageBoxA(nullptr, strreason.c_str(), "Well.. Something went wrong!", MB_OK);
    debug.close();
    exit(0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        OnAttach(hModule);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        ExitGame("deatch");
        break;
    }
    return TRUE;
}

