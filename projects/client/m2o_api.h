#ifndef M2O_API_H
#define M2O_API_H

void gfx_init();
void gfx_free();
// gfx_text_draw()
// gfx_texture_create()
// gfx_texture_destroy()
// gfx_texture_draw()
void gfx_screen_size(int *x, int *y);
// gfx_world_to_screen()
// gfx_screen_to_world()

void vfs_init();
void vfs_free();
void vfs_dump_all(bool value);
void vfs_override_set(const char *src, const char *dst);
char *vfs_override_get(const char *src);

// cef_init()
// cef_free()
// cef_tick()
// cef_browser_create()
// cef_browser_destroy()
// cef_url_set()
// cef_url_get()

// mod_free()
// mod_exit()
void mod_install();
void mod_log(const char* format, ...);
void mod_connect(const char *host, int port);
void mod_disconnect();
bool mod_connected();
void mod_nickname_set(const char *name);
// mod_message_send()

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

void platform_init();
void platform_free();

#endif // M2O_API_H
