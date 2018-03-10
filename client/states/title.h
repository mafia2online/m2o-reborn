struct {
    char hostname_input[256];
    char port_input[6];
    char username_input[64];

    int  hostname_len;
    int  port_len;
    int  username_len;
} title_state_data;

void title_state_init() {
    zpl_strcpy(title_state_data.hostname_input, "localhost");
    zpl_strcpy(title_state_data.port_input, "27010");
    zpl_strcpy(title_state_data.username_input, "Angry Mafioso");
}

static int mod_connecting = 0;

void title_state_render() {

    static int x, y;
    static int w = 210, h = 185;
    graphics_dimensions(&x, &y);

    if (nk_begin(nk_ctx, "", nk_rect(0, 0, x, y), NK_WINDOW_BACKGROUND)) {
        struct nk_command_buffer *canvas;
        struct nk_input *input = &nk_ctx->input;
        canvas = nk_window_get_canvas(nk_ctx);
        nk_fill_rect(canvas, nk_rect(0, 0, x, y), 0, nk_rgb(0, 0, 0));
    }
    nk_end(nk_ctx);

    if (nk_begin(nk_ctx, "Mafia 2 Online: Reborn", nk_rect(x / 2 - w / 2, y / 2 - h / 2, w, h), NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR)) {
        /* 2 columes */
        nk_layout_row_dynamic(nk_ctx, 30, 2);

        nk_label(nk_ctx, "Host:", NK_TEXT_LEFT);
        nk_edit_string_zero_terminated(nk_ctx, NK_EDIT_BOX, title_state_data.hostname_input, 256, nk_filter_default);

        nk_label(nk_ctx, "Port:", NK_TEXT_LEFT);
        nk_edit_string_zero_terminated(nk_ctx, NK_EDIT_FIELD, title_state_data.port_input, 6, nk_filter_default);

        nk_label(nk_ctx, "Username:", NK_TEXT_LEFT);
        nk_edit_string_zero_terminated(nk_ctx, NK_EDIT_FIELD, title_state_data.username_input, 64, nk_filter_default);

        /* 1 column */
        nk_layout_row_dynamic(nk_ctx, 30, 1);
        if (mod_connecting) {
            nk_button_label(nk_ctx, "Connectting...");
        } else if (nk_button_label(nk_ctx, "Connect")) {
            mod_connecting = 1;
            mod_log("[info] connecting to: %s:%d\n", title_state_data.hostname_input, atoi(title_state_data.port_input));
            librg_network_start(ctx, { atoi(title_state_data.port_input), title_state_data.hostname_input });
        }
    }
    nk_end(nk_ctx);
}

#define MOD_TITLE_STATE { title_state_init, nullptr, title_state_render }
