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

static C_TickedModuleManager * GetHook()
{
    mplus::GMPlusModule = new mplus::MPlusModule;

    return C_TickedModuleManager::GetInstance();
}

static nomad::base_function init([]()
{
    char* loc = nio::pattern("E8 ? ? ? ? 83 C4 04 84 C0 75 06").first();
    nio::replace_call(&Initialize_Game__Orig, loc, Initialize_Game);

    //loc = nio::pattern("")
    nio::put_call(0x00450347, GetHook);
});
