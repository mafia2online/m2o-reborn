#include <Hooking.h>

#include <MPlus.h>

static bool (*Initialize_Game__Orig)(void*);

static void InitializeM2Plus()
{
    printf("[Core] : InitializeM2Plus!\n");

    mplus::GMPlusModule = new mplus::MPlusModule;

    mplus::MPlusModule::Intialize(mplus::GMPlusModule);
}

static bool Initialize_Game(void* self)
{
    bool state = Initialize_Game__Orig(self);

    if (state)
    {
        InitializeM2Plus();
    }

    return state;
}

static nomad::base_function init([]()
{
    char* loc = nio::pattern("E8 ? ? ? ? 83 C4 04 84 C0 75 06").first();
    nio::replace_call(&Initialize_Game__Orig, loc, Initialize_Game);
});
