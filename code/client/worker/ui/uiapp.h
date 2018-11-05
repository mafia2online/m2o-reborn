#pragma once

/*
*  This file is part of the NomadFX Project - https://nomad-group.net
*  More information regarding licensing can be found in LICENSE.md
*/

#include <vector>

#include <include/cef_app.h>
#include <include/cef_browser.h>
#include <include/cef_client.h>

namespace nfx
{
    class UiApp : public
         CefApp,
         CefRenderProcessHandler,
         CefV8Handler
    {
        using V8EventHandler = std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value>>;

        std::vector<V8EventHandler> eventhandlers;

    public:

        virtual ~UiApp() = default;

        void CallEvent(const CefV8ValueList &args);

        // cef: v8 handler
        bool Execute(const CefString&, CefRefPtr<CefV8Value>, const CefV8ValueList&, CefRefPtr<CefV8Value>&, CefString&) override;

        // cef: cefapp
        void OnBeforeCommandLineProcessing(const CefString&, CefRefPtr<CefCommandLine>) override;

        // cef: renderprocesshandler
        bool OnProcessMessageReceived(CefRefPtr<CefBrowser>, CefProcessId, CefRefPtr<CefProcessMessage>) override;
        void OnContextCreated(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefV8Context>) override;
        void OnContextReleased(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefV8Context>) override;

        CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; }

        IMPLEMENT_REFCOUNTING(UiApp);
    };
}