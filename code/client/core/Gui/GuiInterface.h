
#include <memory>
#include <Gui/Cef/CefManager.h>

namespace nmd::gui
{
    class GuiInterface
    {
        std::unique_ptr<CefManager> cef_mgr;

    public:

        void Init();
        void Shutdown();
    };
}
