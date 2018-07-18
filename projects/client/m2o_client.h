#ifndef M2O_CLIENT_H
#define M2O_CLIENT_H

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
    void platform_free();

// =======================================================================//
// !
// ! Graphics
// !
// =======================================================================//

    typedef int gfx_handle;

    int gfx_init();
    int gfx_free();

    gfx_handle gfx_create_texture(int w, int h);
    gfx_handle gfx_create_texture_file(const char *path);
    gfx_handle gfx_create_text(int font, int size, const char *text);
    gfx_handle gfx_create_line(int x1, int y1, int x2, int y2);
    gfx_handle gfx_create_rect(int x, int y, int w, int h, bool filled);

    int gfx_destroy(gfx_handle res);

    int gfx_render_add(gfx_handle res, int zindex);
    int gfx_render_exists(gfx_handle res);
    int gfx_render_remove(gfx_handle res);

    int gfx_zindex_get(gfx_handle res);
    int gfx_zindex_set(gfx_handle res, int zindex);

    int gfx_position_set(gfx_handle res, int x, int y);
    int gfx_position_get(gfx_handle res, int *x, int *y);

    void gfx_util_screensize(int *w, int *h);
    void gfx_util_screen2world(vec3 *screen, vec3 **world);
    void gfx_util_world2screen(vec3 *world, vec3 **screen);

// =======================================================================//
// !
// ! CEF, ingame browser
// !
// =======================================================================//

    // cef_init()
    // cef_free()
    // cef_tick()
    // cef_browser_create()
    // cef_browser_destroy()
    // cef_url_set()
    // cef_url_get()

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

#endif // M2O_CLIENT_H
