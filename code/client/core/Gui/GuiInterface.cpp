
#include <Gui\GuiInterface.h>

namespace nmd::gui
{
    void GuiInterface::Init()
    {
        cef_mgr = std::make_unique<CefManager>();

        if (cef_mgr->Initialize())
        {
            printf("Initialized cef!\n");
        }


    }

    void GuiInterface::Shutdown()
    {
        cef_mgr->Shutdown();

        //
        cef_mgr.release();
    }
}
