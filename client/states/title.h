struct {
    char hostname_input[256];
    char port_input[6];

    int  hostname_len;
    int  port_len;
} title_state_data;

void title_state_init() {
    zpl_strcpy(title_state_data.hostname_input, "localhost");
    zpl_strcpy(title_state_data.port_input, "27010");
}

void title_state_render() {

    if (nk_begin(nk_ctx, "Mafia 2 Online: Reborn", nk_rect(10, 10, 210, 150), NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
        /* 2 columes */
        nk_layout_row_dynamic(nk_ctx, 30, 2);

        nk_label(nk_ctx, "Host:", NK_TEXT_LEFT);
        nk_edit_string_zero_terminated(nk_ctx, NK_EDIT_BOX, title_state_data.hostname_input, 256, nk_filter_default);

        nk_label(nk_ctx, "Port:", NK_TEXT_LEFT);
        nk_edit_string_zero_terminated(nk_ctx, NK_EDIT_FIELD, title_state_data.port_input, 6, nk_filter_default);

        /* 1 column */
        nk_layout_row_dynamic(nk_ctx, 30, 1);
        if (nk_button_label(nk_ctx, "Connect")) {
            mod_log("connecting to: %s:%d\n", title_state_data.hostname_input, atoi(title_state_data.port_input));
            librg_network_start(ctx, { atoi(title_state_data.port_input), title_state_data.hostname_input });
        }
    }
    nk_end(nk_ctx);
}

#define MOD_TITLE_STATE { title_state_init, nullptr, title_state_render }
