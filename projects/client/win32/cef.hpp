class RenderHandler : public CefRenderHandler {
public:
    RenderHandler(SDL_Renderer *renderer, int w, int h) :
        width(w),
        height(h),
        renderer(renderer) {
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_STREAMING, w, h);
    }

    ~RenderHandler() {
        if (texture) {
            SDL_DestroyTexture(texture);
        }

        renderer = nullptr;
    }

    bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) {
        rect = CefRect(0, 0, width, height);
        return true;
    }

    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int w, int h) {
        CEF_REQUIRE_UI_THREAD();

        if (texture) {
            unsigned char *texture_data = NULL;
            int texture_pitch = 0;

            SDL_LockTexture(texture, 0, (void **)&texture_data, &texture_pitch);
            memcpy(texture_data, buffer, w * h * 4);
            SDL_UnlockTexture(texture);
        }
    }

    void resize(int w, int h) {
        if (texture) {
            SDL_DestroyTexture(texture);
        }

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_STREAMING, w, h);
        width = w;
        height = h;
    }

    void render() {
        SDL_RenderCopy(renderer, texture, NULL, NULL);
    }

private:
    int width;
    int height;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *texture = nullptr;

    IMPLEMENT_REFCOUNTING(RenderHandler);
};



// for manual render handler
class BrowserClient : public CefClient, public CefLifeSpanHandler, public CefLoadHandler {
public:
    BrowserClient(CefRefPtr<CefRenderHandler> ptr) : handler(ptr) {}

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
        std::cout << "OnLoadEnd(" << httpStatusCode << ")" << std::endl;
        loaded = true;
    }

    bool OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefLoadHandler::ErrorCode errorCode, const CefString & failedUrl, CefString & errorText) {
        std::cout << "OnLoadError()" << std::endl;
        loaded = true;
    }

    void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) {
        std::cout << "OnLoadingStateChange()" << std::endl;
    }

    void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) {
        std::cout << "OnLoadStart()" << std::endl;
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

    IMPLEMENT_REFCOUNTING(BrowserClient);
};


int cef_init() {
    CefSettings settings;
    CefMainArgs args(GetModuleHandle(nullptr));

    CefString(&settings.resources_dir_path)     = mod.paths.index + "\\cef";
    CefString(&settings.log_file)               = mod.paths.index + "\\cef\\ceflog.txt";
    CefString(&settings.locales_dir_path)       = mod.paths.index + "\\cef\\locales";
    CefString(&settings.cache_path)             = mod.paths.index + "\\cef\\cache";
    CefString(&settings.browser_subprocess_path)= mod.paths.index + "\\cef_worker.exe";

    // settings.single_process = true;
    settings.multi_threaded_message_loop    = false;
    settings.log_severity                   = LOGSEVERITY_WARNING; // LOGSEVEкRITY_ERROR;
    settings.remote_debugging_port          = 7777;
    settings.windowless_rendering_enabled   = true;

    if (CefInitialize(args, settings, this, nullptr)) {
        std::cout << "CefMinimal initialized okay" << std::endl;

        return 1;
    }

    return 0
}

cef_handle cef_browser_create(const char *url, int w, int h) {
    render_handler_ = new RenderHandler();
    browser_client_ = new BrowserClient(render_handler_);

    CefWindowInfo window_info;
    window_info.SetAsWindowless(mod.window);

    CefBrowserSettings browser_settings;
    browser_settings.windowless_frame_rate = 60;
    browser_settings.background_color = 0xffffffff;

    CefString url = "https://google.com/";
    browser_ = CefBrowserHost::CreateBrowserSync(window_info, browser_client_.get(), url, browser_settings, nullptr);
}
