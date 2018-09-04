#include <d3d9.h>
#include <d3dx9.h>

#include "SDL.h"
#include "SDL_ttf.h"

typedef IDirect3D9 *(WINAPI *gfx_d3dcreate9_cb)(UINT);

struct gfx_object {
    u8 type;

    u8 valid : 1;
    u8 queued : 1;
    u8 metadata : 6;

    i32 zindex;

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
    char fontpaths[8][MAX_PATH];
    int  object_cursor;

    zpl_array(gfx_object) objects;
    zpl_array(gfx_handle) queue;

    std::unordered_map<int, TTF_Font *> fonts;
    SDL_Renderer *rnd;

    gfx_d3dcreate9_cb       method;
    IDirect3DDevice9        *device;
    D3DPRESENT_PARAMETERS   present_params;
};

enum {GFX_TEXTURE, GFX_RECTANGLE, GFX_LINE};

static gfx_t gfx_state;
static zpl_mutex gfx_lock;

#define GFX_MAX_FONTS 255

// =======================================================================//
// !
// ! Fonts
// !
// =======================================================================//

    int gfx_font_add(int fontid, const char *filename) {
        if (gfx_font_exists(fontid)) {
            return -1;
        }

        if (!zpl_file_exists(filename)) {
            return -2;
        }

        if (fontid > GFX_MAX_FONTS) {
            return -3;
        }

        zpl_memcopy(&gfx_state.fontpaths[fontid], filename, zpl_strlen(filename));
        return 0;
    }

    int gfx_font_exists(int fontid) {
        if (gfx_state.fontpaths[fontid][0] == 0) {
            return 0;
        }

        return 1;
    }

    int gfx_font_remove(int fontid) {
        if (!gfx_font_exists(fontid)) {
            return -1;
        }

        zpl_zero_item(gfx_state.fontpaths[fontid]);
        return 1;
    }

    TTF_Font *gfx_font_request(int fontid, int size) {
        if (!gfx_font_exists(fontid)) {
            return NULL;
        }

        if (fontid > GFX_MAX_FONTS) {
            return NULL;
        }

        int key = fontid + GFX_MAX_FONTS * size;

        if (gfx_state.fonts.find(key) == gfx_state.fonts.end()) {
            gfx_state.fonts[key] = TTF_OpenFont(gfx_state.fontpaths[fontid], size);
        }

        return gfx_state.fonts[key];
    }

// =======================================================================//
// !
// ! Resource creation/destruction
// !
// =======================================================================//

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
        zpl_mutex_lock(&gfx_lock);

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
            obj->valid = 0;
            zpl_array_count(gfx_state.objects)--;
            zpl_mutex_unlock(&gfx_lock);
            return -1;
        }

        zpl_mutex_unlock(&gfx_lock);
        return handle;
    }

    gfx_handle gfx_create_texture_file(const char *path) {
        ZPL_ASSERT(gfx_state.rnd);
        zpl_mutex_lock(&gfx_lock);

        int handle      = gfx_handle_next();
        gfx_object *obj = &gfx_state.objects[handle];

        SDL_Surface *surface = SDL_LoadBMP(path); if (surface == NULL) {
            zpl_mutex_unlock(&gfx_lock);
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
            obj->valid = 0;
            zpl_array_count(gfx_state.objects)--;
            zpl_mutex_unlock(&gfx_lock);
            return -1;
        }

        zpl_mutex_unlock(&gfx_lock);
        return handle;
    }

    gfx_handle gfx_create_text(int font, int size, const char *text, vec4 color) {
        ZPL_ASSERT(gfx_state.rnd);
        zpl_mutex_lock(&gfx_lock);

        int handle      = gfx_handle_next();
        gfx_object *obj = &gfx_state.objects[handle];

        TTF_Font *fontptr    = gfx_font_request(font, size);
        SDL_Color textcolor  = {(u8)color.r, (u8)color.g, (u8)color.b, (u8)color.a};
        SDL_Surface *surface = TTF_RenderUTF8_Blended(fontptr, text, textcolor);if (surface == NULL) {
            zpl_mutex_unlock(&gfx_lock);
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
            obj->valid = 0;
            zpl_array_count(gfx_state.objects)--;
            zpl_mutex_unlock(&gfx_lock);
            return -1;
        }

        zpl_mutex_unlock(&gfx_lock);
        return handle;
    }

    gfx_handle gfx_create_line(int x1, int y1, int x2, int y2, vec4 color) {
        zpl_mutex_lock(&gfx_lock);

        int handle      = gfx_handle_next();
        gfx_object *obj = &gfx_state.objects[handle];

        obj->type    = GFX_LINE;
        obj->data.x1 = x1;
        obj->data.y1 = y1;
        obj->data.x2 = x2;
        obj->data.y2 = y2;
        obj->color   = color;

        zpl_mutex_unlock(&gfx_lock);
        return handle;
    }

    gfx_handle gfx_create_rect(int x, int y, int w, int h, vec4 color) {
        zpl_mutex_lock(&gfx_lock);

        int handle      = gfx_handle_next();
        gfx_object *obj = &gfx_state.objects[handle];

        obj->type    = GFX_RECTANGLE;
        obj->data.x  = x;
        obj->data.y  = y;
        obj->data.w  = w;
        obj->data.h  = h;
        obj->color   = color;

        zpl_mutex_unlock(&gfx_lock);
        return handle;
    }

    int gfx_update_texture(gfx_handle handle, const void *pixels, int w, int h) {
        if (!gfx_exists(handle)) {
            return -1;
        }

        u8 *texture_data  = NULL;
        int texture_pitch = 0;

        zpl_mutex_lock(&gfx_lock);
        gfx_object *obj = &gfx_state.objects[handle];

        if (obj->type != GFX_TEXTURE) {
            zpl_mutex_unlock(&gfx_lock);
            return -2;
        }

        SDL_LockTexture(obj->texture, 0, (void **)&texture_data, &texture_pitch);
        memcpy(texture_data, pixels, w * h * 4);
        SDL_UnlockTexture(obj->texture);

        zpl_mutex_unlock(&gfx_lock);

        return 0;
    }

    int gfx_exists(gfx_handle handle) {
        zpl_mutex_lock(&gfx_lock);

        if (handle >= 0 && handle > zpl_array_capacity(gfx_state.objects)) {
            zpl_mutex_unlock(&gfx_lock);
            return 0;
        }

        int valid = gfx_state.objects[handle].valid;

        zpl_mutex_unlock(&gfx_lock);
        return valid;
    }

    int gfx_destroy(gfx_handle handle) {
        if (!gfx_exists(handle)) {
            return -1;
        }

        zpl_mutex_lock(&gfx_lock);
        gfx_object *obj = &gfx_state.objects[handle];
        int queued = obj->queued;
        zpl_mutex_unlock(&gfx_lock);

        if (queued) {
            gfx_render_remove(handle);
        }

        zpl_mutex_lock(&gfx_lock);
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

        zpl_mutex_unlock(&gfx_lock);
        return 0;
    }

// =======================================================================//
// !
// ! Rendering
// !
// =======================================================================//

    ZPL_COMPARE_PROC(gfx_render_sort_cmp) {
        int *ax = (int *)a;
        int *bx = (int *)b;

        if (*ax == -1) { return 1; }
        if (*bx == -1) { return 1; }

        int az = gfx_state.objects[*ax].zindex;
        int bz = gfx_state.objects[*bx].zindex;

        return az > bz ? 1 : bz < az ? 1 : 0;
    }

    int gfx_render_resort() {
        zpl_sort_array(gfx_state.queue, zpl_array_count(gfx_state.queue), gfx_render_sort_cmp);

        if (zpl_array_back(gfx_state.queue) == -1) {
            zpl_array_pop(gfx_state.queue);
        }

        return 0;
    }

    int gfx_render_add(gfx_handle handle, int zindex) {
        if (!gfx_exists(handle)) {
            return -1;
        }

        zpl_mutex_lock(&gfx_lock);
        gfx_object *obj = &gfx_state.objects[handle];

        obj->queued = true;
        obj->zindex = zindex;

        zpl_array_append(gfx_state.queue, handle);
        gfx_render_resort();
        zpl_mutex_unlock(&gfx_lock);

        return 0;
    }

    int gfx_render_exists(gfx_handle handle) {
        if (!gfx_exists(handle)) {
            return -1;
        }

        zpl_mutex_lock(&gfx_lock);
        int result = gfx_state.objects[handle].queued;
        zpl_mutex_unlock(&gfx_lock);

        return result;
    }

    int gfx_render_remove(gfx_handle handle) {
        if (!gfx_render_exists(handle)) {
            return -1;
        }

        zpl_mutex_lock(&gfx_lock);
        gfx_object *obj = &gfx_state.objects[handle];

        obj->queued = false;
        obj->zindex = 0;

        for (int i = 0; i < zpl_array_count(gfx_state.queue); ++i) {
            if (handle == gfx_state.queue[i]) {
                gfx_state.queue[i] = -1;
            }
        }

        gfx_render_resort();
        zpl_mutex_unlock(&gfx_lock);

        return 0;
    }

    int gfx_render_dump() {
        zpl_mutex_lock(&gfx_lock);

        zpl_printf("[ ");

        for (int i = 0; i < zpl_array_count(gfx_state.queue); ++i) {
            zpl_printf("%d, ", gfx_state.queue[i]);
        }

        zpl_printf(" ]\n");
        zpl_mutex_unlock(&gfx_lock);

        return 0;
    }

    int gfx_render_draw() {
        if (!gfx_state.rnd) {
            return -1;
        }

        SDL_SetRenderDrawBlendMode(gfx_state.rnd, SDL_BLENDMODE_BLEND);

        zpl_mutex_lock(&gfx_lock);
        for (int i = 0; i < zpl_array_count(gfx_state.queue); ++i) {
            gfx_object *obj = &gfx_state.objects[gfx_state.queue[i]];

            SDL_SetRenderDrawColor(
                gfx_state.rnd,
                (u8)obj->color.r,
                (u8)obj->color.g,
                (u8)obj->color.b,
                (u8)obj->color.a
            );

            switch (obj->type) {
                case GFX_LINE: {
                    SDL_RenderDrawLine(gfx_state.rnd, obj->data.x1, obj->data.y1, obj->data.x2, obj->data.y2);
                } break;
                case GFX_RECTANGLE: {
                    SDL_Rect desc = {obj->data.x, obj->data.y, obj->data.w, obj->data.h};
                    SDL_RenderFillRect(gfx_state.rnd, &desc);
                } break;

                case GFX_TEXTURE: {
                    SDL_Rect desc = {obj->data.x, obj->data.y, (int)(obj->data.w * obj->scalex), (int)(obj->data.h * obj->scaley)};
                    SDL_RenderCopy(gfx_state.rnd, obj->texture, NULL, &desc);
                } break;
            }

        }
        zpl_mutex_unlock(&gfx_lock);

        return 0;
    }

// =======================================================================//
// !
// ! Attributes
// !
// =======================================================================//

    int gfx_zindex_get(gfx_handle handle) {
        if (!gfx_exists(handle)) {
            return 0;
        }

        zpl_mutex_lock(&gfx_lock);
        int zindex = gfx_state.objects[handle].zindex;
        zpl_mutex_unlock(&gfx_lock);

        return zindex;
    }

    int gfx_zindex_set(gfx_handle handle, int zindex) {
        if (!gfx_exists(handle)) {
            return -1;
        }

        int queued = gfx_render_exists(handle);

        zpl_mutex_lock(&gfx_lock);
        gfx_state.objects[handle].zindex = zindex;
        if (queued) { gfx_render_resort(); }
        zpl_mutex_unlock(&gfx_lock);

        return 0;
    }


    int gfx_position_set(gfx_handle handle, int x, int y) {
        if (!gfx_exists(handle)) {
            return -1;
        }

        zpl_mutex_lock(&gfx_lock);
        gfx_state.objects[handle].data.x = x;
        gfx_state.objects[handle].data.y = y;
        zpl_mutex_unlock(&gfx_lock);

        return 0;
    }

    int gfx_position_get(gfx_handle handle, int *x, int *y) {
        if (!gfx_exists(handle)) {
            return -1;
        }

        zpl_mutex_lock(&gfx_lock);
        *x = gfx_state.objects[handle].data.x;
        *y = gfx_state.objects[handle].data.y;
        zpl_mutex_unlock(&gfx_lock);

        return 0;
    }

    int gfx_scale_set(gfx_handle handle, float x, float y) {
        if (!gfx_exists(handle)) {
            return -1;
        }

        zpl_mutex_lock(&gfx_lock);
        gfx_state.objects[handle].scalex = x;
        gfx_state.objects[handle].scaley = y;
        zpl_mutex_unlock(&gfx_lock);

        return 0;
    }

    int gfx_scale_get(gfx_handle handle, float *x, float *y) {
        if (!gfx_exists(handle)) {
            return -1;
        }

        zpl_mutex_lock(&gfx_lock);
        *x = gfx_state.objects[handle].scaley;
        *y = gfx_state.objects[handle].scaley;
        zpl_mutex_unlock(&gfx_lock);

        return 0;
    }

// =======================================================================//
// !
// ! Utils
// !
// =======================================================================//

    void gfx_util_screensize(int *w, int *h) {
        *w = static_cast<int>(gfx_state.present_params.BackBufferWidth);
        *h = static_cast<int>(gfx_state.present_params.BackBufferHeight);
    }

    void gfx_util_screen2world(const vec3 *screen, vec3 *world) {
        ZPL_ASSERT_MSG(false, "TODO: implement gfx_util_screen2world");
    }

    void gfx_util_world2screen(const vec3 *world, vec3 *screen) {
        // Get the world view projection matrix
        D3DXMATRIX mat = *(D3DXMATRIX *)M2::GetCameraWorldViewProjection();
        D3DXMatrixTranspose(&mat, &mat);

        // Get the viewport
        D3DVIEWPORT9 viewport;
        gfx_state.device->GetViewport(&viewport);

        // Transform the world coordinate by the worldViewProjection matrix
        D3DXVECTOR3 vec;
        D3DXVec3TransformCoord(&vec, &D3DXVECTOR3(world->x, world->y, world->z), &mat);

        screen->x = viewport.X + (1.0f + vec.x) * viewport.Width / 2.0f;
        screen->y = viewport.Y + (1.0f - vec.y) * viewport.Height / 2.0f;
        // screen->z = viewport.MinZ + vec.z * (viewport.MaxZ - viewport.MinZ);
        screen->z = world->z * mat._33 + world->y * mat._23 + world->x * mat._13 + mat._43;
    }

// =======================================================================//
// !
// ! Hooking
// !
// =======================================================================//

    #include "gfx/CDirect3DDevice9Proxy.h"
    #include "gfx/CDirect3D9Proxy.h"

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
        zpl_array_init(gfx_state.queue, zpl_heap());
        zpl_mutex_init(&gfx_lock);

        return 0;
    }

    int gfx_free() {
        if (!gfx_state.installed) {
            return 1;
        }

        gfx_state.installed = false;
        Mem::Hooks::UninstallDetourPatch(gfx_state.method, (DWORD)gfx_d3dcreate9_hook);

        for (const auto& item : gfx_state.fonts) {
            TTF_CloseFont(item.second);
        }

        zpl_mutex_destroy(&gfx_lock);
        zpl_array_free(gfx_state.queue);
        zpl_array_free(gfx_state.objects);
        TTF_Quit();

        return 0;
    }
