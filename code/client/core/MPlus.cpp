#include <MPlus.h>

namespace mplus
{
    MPlusModule* GMPlusModule;

    MPlusModule::MPlusModule()
    {
        Intialize(this);
    }

    MPlusModule::~MPlusModule()
    {
        printf(__FUNCTION__ "\n");
    }

    void MPlusModule::Intialize(MPlusModule* inst)
    {
        printf("[Core] " __FUNCTION__ "\n");

        auto mgr = C_TickedModuleManager::GetInstance();

        mgr->AddAction(C_TickedModuleManager::E_TickedModuleEvent::SYSTEM_INIT, 500, inst,
                       (C_TickedModuleManager::TickedModuleCallback)(&OnSysInit), -1.0f, 0, 0);
        mgr->AddAction(C_TickedModuleManager::E_TickedModuleEvent::SYSTEM_DONE, 500, inst,
                       (C_TickedModuleManager::TickedModuleCallback)(&OnSysShutdown), -1.0f, 0, 0);
        mgr->AddAction(C_TickedModuleManager::E_TickedModuleEvent::GAME_INIT, 500, inst,
                       (C_TickedModuleManager::TickedModuleCallback)(&OnGameInit), -1.0f, 0, 0);
        mgr->AddAction(C_TickedModuleManager::E_TickedModuleEvent::LOADING_STARTED, 1720, inst,
                       (C_TickedModuleManager::TickedModuleCallback)(&OnLoadStart), -1.0f, 0, 0);
        mgr->AddAction(C_TickedModuleManager::E_TickedModuleEvent::LOADING_FINISHED, 1720, inst,
                       (C_TickedModuleManager::TickedModuleCallback)(&OnLoadFinish), -1.0f, 0, 0);
        mgr->AddAction(C_TickedModuleManager::E_TickedModuleEvent::TICK, 400, inst,
                       (C_TickedModuleManager::TickedModuleCallback)(&OnTick), -1.0f, 0, 0);
        mgr->AddAction(C_TickedModuleManager::E_TickedModuleEvent::RENDER, 100, inst,
                       (C_TickedModuleManager::TickedModuleCallback)(&OnRender), -1.0f, 0, 0);
    }

    void MPlusModule::OnSysInit(I_TickedModuleCallEventContext&)
    {
        gui.Init();
        net.Init();
    }

    void MPlusModule::OnSysShutdown(I_TickedModuleCallEventContext&)
    {
        printf(__FUNCTION__ "\n");

        // lol
        delete this;
    }

    void MPlusModule::OnGameInit(I_TickedModuleCallEventContext&)
    {
    }

    void MPlusModule::OnLoadStart(I_TickedModuleCallEventContext&)
    {
    }

    void MPlusModule::OnLoadFinish(I_TickedModuleCallEventContext&)
    {
    }

    void MPlusModule::OnTick(I_TickedModuleCallEventContext&)
    {
        if (net.IsActive()) net.Process();

        // debug
        if (GetAsyncKeyState(VK_F5) & 0x1)
        {
            if (!net.IsConnected())
            {
                net.Connect("localhost", 27010);
                printf("[Core] Started Network!\n");
            }
            else
            {
                net.Disconnect();
                printf("[Core] Stopped Network!\n");
            }
        }
    }

    void MPlusModule::OnRender(I_TickedModuleCallEventContext&)
    {
        gui.Render();
    }
}
