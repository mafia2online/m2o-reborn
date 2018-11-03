#pragma once

#include <mutex>
#include <queue>

#include <d3d9.h>
#include <d3dx9.h>

#include <include/cef_browser.h>
#include <include/cef_client.h>

namespace nmd::gui
{
    class CefFrame;

    class CefRenderImpl : public CefRenderHandler
    {
        CefFrame &frame;

        std::mutex texture_lock;
        std::queue<CefRect> dirty_rects;

        bool buf_dirty = false;
        uint8_t* tex_buf = nullptr;

        LPDIRECT3DTEXTURE9 texture;
        LPDIRECT3DDEVICE9 device;
        LPD3DXSPRITE sprite;
        LPD3DXFONT font;
        D3DXVECTOR3 pos;

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
