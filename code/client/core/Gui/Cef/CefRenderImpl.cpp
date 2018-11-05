
#include <Gui/Cef/CefFrame.h>
#include <Gui/Cef/CefRenderImpl.h>

#include <Hooking.h>

static IDirect3DDevice9* Gdevice;

namespace nmd::gui
{
    CefRenderImpl::CefRenderImpl(CefFrame& frame) : frame(frame), sprite(nullptr)
    {
        device = Gdevice;

        auto& info = frame.GetFrameInfo();

        device->CreateTexture(info.sizex, info.sizey, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture,
                              nullptr);

        tex_buf = new uint8_t[info.sizex * info.sizey * 4];
        memset(tex_buf, 0, info.sizex * info.sizey * 4);
    }

    void CefRenderImpl::Draw()
    {
        if (!texture) return;

        texture_lock.lock();

        if (!dirty_rects.empty())
        {
            D3DLOCKED_RECT locked_rect;
            auto s_Result = texture->LockRect(0, &locked_rect, nullptr, D3DLOCK_DISCARD);

            void *pBits = locked_rect.pBits;
            int pitch = locked_rect.Pitch;

            if (SUCCEEDED(s_Result))
            {
                if (!tex_buf)
                {
                    texture->UnlockRect(0);
                    texture_lock.unlock();
                    return;
                }

                while (!dirty_rects.empty())
                {
                    CefRect rect = dirty_rects.front();
                    dirty_rects.pop();

                    auto &info = frame.GetFrameInfo();

                    for (int y = rect.y; y < (rect.y + rect.height); y++)
                    {
                        int* src = &((int*)(tex_buf))[(y * info.sizex) + rect.x];
                        int* dest = &((int*)(pBits))[(y * (pitch / 4)) + rect.x];

                        memcpy(dest, src, (rect.width * 4));
                    }
                }

                texture->UnlockRect(0);
            }
        }

        texture_lock.unlock();

        sprite->Begin(D3DXSPRITE_ALPHABLEND);
        sprite->Draw(texture, nullptr, nullptr, &pos, 0xFFFFFFFF);

        sprite->Flush();
        sprite->End();
    }

    bool CefRenderImpl::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
    {
        auto& info = frame.GetFrameInfo();

        rect = CefRect(0, 0, info.sizex, info.sizey);


        return true;
    }

    void CefRenderImpl::OnPaint(CefRefPtr<CefBrowser> browse1r, PaintElementType type, const RectList& dirtyRects,
                                const void* buffer, int width, int height)
    {
        if (type == PET_VIEW)
        {
#ifdef FRAME_DEBUG
            printf("Drawing PET_VIEW\n");
#endif
            const int32_t* pBuffer = (int32_t*)buffer;
            int32_t* pTarget = (int32_t*)tex_buf;

            std::lock_guard<std::mutex> lock(texture_lock);
            for (const auto& rect : dirtyRects)
            {
                for (int y = rect.y; y < (rect.y + rect.height); y++)
                {
                    auto src = &pBuffer[(y * width) + rect.x];
                    auto dst = &pTarget[(y * width) + rect.x];

                    memcpy(dst, src, rect.width * 4);
                }

                dirty_rects.push(rect);
            }
        }
        else if (type == PET_POPUP)
        {
            printf("Drawing PET_POPUP\n");
        }

        buf_dirty = true;
    }
}
