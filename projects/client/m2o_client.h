#ifndef M2O_CLIENT_H
#define M2O_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

// =======================================================================//
// !
// ! General
// !
// =======================================================================//

    // mod_free()
    // mod_exit()
    void mod_install();

    void mod_connect(const char *host, int port);
    void mod_disconnect();
    bool mod_connected();
    void mod_nickname_set(const char *name);
    // mod_message_send()

    void mod_log(const char* format, ...);

    void platform_init();
    void platform_tick();
    void platform_free();

    void *platform_windowid();
    const char *platform_path();

// =======================================================================//
// !
// ! Graphics
// !
// =======================================================================//

    /**
     * Valid handle is an int value equal or bigger than 0
     * If value returned from a method is smaller than 0, it means it's an error code
     */
    typedef int gfx_handle;

    int gfx_init();
    int gfx_free();

    gfx_handle gfx_create_texture(int w, int h);
    gfx_handle gfx_create_texture_file(const char *path);
    gfx_handle gfx_create_text(int font, int size, const char *text, vec4 color);
    gfx_handle gfx_create_line(int x1, int y1, int x2, int y2, vec4 color);
    gfx_handle gfx_create_rect(int x, int y, int w, int h, vec4 color);

    int gfx_update_texture(gfx_handle handle, const void *pixels, int w, int h);

    int gfx_exists(gfx_handle handle);
    int gfx_destroy(gfx_handle handle);

    int gfx_font_add(int fontid, const char *filename);
    int gfx_font_exists(int fontid);
    int gfx_font_remove(int fontid);

    int gfx_render_add(gfx_handle handle, int zindex);
    int gfx_render_exists(gfx_handle handle);
    int gfx_render_remove(gfx_handle handle);
    int gfx_render_dump();

    int gfx_zindex_get(gfx_handle handle);
    int gfx_zindex_set(gfx_handle handle, int zindex);

    int gfx_position_set(gfx_handle handle, int x, int y);
    int gfx_position_get(gfx_handle handle, int *x, int *y);

    int gfx_scale_set(gfx_handle handle, float x, float y);
    int gfx_scale_get(gfx_handle handle, float *x, float *y);

    void gfx_util_screensize(int *w, int *h);
    void gfx_util_screen2world(vec3 *screen, vec3 *world);
    void gfx_util_world2screen(vec3 *world, vec3 *screen);

// =======================================================================//
// !
// ! CEF, ingame browser
// !
// =======================================================================//

    typedef int cef_handle;

    int cef_init();
    int cef_free();
    int cef_tick();

    cef_handle cef_browser_create(const char *url, int w, int h, int zindex);

    int cef_exists(cef_handle handle);

    int cef_browser_resize(cef_handle handle, int w, int h);
    int cef_browser_reload(cef_handle handle);
    int cef_browser_destroy(cef_handle handle);

    int cef_browser_show(cef_handle handle);
    int cef_browser_hide(cef_handle handle);

    int cef_zindex_get(cef_handle handle);
    int cef_zindex_set(cef_handle handle, int zindex);

    int cef_url_set(cef_handle handle, const char *url);
    int cef_url_get(cef_handle handle, char *url, int maxlen);

// =======================================================================//
// !
// ! Virtual file system
// !
// =======================================================================//

    void vfs_init();
    void vfs_free();
    void vfs_dump_all(bool value);
    void vfs_override_set(const char *src, const char *dst);
    char *vfs_override_get(const char *src);

// =======================================================================//
// !
// ! Key/Mouse input
// !
// =======================================================================//

    void input_init();
    void input_free();
    void input_mouse_position(int *x, int *y);
    void input_block_set(bool value);
    bool input_block_get();
    // input_mouse_button()
    bool input_key_down(unsigned int key);
    bool input_key_up();
    // input_joy()

    // game_vehicle_create()
    // game_vehicle_destroy()
    // game_ped_create()
    // game_ped_destroy()

#ifdef __cplusplus
}
#endif

#endif // M2O_CLIENT_H
