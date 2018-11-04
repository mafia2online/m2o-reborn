
#include <Hooking.h>
#include <future>

//#include <VFS/C_VirtualFileSystemCache_Wrapper.h>
//#include <Vfs/C_VirtualFileSystem_Wrapper.h>


static nomad::base_function init([]()
{
#if 1
    // Do not pause game in background
    nio::put_ljump(0xAC6D2B, 0xAC6F79);
    nio::put_ljump(0xAC6E57, 0xAC6F79);

    // Remove nvidia & 2k init logos
    nio::write<uint32_t>(0x08CA820, 0x90C300B0);
#endif

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
