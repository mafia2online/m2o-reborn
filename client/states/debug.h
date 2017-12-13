void mod_disconnected(librg_event_t *);

void debug_state_render()
{
    enum { EASY, HARD };
    static int op = EASY;
    static float value = 0.6f;
    static int i = 20;
    static int field_len;
    static char field_buffer[64];
    static int active;

    if (nk_begin(nk_ctx, "Show Привет", nk_rect(50, 50, 400, 420),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE)) {
        /* fixed widget pixel width */
        nk_layout_row_static(nk_ctx, 30, 80, 1);
        if (nk_button_label(nk_ctx, "disconnect")) {
            librg_network_stop(ctx);
            mod_disconnected(nullptr);
        }

        if (nk_button_label(nk_ctx, "spawn car")) {
            // send vehicle create request onto server
            librg_message_send(ctx, MOD_CAR_CREATE, nullptr, 0);
        }
        if (nk_button_label(nk_ctx, "disable cursor")) {
            mod.input_blocked = 0;
        }
        if (nk_button_label(nk_ctx, "teleport spawn")) {
            ((ped_t *)mod.player->user_data)->object->SetPosition(vec3(-421.75f, 479.31f, 0.05f));
        }


        char b1[16];
        char b2[16];
        sprintf(b1, "%f", test_pos_t);
        sprintf(b2, "%f", test_rot_t);

        nk_layout_row_dynamic(nk_ctx, 30, 2);
        nk_button_label(nk_ctx, b1);
        nk_slider_float(nk_ctx, 0, &test_pos_t, 0.3f, 0.0001f);

        nk_button_label(nk_ctx, b2);
        nk_slider_float(nk_ctx, 0, &test_rot_t, 0.3f, 0.0001f);

        char pos[256];
        auto cpos = ((M2::C_Entity *)M2::C_Game::Get()->GetLocalPed())->GetPosition();
        sprintf(pos, "%.2f %.2f %.2f", cpos.x, cpos.y, cpos.z);
        nk_label(nk_ctx, pos, NK_TEXT_LEFT);

        /* fixed widget window ratio width */
        nk_layout_row_dynamic(nk_ctx, 30, 2);
        if (nk_option_label(nk_ctx, "easy", op == EASY)) op = EASY;
        if (nk_option_label(nk_ctx, "hard", op == HARD)) op = HARD;

        /* custom widget pixel width */
        nk_layout_row_begin(nk_ctx, NK_STATIC, 30, 2);
        {
            nk_layout_row_push(nk_ctx, 50);
            nk_edit_string(nk_ctx, NK_EDIT_FIELD, field_buffer, &field_len, 64, nk_filter_default);

            nk_layout_row_push(nk_ctx, 110);
            nk_slider_float(nk_ctx, 0, &value, 1.0f, 0.1f);
        }
        nk_layout_row_end(nk_ctx);

        nk_layout_row_dynamic(nk_ctx, 30, 2);
        active = nk_check_label(nk_ctx, "AAAAAa", active);
    }
    nk_end(nk_ctx);
}

#define MOD_DEBUG_STATE { nullptr, nullptr, debug_state_render }
