#pragma once

#include <Gui/Cef/CefRenderImpl.h>

namespace nmd::gui
{
    struct CefFrameInfo
    {
        uint32_t sizex, sizey;
    };

    class CefFrame : public

        CefClient,
        CefLifeSpanHandler,
        CefDisplayHandler

    {
        CefRefPtr<CefBrowser> browser;

        // current renderhandler backend ptr
        CefRefPtr<CefRenderHandler> renderhandler;

        CefFrameInfo frame_info;

    public:

        CefFrame(const std::string& url, const CefFrameInfo &info);
        ~CefFrame();

        inline const CefFrameInfo &GetFrameInfo() const
        {
            return frame_info;
        }

        // cef : client
        CefRefPtr<CefRenderHandler> GetRenderHandler() override
        {
            return renderhandler;
        }

        // cef : lifespanhandler
        CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override
        {
            return this;
        }

        // cef : displayhandler
        CefRefPtr<CefDisplayHandler> GetDisplayHandler() override
        {
            return this;
        }

        void OnAfterCreated(CefRefPtr<CefBrowser> cef_browser) override { browser = cef_browser; };
        void OnBeforeClose(CefRefPtr<CefBrowser> cef_browser) override { browser = nullptr; };

        bool OnConsoleMessage(CefRefPtr<CefBrowser> browser, cef_log_severity_t level, const CefString& message, const CefString& source, int line) override;
        //IMPLEMENT_REFCOUNTING(UiWindow);

        // hack :
        void AddRef() const override { };
        bool Release() const override { return false; };
        bool HasOneRef() const override { return false; }
    };

}
