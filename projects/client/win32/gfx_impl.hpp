#include <d3d9.h>
#include <d3dx9.h>

#include "SDL.h"
#include "SDL_ttf.h"

typedef IDirect3D9 *(WINAPI *gfx_d3dcreate9_cb)(UINT);

struct gfx_object {
    u8 type;

    u8 valid : 1;
    u8 metadata : 7;

    union {
        SDL_Texture *texture;
        void *rawptr;
    };

    union {
        struct { // rectangle, texture
            i32 x, y, w, h;
        };

        struct { // line
            i32 x1, y1, x2, y2;
        };

        i32 raw[4];
    } data;

    vec4 color;
    f32 scalex, scaley;
};

struct gfx_t {
    bool installed;
    char fontregistry[8][MAX_PATH];

    zpl_array(gfx_object) objects;
    int object_cursor;

    SDL_Renderer *rnd;

    gfx_d3dcreate9_cb       method;
    IDirect3DDevice9        *device;
    D3DPRESENT_PARAMETERS   present_params;
};

enum {GFX_TEXTURE, GFX_RECTANGLE, GFX_LINE};

static gfx_t gfx_state;

// =======================================================================//
// !
// ! Implementation
// !
// =======================================================================//

int gfx_font_add(int fontid, const char *filename) {
    if (gfx_font_exists(fontid)) {
        return -1;
    }

    if (!zpl_file_exists(filename)) {
        return -2;
    }

    zpl_memcopy(&gfx_state.fontregistry[fontid], filename, zpl_strlen(filename));
    return 0;
}

int gfx_font_exists(int fontid) {
    if (gfx_state.fontregistry[fontid][0] == 0) {
        return 0;
    }

    return 1;
}

int gfx_font_remove(int fontid) {
    if (!gfx_font_exists(fontid)) {
        return -1;
    }

    zpl_zero_item(gfx_state.fontregistry[fontid]);
    return 1;
}

TTF_Font *gfx_font_request(int fontid, int size) {
    return NULL;
}

i32 gfx_handle_next() {
    int capacity = (int)zpl_array_capacity(gfx_state.objects);

    if (zpl_array_count(gfx_state.objects) + 1 >= capacity) {
        zpl_array_set_capacity(gfx_state.objects, capacity * 2);
        capacity = (int)zpl_array_capacity(gfx_state.objects);
    }

    for (; gfx_state.object_cursor <= capacity; ++gfx_state.object_cursor) {
        if (gfx_state.object_cursor == capacity) { gfx_state.object_cursor = 0; }

        if (!gfx_state.objects[gfx_state.object_cursor].valid) {
            zpl_zero_item(&gfx_state.objects[gfx_state.object_cursor]);
            gfx_state.objects[gfx_state.object_cursor].valid = 1;

            zpl_array_count(gfx_state.objects)++;
            return gfx_state.object_cursor;
        }
    }

    return -1;
}

gfx_handle gfx_create_texture(int w, int h) {
    ZPL_ASSERT(gfx_state.rnd);

    int handle      = gfx_handle_next();
    gfx_object *obj = &gfx_state.objects[handle];

    obj->type    = GFX_TEXTURE;
    obj->texture = SDL_CreateTexture(gfx_state.rnd, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_STREAMING, w, h);
    obj->scalex  = 1.0f;
    obj->scaley  = 1.0f;
    obj->data.x  = 0;
    obj->data.y  = 0;
    obj->data.w  = w;
    obj->data.h  = h;

    if (!obj->texture) {
        return -1;
    }

    return handle;
}

gfx_handle gfx_create_texture_file(const char *path) {
    ZPL_ASSERT(gfx_state.rnd);

    int handle      = gfx_handle_next();
    gfx_object *obj = &gfx_state.objects[handle];

    SDL_Surface *surface = SDL_LoadBMP(path); if (surface == NULL) {
        return -1;
    }

    obj->type    = GFX_TEXTURE;
    obj->texture = SDL_CreateTextureFromSurface(gfx_state.rnd, surface);
    obj->scalex  = 1.0f;
    obj->scaley  = 1.0f;
    obj->data.x  = 0;
    obj->data.y  = 0;
    obj->data.w  = surface->w;
    obj->data.h  = surface->h;

    SDL_FreeSurface(surface);

    if (!obj->texture) {
        return -1;
    }

    return handle;
}

gfx_handle gfx_create_text(int font, int size, const char *text, vec4 color) {
    ZPL_ASSERT(gfx_state.rnd);

    int handle      = gfx_handle_next();
    gfx_object *obj = &gfx_state.objects[handle];

    TTF_Font *fontptr    = gfx_font_request(font, size);
    SDL_Color textcolor  = {(int)color.r, (int)color.g, (int)color.b, (int)color.a};
    SDL_Surface *surface = TTF_RenderUTF8_Blended(fontptr, text, textcolor);if (surface == NULL) {
        return -1;
    }

    obj->type    = GFX_TEXTURE;
    obj->texture = SDL_CreateTextureFromSurface(gfx_state.rnd, surface);
    obj->scalex  = 1.0f;
    obj->scaley  = 1.0f;
    obj->data.x  = 0;
    obj->data.y  = 0;
    obj->data.w  = surface->w;
    obj->data.h  = surface->h;
    obj->color   = color;

    SDL_FreeSurface(surface);

    if (!obj->texture) {
        return -1;
    }

    return handle;
}

gfx_handle gfx_create_line(int x1, int y1, int x2, int y2, vec4 color) {
    ZPL_ASSERT(gfx_state.rnd);

    int handle      = gfx_handle_next();
    gfx_object *obj = &gfx_state.objects[handle];

    obj->type    = GFX_LINE;
    obj->data.x1 = x1;
    obj->data.y1 = y1;
    obj->data.x2 = x2;
    obj->data.y2 = y2;
    obj->color   = color;

    return handle;
}

gfx_handle gfx_create_rect(int x, int y, int w, int h, vec4 color) {
    ZPL_ASSERT(gfx_state.rnd);

    int handle      = gfx_handle_next();
    gfx_object *obj = &gfx_state.objects[handle];

    obj->type    = GFX_RECTANGLE;
    obj->data.x  = x;
    obj->data.y  = y;
    obj->data.w  = w;
    obj->data.h  = h;
    obj->color   = color;

    return handle;
}

int gfx_destroy(gfx_handle handle) {
    if (!gfx_state.objects[handle].valid) {
        return -1;
    }

    gfx_object *obj = &gfx_state.objects[handle];

    switch (obj->type) {
        case GFX_LINE:
        case GFX_RECTANGLE:
            break;

        case GFX_TEXTURE:
            SDL_DestroyTexture(obj->texture);
            break;
    }

    zpl_array_count(gfx_state.objects)--;
    gfx_state.objects[handle].valid = 0;

    return 0;
}



void graphics_device_create(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
void graphics_device_prerender();
void graphics_device_render();
void graphics_device_lost(IDirect3DDevice9*);
void graphics_device_reset(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

SDL_Renderer* renderer = NULL;
SDL_Texture *tex;
SDL_Surface *bmp;
TTF_Font *font;
SDL_Rect rect1, rect2;

/*
- x, y: upper left corner.
- texture, rect: outputs.
*/
void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) {
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

IDirect3D9 *WINAPI gfx_d3dcreate9_hook(UINT SDKVersion) {
    IDirect3D9 *pD3D = gfx_state.method(SDKVersion);

    if (pD3D) {
        return new CDirect3D9Proxy(pD3D);
    }

    MessageBox(NULL, "Unable to create Direct3D9 interface.", "Fatal error", MB_ICONERROR);
    TerminateProcess(GetCurrentProcess(), 0);

    return NULL;
}

int gfx_init() {
    gfx_state = {0};

    if (gfx_state.installed) {
        return 1;
    }

    gfx_state.installed = true;
    gfx_state.method = (gfx_d3dcreate9_cb)(Mem::Hooks::InstallDetourPatch("d3d9.dll", "Direct3DCreate9", (DWORD)gfx_d3dcreate9_hook));

    TTF_Init();
    zpl_array_init_reserve(gfx_state.objects, zpl_heap(), 4);

    return 0;
}

int gfx_free() {
    if (!gfx_state.installed) {
        return 1;
    }

    gfx_state.installed = false;
    Mem::Hooks::UninstallDetourPatch(gfx_state.method, (DWORD)gfx_d3dcreate9_hook);

    zpl_array_free(gfx_state.objects);
    TTF_Quit();

    return 0;
}

void graphics_device_create(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters) {
    mod_log("[info] creating dx9 device [%x, %x] ...\n", pDevice, pPresentationParameters);

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

void gfx_util_screen2world(vec3 *screen, vec3 **world) {

}

void gfx_util_world2screen(vec3 *world, vec3 **screen) {

}
