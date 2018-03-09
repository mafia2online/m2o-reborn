// =======================================================================//
// !
// ! Generic gfx methods
// !
// =======================================================================//

/**
 * Graphics init
 * @return
 */
bool graphics_init() {
    mod_log("[info] initializing graphics...\n");
    CDirect3D9Hook::Install();
    return true;
}

/**
 * Terminate gfx
 */
void graphics_terminate() {
    CDirect3D9Hook::Uninstall();
}

/**
 * Get dimensions
 * @param w
 * @param h
 */
inline void graphics_dimensions(int *w, int *h) {
    *w = static_cast<int>(mod.graphics.present_params.BackBufferWidth);
    *h = static_cast<int>(mod.graphics.present_params.BackBufferHeight);
}

void graphics_world_to_screen(vec3_t *screen, vec3_t world) {
    auto camera = M2::C_GameCamera::Get()->GetCamera(1);

    // Get the world view projection matrix
    D3DXMATRIX mat;
    memcpy(&mat, &camera->m_pGameCamera->m_worldViewProjection, sizeof(D3DXMATRIX));
    D3DXMatrixTranspose(&mat, &mat);

    // Get the viewport
    D3DVIEWPORT9 viewport;
    mod.graphics.device->GetViewport(&viewport);

    // Transform the world coordinate by the worldViewProjection matrix
    D3DXVECTOR3 vec;
    D3DXVec3TransformCoord(&vec, &D3DXVECTOR3(world.x, world.y, world.z), &mat);

    screen->x = viewport.X + (1.0f + vec.x) * viewport.Width / 2.0f;
    screen->y = viewport.Y + (1.0f - vec.y) * viewport.Height / 2.0f;
    screen->z = viewport.MinZ + vec.z * (viewport.MaxZ - viewport.MinZ);
}

// =======================================================================//
// !
// ! Generic gfx events
// !
// =======================================================================//

void mod_nk_create() {
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
}

/**
 * Device create callback
 * @param pDevice
 * @param pPresentationParameters
 */
void graphics_device_create(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters) {
    mod_log("[info] creating dx9 device [%x, %x] ...\n", pDevice, pPresentationParameters);

    mod.graphics.font_manager = (void *)new CFontManager(pDevice);
    mod.graphics.device = pDevice;

    memcpy(&mod.graphics.present_params, pPresentationParameters, sizeof(D3DPRESENT_PARAMETERS));

    mod_nk_create();

    if (mod.state.init) {
        mod.state.init();
    }

    auto fm = (CFontManager *)mod.graphics.font_manager;

    fm->AddFont("Ingame", "Aurora BdCn BT", 22, false);
    fm->AddFont("TitleUIFont", "Aurora BdCn BT", 26, false);
    fm->AddFont("TitleUIFontBig", "Aurora BdCn BT", 32, false);

    mod_assert(nk_ctx && nk_atlas->temporary.alloc);
}

/**
 * On device lost callback
 * @param pDevice
 */
void graphics_device_lost(IDirect3DDevice9 * pDevice) {
    mod_log("CGraphicsManager::OnDeviceLost(%x)", pDevice);

    if (mod.graphics.font_manager) {
        ((CFontManager *)mod.graphics.font_manager)->OnDeviceLost();
    }

    nk_d3d9_shutdown();
}

/**
 * On device reset callbacks
 * @param pDevice
 * @param pPresentationParameters
 */
void graphics_device_reset(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters) {
    mod_log("CGraphicsManager::OnDeviceReset(%x, %x)", pDevice, pPresentationParameters);

    mod.graphics.device = pDevice;
    mod.graphics.present_params = *pPresentationParameters;

    if (mod.graphics.font_manager) {
        ((CFontManager *)mod.graphics.font_manager)->OnDeviceReset();
    }

    mod_nk_create();
}

// =======================================================================//
// !
// ! Render gfx events
// !
// =======================================================================//

/**
 * On device pre-render callback
 */
inline void graphics_device_prerender(void) {
    // todo
    mod_assert(nk_ctx && nk_atlas->temporary.alloc);

    if (mod.state.prerender) {
        mod.state.prerender();
    }
}

/**
 * Hanlding input in for gui in the same thread
 */
inline void graphics_input_handling() {
    mod_assert(nk_ctx && nk_atlas->temporary.alloc);

    // start input capture
    nk_input_begin(nk_ctx);
    if (mod.input_blocked) {
        nk_style_show_cursor(nk_ctx);

        // input mouse btns
        for (usize i = 0; i < 3; i++)
            nk_input_button(nk_ctx, mod.mouse.buttons[i].id, mod.mouse.x, mod.mouse.y, mod.mouse.buttons[i].state);

        // input scroll
        nk_input_scroll(nk_ctx, nk_vec2(0, (float)mod.mouse.z / WHEEL_DELTA));

        // input mouse move
        nk_input_motion(nk_ctx, mod.mouse.x, mod.mouse.y);
    }
    else {
        nk_style_hide_cursor(nk_ctx);
    }

    static bool __pressed_ctrl = false;

    // handle queue of msgs
    zpl_mutex_try_lock(&mod.mutexes.wnd_msg);
    while (!mod.wnd_msg.empty()) {
        auto msg = mod.wnd_msg.front(); mod.wnd_msg.pop();

        if (mod.input_blocked) {
            nk_d3d9_handle_event(msg.hWnd, msg.uMsg, msg.wParam, msg.lParam);
        }

        switch (msg.uMsg) {
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN: {
                switch (msg.wParam) {
                    case VK_CONTROL:
                    case VK_LCONTROL:
                    case VK_RCONTROL:
                        __pressed_ctrl = true;
                        break;
                }
            } break;

            case WM_KEYUP:
            case WM_SYSKEYUP: {
                switch (msg.wParam) {
                    case VK_CONTROL:
                    case VK_LCONTROL:
                    case VK_RCONTROL:
                        __pressed_ctrl = false;
                        break;

                    /* trigger exec in debug console */
                    case VK_RETURN:
                        if (mod.console.enabled) {
                            mod_debug_console_execute();
                        }
                        break;

                    /* trigger the debug console */
                    case VK_OEM_3:
                        mod.console.enabled = !mod.console.enabled;
                        mod.input_blocked   = (mod.console.enabled);
                        break;
                }
            } break;
        }

        if (msg.uMsg == WM_CHAR && __pressed_ctrl) {
            int down = !((msg.lParam >> 31) & 1);
            // mod_log("%d", msg.wParam); /* dbg char */

            switch (msg.wParam) {
                case 1: nk_input_key(&d3d9.ctx, NK_KEY_TEXT_SELECT_ALL, true); nk_input_key(&d3d9.ctx, NK_KEY_TEXT_SELECT_ALL, false); break;
                case 3: nk_input_key(&d3d9.ctx, NK_KEY_COPY, true); nk_input_key(&d3d9.ctx, NK_KEY_COPY, false); break;
                case 22: nk_input_key(&d3d9.ctx, NK_KEY_PASTE, true); nk_input_key(&d3d9.ctx, NK_KEY_PASTE, false); break;
                case 24: nk_input_key(&d3d9.ctx, NK_KEY_CUT, true); nk_input_key(&d3d9.ctx, NK_KEY_CUT, false); break;
                case 26: nk_input_key(&d3d9.ctx, NK_KEY_TEXT_UNDO, true); nk_input_key(&d3d9.ctx, NK_KEY_TEXT_UNDO, false); break;
                case 18: nk_input_key(&d3d9.ctx, NK_KEY_TEXT_REDO, true); nk_input_key(&d3d9.ctx, NK_KEY_TEXT_REDO, false); break;
            }
        }
    }
    zpl_mutex_unlock(&mod.mutexes.wnd_msg);

    // finish input queue
    nk_input_end(nk_ctx);
}


/**
 * On device render callback
 */
inline void graphics_device_render(void) {
    if (!mod.window) {
        return;
    }

    graphics_input_handling();

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
