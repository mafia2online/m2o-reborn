void debug_state_render(void * usrptr)
{
    enum { EASY, HARD };
    static int op = EASY;
    static float value = 0.6f;
    static int i = 20;

    if (nk_begin(nk_ctx, "Show Привет", nk_rect(50, 50, 220, 220),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE)) {
        /* fixed widget pixel width */
        nk_layout_row_static(nk_ctx, 30, 80, 1);
        if (nk_button_label(nk_ctx, "button")) {
            /* event handling */
        }

        /* fixed widget window ratio width */
        nk_layout_row_dynamic(nk_ctx, 30, 2);
        if (nk_option_label(nk_ctx, "easy", op == EASY)) op = EASY;
        if (nk_option_label(nk_ctx, "hard", op == HARD)) op = HARD;

        /* custom widget pixel width */
        nk_layout_row_begin(nk_ctx, NK_STATIC, 30, 2);
        {
            nk_layout_row_push(nk_ctx, 50);

            POINT pica;
            GetCursorPos(&pica);
            ScreenToClient(mod.window, &pica);

            char cipa[64] = { 0 };
            sprintf(cipa, "X: %d, Y: %d", pica.x, pica.y);

            nk_label(nk_ctx, cipa, NK_TEXT_LEFT);
            nk_layout_row_push(nk_ctx, 110);
            nk_slider_float(nk_ctx, 0, &value, 1.0f, 0.1f);
        }
        nk_layout_row_end(nk_ctx);
    }
    nk_end(nk_ctx);
}

#define MOD_DEBUG_STATE { nullptr, debug_state_render, nullptr }