#define NOMINMAX // std::numeric_limits min&max
#include <string>
#include <limits>
#include <algorithm>
#include <memory>
#include <list>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <windows.h>

#include "client.h"
#include "disablescripts.hpp"
#include "gtav_world.hpp"

std::ofstream debug;
librg::entity_t local_player;

// End program execution
void die(const char *why) {
    printf("%s", why);
    char r = getc(stdin);
    exit(1);
}

void spawn_player(librg::entity_t entity)
{
    entity.assign<librg::streamable_t>();
    entity.assign<gamedata_t>(PLAYER::PLAYER_PED_ID());
    entity.assign<ped_t>();

    librg::core::log("spawned player!");
}

void unspawn_player(librg::entity_t entity)
{
    entity.remove<ped_t>();
    entity.remove<gamedata_t>();
    entity.remove<librg::streamable_t>();

    librg::core::log("unspawned player!");
}

void client_connect(librg::events::event_t* evt)
{
    auto event = (librg::events::event_entity_t*)evt;
    auto entity = event->entity;

    local_player = entity;

    librg::core::log("connected to the server");
    spawn_player(entity);
}

void client_disconnect(librg::events::event_t* evt)
{
    auto event = (librg::events::event_entity_t*)evt;
    auto entity = event->entity;

    librg::core::log("disconnected form the server");
    unspawn_player(entity);
}


void init()
{
    // setup manual client mode
    librg::core_initialize(librg::mode_client_manual);

    librg::events::add(librg::events::on_log, [](librg::events::event_t* evt) {
        auto event = (librg::events::event_log_t*) evt;
        printf("%s", event->output);
        debug << event->output;
    });

    // setup callbacks
    librg::events::add(librg::events::on_tick, ontick);
    librg::events::add(librg::events::on_inter, entity_inter);
    librg::events::add(librg::events::on_create, entity_create);
    librg::events::add(librg::events::on_update, entity_update);
    librg::events::add(librg::events::on_remove, entity_remove);
    librg::events::add(librg::events::on_connect, client_connect);
    librg::events::add(librg::events::on_disconnect, client_disconnect);
    librg::events::add(librg::events::on_client_stream_entity, clientstream_update);

    auto cfg = librg::config_t{};
    cfg.ip = "inlife.no-ip.org";
    cfg.port = 27010;
    cfg.world_size = HMM_Vec3(5000.00, 5000.00, 5000.00);
    cfg.tick_delay = 64;
    cfg.max_connections = 8;
    cfg.platform_id = 1;
    cfg.proto_version = 1;
    cfg.build_version = 1;

    // start the client (network connection)
    librg::core::start(cfg);
    librg::network::start();

}

void update()
{
    librg::core::tick();

}

// Main program
void main()
{
    debug.open("m2o_debug.log");
    //debug.clear();

    init();

    while (true) {
        update();
        WAIT(0);
    }


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

void ExitGame(SString strreason)
{
    librg::core_terminate();
    MessageBoxA(nullptr, strreason.GetCStr(), "Well.. Something went wrong!", MB_OK);
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

