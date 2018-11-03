
#include <m2/modules/C_TickedModule.h>
#include <m2/modules/C_TickedModuleManager.h>

namespace mplus
{
    class MPlusModule : public C_TickedModule
    {
    public:

        MPlusModule();
        ~MPlusModule() = default;

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

        void OnGameInitialize(I_TickedModuleCallEventContext &);
        void OnLoadStart(I_TickedModuleCallEventContext &);
        void OnLoadFinish(I_TickedModuleCallEventContext &);
        void OnTick(I_TickedModuleCallEventContext &);
        void OnRender(I_TickedModuleCallEventContext &);
    };

    extern MPlusModule *GMPlusModule;
}
