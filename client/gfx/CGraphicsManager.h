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
    GwenManager* GetGwenManager() { return m_pGwenManager; }


    void GetScreenDimensions(int *w, int *h)
    {
        *w = static_cast<int>(m_presentparams.BackBufferWidth);
        *h = static_cast<int>(m_presentparams.BackBufferHeight);
    }

private:
    bool         gui_initialized;
    GwenManager* m_pGwenManager;
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
    m_pGwenManager = new GwenManager(pDevice, pPresentationParameters);

    m_pdevice = pDevice;
    memcpy(&m_presentparams, pPresentationParameters, sizeof(D3DPRESENT_PARAMETERS));
    
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
    //m_pGwenManager->OnDeviceRender();

    if (!gui_initialized && global_window) {
        nk_ctx = nk_d3d9_init(m_pdevice, m_presentparams.BackBufferWidth, m_presentparams.BackBufferHeight);
       
        nk_d3d9_font_stash_begin(&nk_atlas);
        {
            // custom font
        }

        nk_d3d9_font_stash_end();

        gui_initialized = true;
    }

    if (gui_initialized) {
        if (nk_begin(nk_ctx, "Demo", nk_rect(50, 50, 230, 250),
            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
            NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
        {
            nk_label(nk_ctx, "background:", NK_TEXT_LEFT);
        }
        nk_end(nk_ctx);

        
        nk_d3d9_render(NK_ANTI_ALIASING_OFF);
        m_pdevice->Present(0, 0, 0, 0);
    }
}
