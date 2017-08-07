class CGraphicsManager
{
public:
    CGraphicsManager();
    ~CGraphicsManager();
    bool Init();

public:
    void OnDeviceCreate(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
    void OnDeviceLost(IDirect3DDevice9* pDevice);
    void OnDeviceReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
    void OnDevicePreRender(void);
    void OnDeviceRender(void);

    CFontManager* GetFontManager() { return m_pfontmanager; }


    void GetScreenDimensions(int *w, int *h)
    {
        *w = static_cast<int>(m_presentparams.BackBufferWidth);
        *h = static_cast<int>(m_presentparams.BackBufferHeight);
    }

private:
    bool         gui_initialized;
    CFontManager* m_pfontmanager;
    IDirect3DDevice9* m_pdevice;
    D3DPRESENT_PARAMETERS m_presentparams;
};

void GetStateAndInitialize(void *);
void GetStateAndRender(void *);

CGraphicsManager::CGraphicsManager()
{
    gui_initialized = false;
}

CGraphicsManager::~CGraphicsManager()
{
    CDirect3D9Hook::Uninstall();
}

bool CGraphicsManager::Init()
{
    mod_log("CGraphicsManager::Init()");
    CDirect3D9Hook::Install();
    return true;
}

void CGraphicsManager::OnDeviceCreate(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters)
{
    mod_log("CGraphicsManager::OnDeviceCreate(%x, %x)", pDevice, pPresentationParameters);
    m_pfontmanager = new CFontManager(pDevice);

    m_pdevice = pDevice;
    memcpy(&m_presentparams, pPresentationParameters, sizeof(D3DPRESENT_PARAMETERS));
    
    // todo: refactor
    // very important, centers initial mouse position on the zkreen
    mod.mouse.x = pPresentationParameters->BackBufferWidth / 2;
    mod.mouse.y = pPresentationParameters->BackBufferHeight / 2;

    GetStateAndInitialize(this);
}

void CGraphicsManager::OnDeviceLost(IDirect3DDevice9 * pDevice)
{
    mod_log("CGraphicsManager::OnDeviceLost(%x)", pDevice);

    if (m_pfontmanager)
        m_pfontmanager->OnDeviceLost();
}

void CGraphicsManager::OnDeviceReset(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters)
{
    mod_log("CGraphicsManager::OnDeviceReset(%x, %x)", pDevice, pPresentationParameters);

    m_pdevice = pDevice;
    m_presentparams = *pPresentationParameters;

    if (m_pfontmanager)
        m_pfontmanager->OnDeviceReset();
}

void CGraphicsManager::OnDevicePreRender(void)
{
}

struct nk_font_config conf = nk_font_config(22);


void CGraphicsManager::OnDeviceRender(void)
{
    GetStateAndRender(this);

    if (!gui_initialized && mod.window) {
        nk_ctx = nk_d3d9_init(m_pdevice, m_presentparams.BackBufferWidth, m_presentparams.BackBufferHeight);

        conf.range = nk_font_cyrillic_glyph_ranges();
        conf.oversample_h = 1;
        conf.oversample_v = 1;

        // nk_d3d9_font_stash_begin(&nk_atlas);
        // {
        //     // custom font
        // }
        // nk_style_load_all_cursors(nk_ctx, nk_atlas->cursors);
        // nk_d3d9_font_stash_end();

        // nk_d3d9_font_stash_begin(&nk_atlas);
        // struct nk_font *font = nk_font_atlas_add_from_file(nk_atlas, (mod.paths.files + "\\Roboto-Regular.ttf").c_str(), 22, &conf);
        // nk_d3d9_font_stash_end();
        // nk_style_set_font(nk_ctx, &font->handle);


        // {struct nk_font_atlas *atlas;
        // nk_d3d9_font_stash_begin(&atlas);

        // struct nk_font *genshin = nk_font_atlas_add_from_file(atlas, (mod.paths.files + "\\Roboto-Regular.ttf").c_str(), 22, &conf);
        // nk_d3d9_font_stash_end();
        // nk_style_set_font(nk_ctx, &genshin->handle);}

        // mod_log((mod.paths.files + "\\Roboto-Regular.ttf").c_str());

        /* Load Fonts: if none of these are loaded a default font will be used  */
        /* Load Cursor: if you uncomment cursor loading please hide the cursor */
        {nk_d3d9_font_stash_begin(&nk_atlas);
        /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../extra_font/DroidSans.ttf", 14, 0);*/
        struct nk_font *robot = nk_font_atlas_add_from_file(nk_atlas, (mod.paths.files + "\\Roboto-Regular.ttf").c_str(), 14, &conf);
        /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
        /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../extra_font/ProggyClean.ttf", 12, 0);*/
        /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../extra_font/ProggyTiny.ttf", 10, 0);*/
        /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../extra_font/Cousine-Regular.ttf", 13, 0);*/
        nk_d3d9_font_stash_end();
        nk_style_load_all_cursors(nk_ctx, nk_atlas->cursors);
        nk_style_set_font(nk_ctx, &robot->handle);}



        gui_initialized = true;
    }

    if (gui_initialized) {

        // THIS IS A HACK AND NEEDS ACTUAL PROPER PLACE
        // It will fuck up button press states and whatnot.
        if (0)
        {
            nk_input_begin(nk_ctx);
            
            nk_input_end(nk_ctx);
        }

        enum { EASY, HARD };
        static int op = EASY;
        static float value = 0.6f;
        static int i = 20;

        if (nk_begin(nk_ctx, "Show Привет", nk_rect(50, 50, 220, 220),
            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE)) {
            /* fixed widget pixel width */
            nk_layout_row_static(nk_ctx, 30, 80, 1);
            if (nk_button_label(nk_ctx, "button")) {
                /* event handling */
            }

            /* fixed widget window ratio width */
            nk_layout_row_dynamic(nk_ctx, 30, 2);
            if (nk_option_label(nk_ctx, "easy", op == EASY)) op = EASY;
            if (nk_option_label(nk_ctx, "hard", op == HARD)) op = HARD;

            /* custom widget pixel width */
            nk_layout_row_begin(nk_ctx, NK_STATIC, 30, 2);
            {
                nk_layout_row_push(nk_ctx, 50);

                POINT pica;
                GetCursorPos(&pica);
                ScreenToClient(mod.window, &pica);

                char cipa[64] = { 0 };
                sprintf(cipa, "X: %d, Y: %d", pica.x, pica.y);

                nk_label(nk_ctx, cipa, NK_TEXT_LEFT);
                nk_layout_row_push(nk_ctx, 110);
                nk_slider_float(nk_ctx, 0, &value, 1.0f, 0.1f);
            }
            nk_layout_row_end(nk_ctx);
        }
        nk_end(nk_ctx);

        IDirect3DStateBlock9* pStateBlock = NULL;
        m_pdevice->CreateStateBlock(D3DSBT_ALL, &pStateBlock);

        nk_d3d9_render(NK_ANTI_ALIASING_ON);
        m_pdevice->Present(0, 0, 0, 0);

        pStateBlock->Apply();
        pStateBlock->Release();
    }
}
