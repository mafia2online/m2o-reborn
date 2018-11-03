#pragma once

#include <include/cef_browser.h>
#include <include/cef_client.h>

namespace nmd::gui
{
    class CefFrame;

    class CefRenderImpl : public CefRenderHandler
    {
        CefFrame &frame;

    public:

        CefRenderImpl(CefFrame &frame);

        // actual render
        void Draw();

        // cef overrides
        bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
        void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects,
            const void* buffer, int width, int height) override;

        IMPLEMENT_REFCOUNTING(CefRenderImpl);
    };
}