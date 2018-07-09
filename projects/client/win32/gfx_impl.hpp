#include <d3d9.h>
#include <d3dx9.h>

void graphics_device_create(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
void graphics_device_prerender();
void graphics_device_render();
void graphics_device_lost(IDirect3DDevice9*);
void graphics_device_reset(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

#include "SDL_render.h"
SDL_Renderer* renderer = NULL;
SDL_Texture *tex;
SDL_Surface *bmp;

#include "gfx/CDirect3DDevice9Proxy.h"
#include "gfx/CDirect3D9Proxy.h"

// =======================================================================//
// !
// ! Hooking
// !
// =======================================================================//

typedef IDirect3D9 *(WINAPI *gfx_d3dcreate9_cb)(UINT);

struct mod_graphics_t {
    IDirect3DDevice9 *device;
    D3DPRESENT_PARAMETERS present_params;
    void *font_manager;
};

struct gfx_state {
    bool installed;
    gfx_d3dcreate9_cb method;
};

static gfx_state _gfx_state;



IDirect3D9 *WINAPI gfx_d3dcreate9_hook(UINT SDKVersion) {
    // Call the real device
    IDirect3D9 *pD3D = _gfx_state.method(SDKVersion);

    // If the call was successful return our proxy device
    if (pD3D) {
        return new CDirect3D9Proxy(pD3D);
    }

    MessageBox(NULL, "Unable to create Direct3D9 interface.", "Fatal error", MB_ICONERROR);
    TerminateProcess(GetCurrentProcess(), 0);

    return NULL;
}

// =======================================================================//
// !
// ! Control methods
// !
// =======================================================================//

void gfx_init() {
    if (_gfx_state.installed == false) {
        _gfx_state.installed = true;
        _gfx_state.method = (gfx_d3dcreate9_cb)(Mem::Hooks::InstallDetourPatch("d3d9.dll", "Direct3DCreate9", (DWORD)gfx_d3dcreate9_hook));
    }
}

void gfx_free() {
    if (_gfx_state.installed) {
        _gfx_state.installed = false;
        Mem::Hooks::UninstallDetourPatch(_gfx_state.method, (DWORD)gfx_d3dcreate9_hook);
    }
}

void graphics_device_create(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters) {
    mod_log("[info] creating dx9 device [%x, %x] ...\n", pDevice, pPresentationParameters);
}

void graphics_device_lost(IDirect3DDevice9 * pDevice) {
    mod_log("CGraphicsManager::OnDeviceLost(%x)", pDevice);
}

void graphics_device_reset(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters) {
    mod_log("CGraphicsManager::OnDeviceReset(%x, %x)", pDevice, pPresentationParameters);
}

inline void graphics_device_prerender(void) {}
inline void graphics_device_render(void) {
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    // SDL_RenderClear(renderer);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 15);
    SDL_RenderDrawLine(renderer, 0, 0, 300, 240);
    SDL_RenderDrawLine(renderer, 300, 240, 340, 240);
    SDL_RenderDrawLine(renderer, 340, 240, 320, 200);
    // SDL_RenderPresent(renderer);

    if (tex) {
        SDL_RenderCopy(renderer, tex, NULL, NULL);
    }

    SDL_Rect position = {0, 0, 300, 300};
    SDL_RenderFillRect(renderer, &position);
}

void gfx_screen_size(int *x, int *y) {
    // *w = static_cast<int>(mod.graphics.present_params.BackBufferWidth);
    // *h = static_cast<int>(mod.graphics.present_params.BackBufferHeight);
    *x = 800;
    *y = 600;
}
