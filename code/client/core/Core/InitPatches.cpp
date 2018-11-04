
#include <Hooking.h>
#include <future>

#include <ue/sys/filesystem.h>
#include <wrappers/C_VirtualFileSystemCache_Wrapper.h>

static void* origVFSInit;
bool CreateVFSHook(uint32_t a1) {
  uintptr_t result = nio::call<uintptr_t>(origVFSInit, a1);

  auto vfsSys = ue::sys::filesystem::C_VirtualFileSystemCache::GetInstance();

  static auto virtualFileSysWrapper = static_cast<ue::sys::filesystem::C_VirtualFileSystemCache_Wrapper*>(vfsSys);
  virtualFileSysWrapper->Cast();

  return true;
}

static nomad::base_function init([]()
{
    // Disable mutex
    nio::put_ljump(0x00401B89, 0x00401C16);

    // Do not pause game in background
    nio::put_ljump(0xAC6D2B, 0xAC6F79);
    nio::put_ljump(0xAC6E57, 0xAC6F79);

    // Remove nvidia & 2k init logos
    nio::write<uint32_t>(0x08CA820, 0x90C300B0);

    // Tmp development stuff
    //nio::replace_call(&origVFSInit, (void*)0x69A35C, AllocVFS);
    *(uint8_t*)0x69A358 = 0xFF; // allocate 255 instead of 224 bytes

    nio::replace_call(&origVFSInit, (void*)0x69A36A, CreateVFSHook);

#if 0
    
    //nio::nop(0x004F2B8D, 5);

    //nio::write<uint8_t>(0x008CA820, 0xCC);

    //nio::return_function(0x7B9DB0); // mafia::gui::C_AnimLoadingScreen::ActivateLoadingScreenInternal(
     
    //nio::write(0x007B6A10, 0xC3);
    //nio::nop(0x7B687E, 5);
    //nio::nop(0x7B6021, 5);

    //nio::write<uint32_t>(0x7B6021 + 0x1, 0x7B6900);

    // Startup screen, let us decide when we are ready
    //nio::nop(0x004B2EDF, 22);
    //nio::nop(0x0044D488, 6);

    // StartRenderPresentationMode 0x008DD430
    // ActivateLoadingScreenInternal 0x008CB530
    // StopRenderPresentationMode 0x008D60E0
    // GuiGameLoaderFinishLoad 0x008FFDF0

    //*(uint8_t*)0x18DE466 = 0x75;

    //nio::nop(0x013C396F, 5);

    //nio::return_function(0x18DE370);

    //nio::nop(0xA88681, 5);

    static void* origScreen;
    nio::replace_call(&origScreen, (void*)0x4B2EF8, (LPVOID)&[](uint32_t instance) -> void { 
        uint32_t realAddr = *(uint32_t *)instance;

        *(uint32_t*)(*(uint32_t*)(realAddr + 6012) + 4) = 3;
        
        *(uint8_t *)((uint32_t)realAddr + 5995) = 1;
        *(uint8_t *)((uint32_t)realAddr + 5994) = 1;

        *(uint32_t *)(*(uint32_t*)0x23841FC + 140) = 5;

        //if(*(uint8_t *)((uint32_t)realAddr + 5995) && *(uint8_t *)((uint32_t)realAddr + 5994))
            nio::call(origScreen, instance, 1);
    });
#endif
});
