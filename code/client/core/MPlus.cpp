#include <MPlus.h>

namespace mplus
{
    MPlusModule* GMPlusModule;

    MPlusModule::MPlusModule()
    {
        Intialize(this);

        gui.Init();
    }

    MPlusModule::~MPlusModule()
    {
        printf(__FUNCTION__ "\n");
        gui.Shutdown();
    }

    void MPlusModule::Intialize(MPlusModule* inst)
    {
        printf(__FUNCTION__ "\n");

        auto mgr = C_TickedModuleManager::GetInstance();

        mgr->AddAction(C_TickedModuleManager::E_TickedModuleEvent::APP_ACTIVATE, 500, inst,
                       (C_TickedModuleManager::TickedModuleCallback)(&OnAppInit), -1.0f, 0, 0);
        mgr->AddAction(C_TickedModuleManager::E_TickedModuleEvent::APP_DEACTIVATE, 500, inst,
                       (C_TickedModuleManager::TickedModuleCallback)(&OnAppInit), -1.0f, 0, 0);
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

    void MPlusModule::OnAppInit(I_TickedModuleCallEventContext&)
    {
    }

    void MPlusModule::OnAppShutdown(I_TickedModuleCallEventContext&)
    {
        auto mgr = C_TickedModuleManager::GetInstance();

        //mgr->DelAction()

        //delete this;
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
    }

    void MPlusModule::OnRender(I_TickedModuleCallEventContext&)
    {
        printf(__FUNCTION__"\n");
        printf("wat!\n");
    }
}
