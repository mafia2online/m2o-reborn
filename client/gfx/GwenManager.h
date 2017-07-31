class GwenManager
{
public:
    GwenManager(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters);

    void OnDeviceRender();
    Gwen::Input::Windows& GetInput() { return m_gweninput; }
    Gwen::Controls::Canvas* GetCanvas() { return m_pcanvas; }

private:
    IDirect3DDevice9 *m_pdevice;

private:
    Gwen::Renderer::DirectX9* m_prenderer;
    Gwen::Skin::TexturedBase* m_pskin;
    Gwen::Controls::Canvas* m_pcanvas;
    Gwen::Input::Windows m_gweninput;
};
GwenManager::GwenManager(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters)
{
    m_pdevice = pDevice;

    // Create a GWEN DirectX renderer
    m_prenderer = new Gwen::Renderer::DirectX9(m_pdevice);

    // Create a GWEN skin
    m_pskin = new Gwen::Skin::TexturedBase(m_prenderer);
    m_pskin->Init((CCore::Instance().GetModFilesDir() + "\\DefaultSkin.png").GetCStr());

    // Create a Canvas (it's root, on which all other GWEN panels are created)
    m_pcanvas = new Gwen::Controls::Canvas(m_pskin);
    m_pcanvas->SetSize(pPresentationParameters->BackBufferWidth, pPresentationParameters->BackBufferHeight);
    m_pcanvas->SetDrawBackground(false);
    m_pcanvas->SetBackgroundColor(Gwen::Color(0, 0, 0, 100));

    m_gweninput.Initialize(m_pcanvas);

    CDebugConsole::Instance().RegisterComponents(m_pcanvas);


    m_pcanvas->SetMouseInputEnabled(true);
    m_pcanvas->SetKeyboardInputEnabled(true);
}

void GwenManager::OnDeviceRender()
{
    if(m_pcanvas)
        m_pcanvas->RenderCanvas();
}
