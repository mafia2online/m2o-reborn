#pragma once

#include <include/cef_app.h>

namespace nmd::gui
{
    class CefManager;

    class CefAppImpl : 
        public CefApp, 
        public CefRenderProcessHandler,
        public CefV8Handler

    {
        CefManager &mgr;

    public:

        CefAppImpl(CefManager &);

        // app
        void OnBeforeCommandLineProcessing(const CefString&, CefRefPtr<CefCommandLine>) override;

        // v8
        bool Execute(const CefString&, CefRefPtr<CefV8Value>, const CefV8ValueList&, CefRefPtr<CefV8Value>&, CefString&) override;

        // renderhandler
        bool OnProcessMessageReceived(CefRefPtr<CefBrowser>, CefProcessId, CefRefPtr<CefProcessMessage>) override;
     //   void OnContextCreated(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefV8Context>) override;
     //   void OnContextReleased(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefV8Context>) override;

        CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; };

        IMPLEMENT_REFCOUNTING(CefAppImpl);
    };
}