#include <m2sdk.h>

namespace M2
{
    class GameModule : public C_TickedModule
    {
    public:
        void Tick(I_TickedModuleCallEventContext &)
        {
        }
    };
};

static M2::GameModule module;

void gamemodule_install()
{
    M2::C_TickedModuleManager2::Get()->AddAction(M2::TickedModuleEvent::EVENT_TICK, 500, &module, static_cast<void (M2::C_TickedModule::*)(M2::I_TickedModuleCallEventContext &)>(&M2::GameModule::Tick), -1.0f, 0, 0);
}
