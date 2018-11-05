
#include <Gui/Cef/CefAppImpl.h>

namespace nmd::gui
{
    CefAppImpl::CefAppImpl(CefManager & mgr) : mgr(mgr)
    {

    }

    void CefAppImpl::OnBeforeCommandLineProcessing(const CefString &, CefRefPtr<CefCommandLine> cmdl)
    {
        cmdl->AppendSwitch("enable-experimental-web-platform-features");
        cmdl->AppendSwitch("transparent-painting-enabled");
        cmdl->AppendSwitch("off-screen-rendering-enabled");
        cmdl->AppendSwitch("disable-gpu-compositing");
        cmdl->AppendSwitch("enable-begin-frame-scheduling");
    }

    bool CefAppImpl::Execute(const CefString &, CefRefPtr<CefV8Value>, const CefV8ValueList &, CefRefPtr<CefV8Value>&, CefString &)
    {
        return false;
    }

    bool CefAppImpl::OnProcessMessageReceived(CefRefPtr<CefBrowser>, CefProcessId, CefRefPtr<CefProcessMessage>)
    {
        return false;
    }

/*
    void CefAppImpl::OnContextCreated(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefV8Context>)
    {
    }

    void CefAppImpl::OnContextReleased(CefRefPtr<CefBrowser>, CefRefPtr<CefFrame>, CefRefPtr<CefV8Context>)
    {
    }
    */
}
