CGraphicsManager m_graphicsmanager;
CMPStateManager m_statemanager;

void GetStateAndInitialize(void *ptr) {
    m_statemanager.InitializeResources(ptr);
}

void GetStateAndRender(void *ptr) {
    m_statemanager.Render(ptr);
}

void gfx_OnDeviceCreate(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
    m_graphicsmanager.OnDeviceCreate(pDevice, pPresentationParameters);
}

void gfx_OnDevicePreRender() {
    m_graphicsmanager.OnDevicePreRender();
}

void gfx_OnDeviceRender() {
    m_graphicsmanager.OnDeviceRender();
}

void gfx_OnDeviceLost(IDirect3DDevice9* pDevice) {
    m_graphicsmanager.OnDeviceLost(pDevice);
}

void gfx_OnDeviceReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
    m_graphicsmanager.OnDeviceReset(pDevice, pPresentationParameters);
}
