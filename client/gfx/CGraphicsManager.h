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

    ImGui_ImplDX9_Shutdown();

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
        ImGui_ImplDX9_Init(global_window, m_pdevice);
        gui_initialized = true;
    }

    if (gui_initialized) {
        ImGui_ImplDX9_NewFrame();

        ImGui::ShowTestWindow();
        {
            static bool show_test_window, show_another_window;
            static float f = 0.0f;
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            if (ImGui::Button("Test Window")) show_test_window ^= 1;
            if (ImGui::Button("Another Window")) show_another_window ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

        //m_pdevice->BeginScene();
        ImGui::Render();
        //m_pdevice->EndScene();
        m_pdevice->Present(0, 0, 0, 0);
    }
}
