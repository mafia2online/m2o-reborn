
#include <Gui/Cef/CefFrame.h>
#include <Gui/Cef/CefRenderImpl.h>

namespace nmd::gui
{

    CefRenderImpl::CefRenderImpl(CefFrame & frame) : frame(frame)
    {
    }

    void CefRenderImpl::Draw()
    {
        // todo: actually draw anything
    }

    bool CefRenderImpl::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect & rect)
    {
        auto &info = frame.GetFrameInfo();

        rect = CefRect(0, 0, info.sizex, info.sizey);

        return true;
    }

    void CefRenderImpl::OnPaint(CefRefPtr<CefBrowser> browse1r, PaintElementType type, const RectList & dirtyRects, const void * buffer, int width, int height)
    {
       
    }
}

