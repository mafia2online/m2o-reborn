
#include <Gui/Cef/CefFrame.h>

//#include <Console\DebugConsole.h>

namespace nmd::gui
{
    CefFrame::CefFrame(const std::string & url, const CefFrameInfo &frame_info) : frame_info(frame_info)
    {
        CefWindowInfo info;
        info.SetAsWindowless(nullptr);

        // Browser Settings
        CefBrowserSettings settings;
        settings.javascript_close_windows = STATE_DISABLED;
        settings.javascript_access_clipboard = STATE_DISABLED;
        settings.plugins = STATE_DISABLED;
        settings.windowless_frame_rate = 60;
        settings.webgl = STATE_ENABLED;
        settings.file_access_from_file_urls = STATE_ENABLED;

        // RenderHandler
        renderhandler = new CefRenderImpl(*this);

        auto ctx = CefRequestContext::GetGlobalContext();

        CefBrowserHost::CreateBrowser(info, this, url, settings, ctx);
    }

    CefFrame::~CefFrame()
    {
        browser->GetHost()->CloseBrowser(true);
    }

    bool CefFrame::OnConsoleMessage(CefRefPtr<CefBrowser> browser, cef_log_severity_t level, const CefString & message, const CefString & source, int line)
    {
      //  ConHost::trace(DebugMessageType::none, L"cef : %S:%d,%s\n", source.ToWString().c_str(), line, message.ToWString().c_str());

        return false;
    }
}
