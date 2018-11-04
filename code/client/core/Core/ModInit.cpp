#include <Hooking.h>

#include <MPlus.h>

static bool (*Initialize_Game__Orig)(void*);

static void InitializeM2Plus()
{
    printf("[Core] : InitializeM2Plus!\n");
}

static bool Initialize_Game(void* self)
{
    printf("[Core] : CInit!\n");

    return Initialize_Game__Orig(self);
}

static void GetHook()
{
    mplus::GMPlusModule = new mplus::MPlusModule;

    auto tickedModuleMgr = C_TickedModuleManager::GetInstance();
    tickedModuleMgr->RegisterStaticPlugins();
}

static nomad::base_function init([]()
{
    char* loc = nio::pattern("E8 ? ? ? ? 83 C4 04 84 C0 75 06").first();
    nio::replace_call(&Initialize_Game__Orig, loc, Initialize_Game);

    //loc = nio::pattern("")
    nio::nop(0x45034C, 9);
    nio::put_call(0x00450347, GetHook);
});
