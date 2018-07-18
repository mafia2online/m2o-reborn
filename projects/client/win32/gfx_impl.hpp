#include <d3d9.h>
#include <d3dx9.h>

void graphics_device_create(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
void graphics_device_prerender();
void graphics_device_render();
void graphics_device_lost(IDirect3DDevice9*);
void graphics_device_reset(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

#include "SDL.h"
#include "SDL_ttf.h"

SDL_Renderer* renderer = NULL;
SDL_Texture *tex;
SDL_Surface *bmp;
TTF_Font *font;
SDL_Rect rect1, rect2;

/*
- x, y: upper left corner.
- texture, rect: outputs.
*/
void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {255, 255, 255, 0};

    surface = TTF_RenderUTF8_Blended(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}

SDL_Texture *texture1, *texture2;

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

int gfx_init() {
    if (_gfx_state.installed == false) {
        _gfx_state.installed = true;
        _gfx_state.method = (gfx_d3dcreate9_cb)(Mem::Hooks::InstallDetourPatch("d3d9.dll", "Direct3DCreate9", (DWORD)gfx_d3dcreate9_hook));
    }

    TTF_Init();
    return 0;
}

int gfx_free() {
    if (_gfx_state.installed) {
        _gfx_state.installed = false;
        Mem::Hooks::UninstallDetourPatch(_gfx_state.method, (DWORD)gfx_d3dcreate9_hook);
    }

    return 0;
}

void graphics_device_create(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters) {
    mod_log("[info] creating dx9 device [%x, %x] ...\n", pDevice, pPresentationParameters);

    renderer = SDL_CreateWrapperForRenderer(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, *ppReturnedDeviceInterface);
    font = TTF_OpenFont((mod_path + "\\files\\Roboto-Regular.ttf").c_str(), 44);

    bmp = SDL_LoadBMP((mod_path + "\\pug.bmp").c_str());
    if (bmp == NULL) {
        mod_log("SDL_LoadBMP Error: %s\n", SDL_GetError());
    }

    tex = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_FreeSurface(bmp);
    if (tex == NULL) {
        mod_log("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    }

    get_text_and_rect(renderer, 300, 300, "hello", font, &texture1, &rect1);
    get_text_and_rect(renderer, 300, rect1.y + rect1.h, "world", font, &texture2, &rect2);
}

void graphics_device_lost(IDirect3DDevice9 * pDevice) {
    mod_log("CGraphicsManager::OnDeviceLost(%x)", pDevice);
}

void graphics_device_reset(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters) {
    mod_log("CGraphicsManager::OnDeviceReset(%x, %x)", pDevice, pPresentationParameters);
}

void graphics_device_prerender(void) {

}

void graphics_device_render(void) {
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    // SDL_RenderClear(renderer);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 15);
    // SDL_RenderDrawLine(renderer, 0, 0, 300, 240);
    // SDL_RenderDrawLine(renderer, 300, 240, 340, 240);
    // SDL_RenderDrawLine(renderer, 340, 240, 320, 200);
    // // SDL_RenderPresent(renderer);

    // if (tex) {
    //     SDL_RenderCopy(renderer, tex, NULL, NULL);
    // }
    // SDL_Rect position = {0, 0, 300, 300};
    // SDL_RenderFillRect(renderer, &position);

    if (texture1 && texture2) {
        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        SDL_RenderCopy(renderer, texture2, NULL, &rect2);
    }
}

void gfx_util_screensize(int *w, int *h) {
    // *w = static_cast<int>(mod.graphics.present_params.BackBufferWidth);
    // *h = static_cast<int>(mod.graphics.present_params.BackBufferHeight);
    *w = 800;
    *h = 600;
}
