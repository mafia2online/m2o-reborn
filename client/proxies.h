CGraphicsManager global_gfx;
CMPStateManager global_state;

void GetStateAndInitialize(void *ptr) {
    global_state.InitializeResources(ptr);
}

void GetStateAndRender(void *ptr) {
    global_state.Render(ptr);
}

void gfx_OnDeviceCreate(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
    global_gfx.OnDeviceCreate(pDevice, pPresentationParameters);
}

void gfx_OnDevicePreRender() {
    global_gfx.OnDevicePreRender();
}

void gfx_OnDeviceRender() {
    global_gfx.OnDeviceRender();
}

void gfx_OnDeviceLost(IDirect3DDevice9* pDevice) {
    global_gfx.OnDeviceLost(pDevice);
}

void gfx_OnDeviceReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
    global_gfx.OnDeviceReset(pDevice, pPresentationParameters);
}
