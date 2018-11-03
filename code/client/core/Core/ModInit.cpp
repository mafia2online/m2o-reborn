#include <Hooking.h>

#include <MPlus.h>

static bool (*Initialize_Game__Orig)(void*);

static bool Initialize_Game(void* self)
{
    printf("[Core] : CInit!\n");

    return Initialize_Game__Orig(self);
}

static void InitializeModuleManager()
{
    mplus::GMPlusModule = new mplus::MPlusModule;

    auto tickedModuleMgr = C_TickedModuleManager::GetInstance();
    tickedModuleMgr->RegisterStaticPlugins();
}

static nomad::base_function init([]()
{
    char* loc = nio::pattern("E8 ? ? ? ? 83 C4 04 84 C0 75 06").first();
    nio::replace_call(&Initialize_Game__Orig, loc, Initialize_Game);

    loc = nio::pattern("8B 10 8B C8 8B 42 2C FF D0 8B 0D").first();
    nio::nop(loc, 9);
    nio::put_call(loc - 5, InitializeModuleManager);
});
