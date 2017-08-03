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
    corelog("CGraphicsManager::Init()");
    CDirect3D9Hook::Install();
    return true;
}

void CGraphicsManager::OnDeviceCreate(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters)
{
    corelog("CGraphicsManager::OnDeviceCreate(%x, %x)", pDevice, pPresentationParameters);
    m_pfontmanager = new CFontManager(pDevice);

    m_pdevice = pDevice;
    memcpy(&m_presentparams, pPresentationParameters, sizeof(D3DPRESENT_PARAMETERS));
    
    // todo: refactor
    // very important, centers initial mouse position on the zkreen
    global_mouse_state.x = pPresentationParameters->BackBufferWidth / 2;
    global_mouse_state.y = pPresentationParameters->BackBufferHeight / 2;

    GetStateAndInitialize(this);
}

void CGraphicsManager::OnDeviceLost(IDirect3DDevice9 * pDevice)
{
    corelog("CGraphicsManager::OnDeviceLost(%x)", pDevice);

    if (m_pfontmanager)
        m_pfontmanager->OnDeviceLost();
}

void CGraphicsManager::OnDeviceReset(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters)
{
    corelog("CGraphicsManager::OnDeviceReset(%x, %x)", pDevice, pPresentationParameters);

    m_pdevice = pDevice;
    m_presentparams = *pPresentationParameters;

    if (m_pfontmanager)
        m_pfontmanager->OnDeviceReset();
}

void CGraphicsManager::OnDevicePreRender(void)
{
}

void CGraphicsManager::OnDeviceRender(void)
{
    GetStateAndRender(this);

    if (!gui_initialized && global_window) {
        nk_ctx = nk_d3d9_init(m_pdevice, m_presentparams.BackBufferWidth, m_presentparams.BackBufferHeight);
       
        nk_d3d9_font_stash_begin(&nk_atlas);
        {
            // custom font
        }
        nk_style_load_all_cursors(nk_ctx, nk_atlas->cursors);
        nk_d3d9_font_stash_end();

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

        if (nk_begin(nk_ctx, "Show", nk_rect(50, 50, 220, 220),
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
                ScreenToClient(global_window, &pica);

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
