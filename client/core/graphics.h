/**
 * Graphics init
 * @return
 */
bool graphics_init()
{
    mod_log("graphics_init\n");
    CDirect3D9Hook::Install();
    return true;
}

/**
 * Terminate gfx
 */
void graphics_terminate()
{
    CDirect3D9Hook::Uninstall();
}

/**
 * Get dimensions
 * @param w
 * @param h
 */
void graphics_dimensions(int *w, int *h)
{
    *w = static_cast<int>(mod.graphics.present_params.BackBufferWidth);
    *h = static_cast<int>(mod.graphics.present_params.BackBufferHeight);
}

/**
 * Device create callback
 * @param pDevice
 * @param pPresentationParameters
 */
void graphics_device_create(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters)
{
    mod_log("CGraphicsManager::OnDeviceCreate(%x, %x)\n", pDevice, pPresentationParameters);

    mod.graphics.font_manager = (void *)new CFontManager(pDevice);
    mod.graphics.device = pDevice;

    memcpy(&mod.graphics.present_params, pPresentationParameters, sizeof(D3DPRESENT_PARAMETERS));

    // todo: refactor
    // very important, centers initial mouse position on the zkreen
    mod.mouse.x = pPresentationParameters->BackBufferWidth / 2;
    mod.mouse.y = pPresentationParameters->BackBufferHeight / 2;

    nk_ctx = nk_d3d9_init(
        mod.graphics.device,
        mod.graphics.present_params.BackBufferWidth,
        mod.graphics.present_params.BackBufferHeight
    );

    struct nk_font_config conf = nk_font_config(22);

    conf.range = nk_font_cyrillic_glyph_ranges();
    conf.oversample_h = 1;
    conf.oversample_v = 1;

    nk_d3d9_font_stash_begin(&nk_atlas);
    struct nk_font *robot = nk_font_atlas_add_from_file(nk_atlas, (mod.paths.files + "\\Roboto-Regular.ttf").c_str(), 14, &conf);
    nk_d3d9_font_stash_end();
    nk_style_load_all_cursors(nk_ctx, nk_atlas->cursors);
    nk_style_set_font(nk_ctx, &robot->handle);

    if (mod.state.init) {
        mod.state.init();
    }
}

/**
 * On device lost callback
 * @param pDevice
 */
void graphics_device_lost(IDirect3DDevice9 * pDevice)
{
    mod_log("CGraphicsManager::OnDeviceLost(%x)", pDevice);

    if (mod.graphics.font_manager) {
        ((CFontManager *)mod.graphics.font_manager)->OnDeviceLost();
    }
}

/**
 * On device reset callbacks
 * @param pDevice
 * @param pPresentationParameters
 */
void graphics_device_reset(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters)
{
    mod_log("CGraphicsManager::OnDeviceReset(%x, %x)", pDevice, pPresentationParameters);

    mod.graphics.device = pDevice;
    mod.graphics.present_params = *pPresentationParameters;

    if (mod.graphics.font_manager) {
        ((CFontManager *)mod.graphics.font_manager)->OnDeviceReset();
    }
}

/**
 * On device pre-render callback
 */
void graphics_device_prerender(void)
{
    // todo
}

/**
 * On device render callback
 */
void graphics_device_render(void)
{
    if (!mod.window) {
        return;
    }

    if (mod.state.render) {
        mod.state.render();
    }

    IDirect3DStateBlock9* pStateBlock = NULL;
    mod.graphics.device->CreateStateBlock(D3DSBT_ALL, &pStateBlock);

    nk_d3d9_render(NK_ANTI_ALIASING_ON);
    mod.graphics.device->Present(0, 0, 0, 0);

    pStateBlock->Apply();
    pStateBlock->Release();
}
