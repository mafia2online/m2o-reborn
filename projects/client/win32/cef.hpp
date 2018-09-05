#include <cef_app.h>
#include <cef_client.h>
#include <cef_render_handler.h>
#include <cef_life_span_handler.h>
#include <cef_load_handler.h>
#include <wrapper/cef_helpers.h>

#define CEF_DEFAULT_GFX_LAYER 200

struct cefm_state_t {
    zpl_array(CefRefPtr<CefBrowser>) browsers;
};

static cefm_state_t cef_state;

// =======================================================================//
// !
// ! Extended CEF classes
// !
// =======================================================================//

class CEF_RenderHandler : public CefRenderHandler {
public:
    CEF_RenderHandler(int w, int h) :
        width(w),
        height(h) {
        texture = gfx_create_texture(w, h);
        gfx_render_add(texture, CEF_DEFAULT_GFX_LAYER);
    }

    ~CEF_RenderHandler() {
        gfx_destroy(texture);
    }

    bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) {
        rect = CefRect(0, 0, width, height);
        return true;
    }

    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int w, int h) {
        CEF_REQUIRE_UI_THREAD();
        gfx_update_texture(texture, buffer, w, h);
    }

    void resize(int w, int h) {
        gfx_destroy(texture);

        texture = gfx_create_texture(w, h);
        gfx_render_add(texture, CEF_DEFAULT_GFX_LAYER);

        width = w;
        height = h;
    }

private:
    int width;
    int height;
    gfx_handle texture = -1;

    IMPLEMENT_REFCOUNTING(CEF_RenderHandler);
};

// for manual render handler
class CEF_BrowserClient : public CefClient, public CefLifeSpanHandler, public CefLoadHandler {
public:
    CEF_BrowserClient(CefRefPtr<CefRenderHandler> ptr) : handler(ptr) {}

    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() {
        return this;
    }

    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() {
        return this;
    }

    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() {
        return handler;
    }

    // CefLifeSpanHandler methods.
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) {
        // Must be executed on the UI thread.
        CEF_REQUIRE_UI_THREAD();

        browser_id = browser->GetIdentifier();
    }

    bool DoClose(CefRefPtr<CefBrowser> browser) {
        // Must be executed on the UI thread.
        CEF_REQUIRE_UI_THREAD();

        // Closing the main window requires special handling. See the DoClose()
        // documentation in the CEF header for a detailed description of this
        // process.
        if (browser->GetIdentifier() == browser_id) {
            // Set a flag to indicate that the window close should be allowed.
            closing = true;
        }

        // Allow the close. For windowed browsers this will result in the OS close
        // event being sent.
        return false;
    }

    void OnBeforeClose(CefRefPtr<CefBrowser> browser) {}

    void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) {
        mod_log("OnLoadEnd(%d)", httpStatusCode);
        loaded = true;
    }

    bool OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefLoadHandler::ErrorCode errorCode, const CefString & failedUrl, CefString & errorText) {
        mod_log("OnLoadError()");
        loaded = true;
    }

    void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) {
        mod_log("OnLoadingStateChange()");
    }

    void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) {
        mod_log("OnLoadStart()");
    }

    bool closeAllowed() const {
        return closing;
    }

    bool isLoaded() const {
        return loaded;
    }

private:
    int browser_id;
    bool closing = false;
    bool loaded = false;
    CefRefPtr<CefRenderHandler> handler;

    IMPLEMENT_REFCOUNTING(CEF_BrowserClient);
};

// =======================================================================//
// !
// ! Internal CEF implementation
// !
// =======================================================================//

int cef_init() {
    CefSettings settings;
    CefMainArgs args(GetModuleHandle(nullptr));

    std::string path(platform_path());

    CefString(&settings.resources_dir_path)     = path + "\\cef";
    CefString(&settings.log_file)               = path + "\\cef\\ceflog.txt";
    CefString(&settings.locales_dir_path)       = path + "\\cef\\locales";
    CefString(&settings.cache_path)             = path + "\\cef\\cache";
    CefString(&settings.browser_subprocess_path)= path + "\\cef_worker.exe";

    // settings.single_process = true;
    settings.multi_threaded_message_loop    = false;
    settings.log_severity                   = LOGSEVERITY_WARNING; // LOGSEVEкRITY_ERROR;
    settings.remote_debugging_port          = 7777;
    settings.windowless_rendering_enabled   = true;

    if (CefInitialize(args, settings, nullptr, nullptr)) {
        mod_log("CefMinimal initialized okay");
        return 0;
    }

    return -1;
}

cef_handle cef_browser_create(const char *url, int w, int h) {
    auto client = new CEF_BrowserClient(new CEF_RenderHandler(w, h));
    HWND win_id = (HWND)platform_windowid();

    CefWindowInfo window;
    window.SetAsWindowless(win_id);

    CefBrowserSettings settings;
    settings.windowless_frame_rate  = 60;
    settings.background_color       = 0xffffffff;

    CefString cefurl(url);

    auto browser = CefBrowserHost::CreateBrowserSync(window, client, cefurl, settings, nullptr);

    return 0;
}

int cef_tick() {
    CefDoMessageLoopWork();
    return 0;
}
