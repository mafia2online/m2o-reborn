#include <m2sdk.h>

#include <future>

void call_respawn()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    mod_player_spawn();
}

namespace M2
{
    class GameModule : public C_TickedModule
    {
    public:
        void EventGameInit(I_TickedModuleCallEventContext &)
        {
            mod_log("[GameModule] : EventGameInit\n");
            mod_game_init();
        }

        void EventTick(I_TickedModuleCallEventContext&)
        {
            mod_game_tick();
        }
        void EventLoadingStarted(I_TickedModuleCallEventContext &)
        {
            mod_log("[GameModule] : EventLoadingStarted\n");
        }

        void EventLoadingFinished(I_TickedModuleCallEventContext &)
        {
            mod_log("[GameModule] : EventLoadingFinished\n");
            
            std::thread(call_respawn).detach();
        }
    };
};

static M2::GameModule module;

void gamemodule_install()
{
    M2::C_TickedModuleManager2::Get()->AddAction(M2::TickedModuleEvent::EVENT_GAME_INIT, 400, &module, static_cast<void (M2::C_TickedModule::*)(M2::I_TickedModuleCallEventContext &)>(&M2::GameModule::EventGameInit), -1.0f, 0, 0);
    M2::C_TickedModuleManager2::Get()->AddAction(M2::TickedModuleEvent::EVENT_TICK, 500, &module, static_cast<void (M2::C_TickedModule::*)(M2::I_TickedModuleCallEventContext &)>(&M2::GameModule::EventTick), -1.0f, 0, 0);
    M2::C_TickedModuleManager2::Get()->AddAction(M2::TickedModuleEvent::EVENT_LOADING_STARTED, 1720, &module, static_cast<void (M2::C_TickedModule::*)(M2::I_TickedModuleCallEventContext &)>(&M2::GameModule::EventLoadingStarted), -1.0f, 0, 0);
    M2::C_TickedModuleManager2::Get()->AddAction(M2::TickedModuleEvent::EVENT_LOADING_FINISHED, 1720, &module, static_cast<void (M2::C_TickedModule::*)(M2::I_TickedModuleCallEventContext &)>(&M2::GameModule::EventLoadingFinished), -1.0f, 0, 0);
}
