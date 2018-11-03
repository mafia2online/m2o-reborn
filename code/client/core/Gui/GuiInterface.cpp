
#include <Gui\GuiInterface.h>

namespace nmd::gui
{
    GuiInterface::GuiInterface()
    {
        cef_mgr = std::make_unique<CefManager>();
    }

    GuiInterface::~GuiInterface()
    {
        
    }

    void GuiInterface::Init()
    {
        if (cef_mgr->Initialize())
        {
            printf("Initialized cef!\n");
        }
    }

    void GuiInterface::Shutdown()
    {
        cef_mgr->Shutdown();
    }

    void GuiInterface::Render()
    {
        cef_mgr->DoFrames();
    }
}
