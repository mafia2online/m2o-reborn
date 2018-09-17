#include "cef_app.h"
#include "cef_client.h"
#include "wrapper/cef_helpers.h"

class RenderHandler;
class BrowserClient;

struct cef_object {
    u8 type;

    u8 valid : 1;
    u8 queued : 1;
    u8 metadata : 6;

    i32 zindex;

    CefRefPtr<CefBrowser> browser;
    CefRefPtr<BrowserClient> client;
    CefRefPtr<RenderHandler> renderer;
};

struct {
    int object_cursor;
    zpl_array(cef_object) objects;
} cef_state;

// =======================================================================//
// !
// ! CEF interfaces
// !
// =======================================================================//

class RenderHandler : public CefRenderHandler {
    public:
        unsigned char* mPixelBuffer;
        int mPixelBufferWidth;
        int mPixelBufferHeight;
        unsigned char* mPopupBuffer;
        unsigned char* mPixelBufferRow;
        CefRect mPopupBufferRect;
        int mBufferDepth;
        bool mFlipYPixels;

        gfx_handle mTexture;

        RenderHandler(int w, int h, int zindex) {
            // inidcates if we should flip the pixel buffer in Y direction
            mFlipYPixels = false;

            // the pixel buffer
            mPixelBuffer = nullptr;
            mPixelBufferWidth = 0;
            mPixelBufferHeight = 0;

            // the popup buffer
            mPopupBuffer = nullptr;

            // a row in the pixel buffer - used as temp buffer when flipping
            mPixelBufferRow = nullptr;

            // depth is same for all buffer
            mBufferDepth = 4;
            mTexture = -1;

            resize(w, h, zindex);
            show(zindex);
        }

        ~RenderHandler() {
            delete[] mPixelBuffer;
            delete[] mPopupBuffer;
            delete[] mPixelBufferRow;

            gfx_destroy(mTexture);
        }

        void show(int zindex) {
            gfx_render_add(mTexture, zindex);
        }

        void hide() {
            gfx_render_remove(mTexture);
        }

        void changeIndex(int zindex) {
            gfx_zindex_set(mTexture, zindex);
        }

        void resize(int width, int height, int zindex) {
            if (mPixelBufferWidth != width || mPixelBufferHeight != height) {
                delete[] mPixelBuffer;
                mPixelBufferWidth = width;
                mPixelBufferHeight = height;
                mPixelBuffer = new unsigned char[mPixelBufferWidth * mPixelBufferHeight * mBufferDepth];
                memset(mPixelBuffer, 0xff, mPixelBufferWidth * mPixelBufferHeight * mBufferDepth);

                delete[] mPixelBufferRow;
                mPixelBufferRow = new unsigned char[mPixelBufferWidth * mBufferDepth];

                int was_visible = gfx_render_exists(mTexture);

                if (mTexture != -1) {
                    gfx_destroy(mTexture);
                }

                mTexture = gfx_create_texture(width, height);

                if (was_visible > 0) {
                    gfx_render_add(mTexture, zindex);
                }
            }
        }

        void copyPopupIntoView() {
            int popup_y = (mFlipYPixels ? (mPixelBufferHeight - mPopupBufferRect.y) : mPopupBufferRect.y);
            unsigned char* src = (unsigned char*)mPopupBuffer;
            unsigned char* dst = mPixelBuffer + popup_y * mPixelBufferWidth * mBufferDepth + mPopupBufferRect.x * mBufferDepth;
            while (src < (unsigned char*)mPopupBuffer + mPopupBufferRect.width * mPopupBufferRect.height * mBufferDepth) {
                memcpy(dst, src, mPopupBufferRect.width * mBufferDepth);
                src += mPopupBufferRect.width * mBufferDepth;
                dst += mPixelBufferWidth * mBufferDepth * (mFlipYPixels ? -1 : 1);
            }
        }

        bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override {
            rect = CefRect(0, 0, mPixelBufferWidth, mPixelBufferHeight);
            return true;
        }

        void OnCursorChange(CefRefPtr<CefBrowser> browser,
                          CefCursorHandle cursor,
                          CursorType type,
                          const CefCursorInfo& custom_cursor_info) override {
            mod_log("cursor has changed to %d", type);
            SetCursor(cursor);
        }

        void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) override {
            CEF_REQUIRE_UI_THREAD();

            // whole page was updated
            if (type == PET_VIEW) {
                // and copy them in
                memcpy(mPixelBuffer, buffer, width * height * mBufferDepth);

                // we need to flip pixel buffer in Y direction as per settings
                if (mFlipYPixels) {
                    const size_t stride = mPixelBufferWidth * mBufferDepth;
                    unsigned char* lower = mPixelBuffer;
                    unsigned char* upper = mPixelBuffer + (mPixelBufferHeight - 1) * stride;
                    while (lower < upper) {
                        memcpy(mPixelBufferRow, lower, stride);
                        memcpy(lower, upper, stride);
                        memcpy(upper, mPixelBufferRow, stride);
                        lower += stride;
                        upper -= stride;
                    }
                }

                // if there is still a popup open, write it into the page too (it's pixels will have been
                // copied into it's buffer by a call to OnPaint with type of PET_POPUP earlier)
                if (mPopupBuffer != nullptr) {
                    copyPopupIntoView();
                }
            }
            // popup was updated
            else if (type == PET_POPUP) {
                // copy over the popup pixels into it's buffer
                // (popup buffer created in onPopupSize() as we know the size there)
                memcpy(mPopupBuffer, buffer, width * height * mBufferDepth);

                // copy over popup pixels into page pixels. We need this for when popup is changing (e.g. highlighting or scrolling)
                // when the containing page is not changing and therefore doesn't get an OnPaint update
                copyPopupIntoView();
            }

            // if we have a buffer, indicate to consuming app that the page changed.
            if (mPixelBufferWidth > 0 && mPixelBufferHeight > 0) {
                gfx_update_texture(mTexture, mPixelBuffer, mPixelBufferWidth, mPixelBufferHeight);
            }
        }

        // CefRenderHandler override
        void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) override {
            CEF_REQUIRE_UI_THREAD();

            mod_log("[cef] Popup state set to %d", show);

            if (!show) {
                delete[] mPopupBuffer;
                mPopupBuffer = nullptr;

                mPopupBufferRect.Reset();
            }
        }

        // CefRenderHandler override
        void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) override {
            CEF_REQUIRE_UI_THREAD();

            mPopupBufferRect = rect;
            if (mPopupBuffer == nullptr) {
                mPopupBuffer = new unsigned char[rect.width * rect.height * mBufferDepth];
                memset(mPopupBuffer, 0xff, rect.width * rect.height * mBufferDepth);
            }
        }

        IMPLEMENT_REFCOUNTING(RenderHandler);
};

class BrowserClient : public CefClient, public CefLifeSpanHandler {
    public:
        BrowserClient(RenderHandler* render_handler) : render_handler_(render_handler) {}

        CefRefPtr<CefRenderHandler> GetRenderHandler() override {
            return render_handler_;
        }

        CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
            return this;
        }

        void OnAfterCreated(CefRefPtr<CefBrowser> browser) override {
            CEF_REQUIRE_UI_THREAD();
            browser_list_.push_back(browser);
        }

        void OnBeforeClose(CefRefPtr<CefBrowser> browser) override {
            CEF_REQUIRE_UI_THREAD();

            BrowserList::iterator bit = browser_list_.begin();
            for (; bit != browser_list_.end(); ++bit) {
                if ((*bit)->IsSame(browser)) {
                    browser_list_.erase(bit);
                    break;
                }
            }

            if (browser_list_.empty()) {
                // gExitFlag = true;
            }
        }

        void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) {
            mod_log("[cef] OnLoadEnd(%d)", httpStatusCode);
        }

        bool OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefLoadHandler::ErrorCode errorCode, const CefString & failedUrl, CefString & errorText) {
            mod_log("[cef] OnLoadError()");
        }

        void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) {
            mod_log("[cef] OnLoadingStateChange()");
        }

        void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) {
            mod_log("[cef] OnLoadStart()");
        }

        bool OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                           const CefString& target_url, const CefString& target_frame_name,
                           CefLifeSpanHandler::WindowOpenDisposition target_disposition,
                           bool user_gesture, const CefPopupFeatures& popupFeatures,
                           CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client,
                           CefBrowserSettings& settings, bool* no_javascript_access)
        {
            mod_log("[cef] Page wants to open a popup");

            return true;
        };

        IMPLEMENT_REFCOUNTING(BrowserClient);

    private:
        CefRefPtr<CefRenderHandler> render_handler_;
        typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
        BrowserList browser_list_;
};

class CefMinimal : public CefApp {
    public:
        ~CefMinimal() { }

        void navigate(const std::string url) {
            if (browser_.get() && browser_->GetMainFrame()) {
                mod_log("CefMinimal loading URL %s", url);
                browser_->GetMainFrame()->LoadURL(url);
            }
        }

        void clickCenter() {
            // send to CEF
            if (browser_ && browser_->GetHost()) {
                CefMouseEvent cef_mouse_event;
                cef_mouse_event.x = 512;
                cef_mouse_event.y = 512;
                cef_mouse_event.modifiers = EVENTFLAG_LEFT_MOUSE_BUTTON;

                bool is_up = false;
                int last_click_count = 1;
                browser_->GetHost()->SendMouseClickEvent(cef_mouse_event, MBT_LEFT, is_up, last_click_count);

                is_up = true;
                browser_->GetHost()->SendMouseClickEvent(cef_mouse_event, MBT_LEFT, is_up, last_click_count);
            }
        }

        void requestExit() {
            if (browser_.get() && browser_->GetHost()) {
                browser_->GetHost()->CloseBrowser(true);
            }
        }

        IMPLEMENT_REFCOUNTING(CefMinimal);

    private:
        CefRefPtr<RenderHandler> render_handler_;
        CefRefPtr<BrowserClient> browser_client_;
        CefRefPtr<CefBrowser> browser_;
};

// =======================================================================//
// !
// ! Internal CEF implementation
// !
// =======================================================================//

    CefRefPtr<CefMinimal> cef_minimal;
    CefRefPtr<CefBrowser> browser_;

    int cef_init() {
        cef_minimal = new CefMinimal();

        CefSettings settings;
        CefMainArgs args(GetModuleHandle(nullptr));

        std::string path(platform_path());

        CefString(&settings.resources_dir_path)     = path + "\\cef";
        CefString(&settings.log_file)               = path + "\\cef\\ceflog.txt";
        CefString(&settings.locales_dir_path)       = path + "\\cef\\locales";
        CefString(&settings.cache_path)             = path + "\\cef\\cache";
        CefString(&settings.user_data_path)         = path + "\\cef\\userdata";
        CefString(&settings.browser_subprocess_path)= path + "\\cef_worker.exe";
        // CefString(&settings.user_agent)             = "m2o-reborn client (HTTP 1.1, CEF Backend)";

        settings.single_process                 = false;
        settings.multi_threaded_message_loop    = false;
        settings.log_severity                   = LOGSEVERITY_WARNING; // LOGSEVERITY_ERROR;
        settings.remote_debugging_port          = 7777;
        settings.windowless_rendering_enabled   = true;
        settings.background_color               = 0x00000000;

        if (!CefInitialize(args, settings, cef_minimal, nullptr)) {
            mod_log("[error] unable to initalize cef...");
            return -1;
        }

        zpl_array_init(cef_state.objects, zpl_heap());
        zpl_array_set_capacity(cef_state.objects, 16);

        return -1;
    }

    int cef_handle_sdl(SDL_Event *event) {
        /*
        switch (e.type)
        {
            case SDL_KEYDOWN:
                {
                    CefKeyEvent event;
                    event.modifiers = getKeyboardModifiers(e.key.keysym.mod);
                    event.windows_key_code = getWindowsKeyCode(e.key.keysym);

                    event.type = KEYEVENT_RAWKEYDOWN;
                    browser->GetHost()->SendKeyEvent(event);

                    event.type = KEYEVENT_CHAR;
                    browser->GetHost()->SendKeyEvent(event);
                }
                break;

            case SDL_KEYUP:
                {
                    CefKeyEvent event;
                    event.modifiers = getKeyboardModifiers(e.key.keysym.mod);
                    event.windows_key_code = getWindowsKeyCode(e.key.keysym);

                    event.type = KEYEVENT_KEYUP;

                    browser->GetHost()->SendKeyEvent(event);
                }
                break;

            case SDL_WINDOWEVENT:
                switch (e.window.event)
                {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        renderHandler->resize(e.window.data1, e.window.data2);
                        browser->GetHost()->WasResized();
                        break;

                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        browser->GetHost()->SetFocus(true);
                        break;

                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        browser->GetHost()->SetFocus(false);
                        break;

                    case SDL_WINDOWEVENT_HIDDEN:
                    case SDL_WINDOWEVENT_MINIMIZED:
                        browser->GetHost()->SetWindowVisibility(false);
                        browser->GetHost()->WasHidden(true);
                        break;

                    case SDL_WINDOWEVENT_SHOWN:
                    case SDL_WINDOWEVENT_RESTORED:
                        browser->GetHost()->SetWindowVisibility(true);
                        browser->GetHost()->WasHidden(false);
                        break;

                    case SDL_WINDOWEVENT_CLOSE:
                        e.type = SDL_QUIT;
                        SDL_PushEvent(&e);
                        break;
                }
                break;

            case SDL_MOUSEMOTION:
                {
                    CefMouseEvent event;
                    event.x = e.motion.x;
                    event.y = e.motion.y;

                    browser->GetHost()->SendMouseMoveEvent(event, false);
                }
                break;

            case SDL_MOUSEBUTTONUP:
                {
                    CefMouseEvent event;
                    event.x = e.button.x;
                    event.y = e.button.y;

                    browser->GetHost()->SendMouseClickEvent(event, translateMouseButton(e.button), true, 1);
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                {
                    CefMouseEvent event;
                    event.x = e.button.x;
                    event.y = e.button.y;

                    browser->GetHost()->SendMouseClickEvent(event, translateMouseButton(e.button), false, 1);
                }
                break;

            case SDL_MOUSEWHEEL:
                {
                    int delta_x = e.wheel.x;
                    int delta_y = e.wheel.y;

                    if (SDL_MOUSEWHEEL_FLIPPED == e.wheel.direction)
                    {
                        delta_y *= -1;
                    }
                    else
                    {
                        delta_x *= -1;
                    }

                    CefMouseEvent event;
                    browser->GetHost()->SendMouseWheelEvent(event, delta_x, delta_y);
                }
                break;
        }
        */
        return 0;
    }

    int cef_tick() {
        CefDoMessageLoopWork();
        return 0;
    }

    int cef_free() {
        CefShutdown();
        zpl_array_free(cef_state.objects);
        return 0;
    }

    int cef_inject_event(void *evt) {
        auto event = (SDL_Event *)evt;

        if (event->type == SDL_MOUSEMOTION && input_block_get() && cef_exists(0)) {
            CefMouseEvent ax;
            ax.x = event->motion.x;
            ax.y = event->motion.y;
            cef_state.objects[0].browser.get()->GetHost()->SendMouseMoveEvent(ax, false);
        }

        return 0;
    }

// =======================================================================//
// !
// ! Resource creation/destruction
// !
// =======================================================================//

    i32 cef_handle_next() {
        int capacity = (int)zpl_array_capacity(cef_state.objects);

        if (zpl_array_count(cef_state.objects) + 1 >= capacity) {
            zpl_array_set_capacity(cef_state.objects, capacity * 2);
            capacity = (int)zpl_array_capacity(cef_state.objects);
        }

        for (; cef_state.object_cursor <= capacity; ++cef_state.object_cursor) {
            if (cef_state.object_cursor == capacity) { cef_state.object_cursor = 0; }

            if (!cef_state.objects[cef_state.object_cursor].valid) {
                zpl_zero_item(&cef_state.objects[cef_state.object_cursor]);
                cef_state.objects[cef_state.object_cursor].valid = 1;

                zpl_array_count(cef_state.objects)++;
                return cef_state.object_cursor;
            }
        }

        return -1;
    }

    cef_handle cef_browser_create(const char *url, int w, int h, int zindex) {
        int handle      = cef_handle_next();
        cef_object *obj = &cef_state.objects[handle];

        HWND win_id = (HWND)platform_windowid();

        CefWindowInfo window_info;
        window_info.SetAsWindowless(NULL);

        CefBrowserSettings settings;
        settings.windowless_frame_rate = 60;
        settings.background_color = 0x00000000;

        CefString cefurl(url);

        obj->type     = 0;
        obj->zindex   = zindex;
        obj->renderer = new RenderHandler(w, h, zindex);
        obj->client   = new BrowserClient(obj->renderer);
        obj->browser  = CefBrowserHost::CreateBrowserSync(window_info, obj->client, cefurl, settings, nullptr);

        obj->browser->GetHost()->SetFocus(true);

        return (cef_handle)handle;
    }

    int cef_exists(cef_handle handle) {
        if (handle >= 0 && handle > zpl_array_capacity(cef_state.objects)) {
            return 0;
        }

        return cef_state.objects[handle].valid;
    }

    int cef_browser_destroy(cef_handle handle) {
        if (!cef_exists(handle)) {
            return -1;
        }

        cef_object *obj = &cef_state.objects[handle];

        cef_state.objects[handle].valid = 0;
        cef_state.objects[handle].browser.get()->GetHost()->CloseBrowser(true);

        cef_state.objects[handle].browser   = NULL;
        cef_state.objects[handle].client    = NULL;
        cef_state.objects[handle].renderer  = NULL;

        return 0;
    }

// =======================================================================//
// !
// ! Operation interface
// !
// =======================================================================//

    int cef_browser_resize(cef_handle handle, int w, int h) {
        if (!cef_exists(handle)) {
            return -1;
        }

        cef_state.objects[handle].renderer.get()->resize(w, h, cef_state.objects[handle].zindex);
        cef_state.objects[handle].browser.get()->GetHost()->WasResized();

        return 0;
    }

    int cef_browser_show(cef_handle handle) {
        if (!cef_exists(handle)) {
            return -1;
        }

        cef_state.objects[handle].renderer.get()->show(cef_state.objects[handle].zindex);

        return 0;
    }

    int cef_browser_hide(cef_handle handle) {
        if (!cef_exists(handle)) {
            return -1;
        }

        cef_state.objects[handle].renderer.get()->hide();

        return 0;
    }

    int cef_zindex_get(cef_handle handle) {
        if (!cef_exists(handle)) {
            return -1;
        }

        return cef_state.objects[handle].zindex;
    }

    int cef_zindex_set(cef_handle handle, int zindex) {
        if (!cef_exists(handle)) {
            return -1;
        }

        cef_state.objects[handle].zindex = zindex;
        cef_state.objects[handle].renderer.get()->changeIndex(zindex);

        return 0;
    }

// =======================================================================//
// !
// ! Location hanlding
// !
// =======================================================================//

    int cef_browser_reload(cef_handle handle) {
        if (!cef_exists(handle)) {
            return -1;
        }

        cef_state.objects[handle].browser.get()->Reload();

        return 0;
    }

    int cef_url_set(cef_handle handle, const char *url) {
        if (!cef_exists(handle)) {
            return -1;
        }

        CefString cefurl(url);
        cef_state.objects[handle].browser.get()->GetMainFrame()->LoadURL(cefurl);

        return 0;
    }

    int cef_url_get(cef_handle handle, char *url, int maxlen) {
        if (!cef_exists(handle)) {
            return -1;
        }

        CefString cefurl = cef_state.objects[handle].browser.get()->GetMainFrame()->GetURL();
        auto str = cefurl.ToString();

        if (str.size() + 1 > (u32)maxlen) {
            return -1;
        }

        zpl_memcopy(url, str.c_str(), str.size());
        url[str.size()] = '\0';

        return str.size();
    }
