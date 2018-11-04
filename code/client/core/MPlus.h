
#include <m2/modules/C_TickedModule.h>
#include <m2/modules/C_TickedModuleManager.h>

#include <Gui/GuiInterface.h>

namespace mplus
{
    class MPlusModule : public C_TickedModule
    {
        nmd::gui::GuiInterface gui;

    public:

        MPlusModule();
        ~MPlusModule();

        // == overrides == //
        int32_t GetTypeID() override
        {
            return 7;
        }

        const char * GetDebugName() override
        {
            return "MPlusModule";
        }

        // no imp right now
        void RegisterCustoms() override
        {
            
        }

        // == intialize == //

        static void Intialize(MPlusModule* inst);


        // == callbacks == //

        void OnAppInit(I_TickedModuleCallEventContext &);
        void OnAppShutdown(I_TickedModuleCallEventContext &);

        void OnGameInit(I_TickedModuleCallEventContext &);
        void OnLoadStart(I_TickedModuleCallEventContext &);
        void OnLoadFinish(I_TickedModuleCallEventContext &);
        void OnTick(I_TickedModuleCallEventContext &);
        void OnRender(I_TickedModuleCallEventContext &);
    };

    extern MPlusModule *GMPlusModule;
}
