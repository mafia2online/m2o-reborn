// TODO: remove
void mod_disconnected(librg_event_t *);

// EXAMPLE: draw custom rectangle
// nk_fill_rect(nk_window_get_canvas(nk_ctx), nk_rect(0, 0, 100, 100), 10.0, nk_rgb(255,0,0));

// =======================================================================//
// !
// ! Debug handler
// !
// =======================================================================//

const int MOD_CONSOLE_HEIGHT = 280;
const int MOD_CONSOLE_BUFFER = 512;

static int mod_console_command_length;
static char mod_console_command_buffer[MOD_CONSOLE_BUFFER];

/**
 * Debug command handler
 * add your commands there
 */
void mod_debug_console_execute() {
    auto command_line = std::string(mod_console_command_buffer, mod_console_command_length);
    auto command = command_line;

    size_t pos = command_line.find(" ");
    if (pos != std::string::npos) {
        command = command_line.substr(0, pos);
    }

    if (command.compare("/q") == 0) {
        mod_exit("bye");
    }

    if (command.compare("/test") == 0) {
        mod_log("test :O\n");
    }

    if (command.compare("/spawn") == 0) {
        ((ped_t *)mod.player->user_data)->CEntity->SetPosition(vec3(-421.75f, 479.31f, 0.05f));
    }

#if _DEBUG
    // send vehicle create request onto server
    if (command.compare("/car") == 0) {
        librg_message_send(ctx, MOD_CAR_CREATE, nullptr, 0);
    }
#endif

    if (mod_console_command_buffer[0] != '/' && mod_console_command_length > 1) {
        mod_message_send(ctx, MOD_USER_MESSAGE, [&](librg_data_t *data) {
            librg_data_wu32(data, mod_console_command_length);
            librg_data_wptr(data, mod_console_command_buffer, mod_console_command_length);
        });
    }

    // unfocus edit and reset command
    mod_console_command_length = 0;
}

// =======================================================================//
// !
// ! Render code
// !
// =======================================================================//

void debug_console_render() {

    static struct nk_scroll log_scroll = { 0, 0 };
    struct nk_style_window original_window  = nk_ctx->style.window;
    struct nk_style_text original_text      = nk_ctx->style.text;

    bool enabling_console = false;
    int ysize = mod.console.enabled ? MOD_CONSOLE_HEIGHT : 32;

    int x, y;
    graphics_dimensions(&x, &y);
    nk_ctx->style.window.fixed_background = nk_style_item_color(nk_rgba(16, 16, 16, 100));

    if (nk_begin(nk_ctx, "console", nk_rect(0, y - ysize, x, ysize), NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_begin(nk_ctx, NK_DYNAMIC, 25, 2);
        {
            if (mod.console.enabled) {
                nk_layout_row_push(nk_ctx, 0.30f);
                nk_edit_string(nk_ctx, NK_EDIT_FIELD, mod_console_command_buffer, &mod_console_command_length, MOD_CONSOLE_BUFFER, nk_filter_default);

                nk_layout_row_push(nk_ctx, 0.10f);
                if (nk_button_label(nk_ctx, "Execute (Enter)")) {
                    mod_debug_console_execute();
                }
            } else {
                nk_layout_row_push(nk_ctx, 0.40f);
                auto pos = ((M2::C_Entity *)M2::C_Game::Get()->GetLocalPed())->GetPosition();

                nk_labelf(nk_ctx, NK_TEXT_LEFT, "position: %.3f %.3f %.3f",
                    pos.x, pos.y, pos.z
                );
            }

            /* empty space */
            nk_layout_row_push(nk_ctx, 0.46f);
            if (ctx->network.peer && librg_is_connected(ctx)) {
                nk_labelf(nk_ctx, NK_TEXT_CENTERED, "state: connected (%s:%s) | ping: %d ms | created entities: %d | streamed entities: %d",
                    title_state_data.hostname_input,
                    title_state_data.port_input,
                    ctx->network.peer->lastRoundTripTime,
                    ctx->entity.count,
                    mod.stats.streamed_entities
                );
            } else {
                nk_labelf(nk_ctx, NK_TEXT_RIGHT, "state: not connected");
            }

            nk_layout_row_push(nk_ctx, 0.07f);
            if (nk_button_label(nk_ctx, "Disconnect")) {
                librg_network_stop(ctx);
                mod_disconnected(nullptr);
            }

            nk_layout_row_push(nk_ctx, 0.06f);
            if (nk_button_label(nk_ctx, mod.console.enabled ? "Hide (~)" : "Show (~)")) {
                enabling_console = true;
            }
        }
        nk_layout_row_end(nk_ctx);

        if (mod.console.enabled) {
            nk_layout_row_dynamic(nk_ctx, 250, 1);
            nk_group_scrolled_begin(nk_ctx, &log_scroll, "title", NK_WINDOW_SCROLL_AUTO_HIDE);
            {
                nk_layout_row_dynamic(nk_ctx, 12, 1);
                for (u32 i = mod.console.queue.size(); i-- > 0; ) {
                    nk_label(nk_ctx, mod.console.queue._Get_container()[i].c_str(), NK_TEXT_LEFT);
                }
            }
            nk_group_scrolled_end(nk_ctx);
        }
    }

    if (enabling_console) {
        mod.console.enabled = !mod.console.enabled;
    }

    /* reset styles back */
    nk_ctx->style.window = original_window;
    nk_ctx->style.text = original_text;

    nk_end(nk_ctx);
}

#define MOD_USERNAME_DRAW_DISTANCE 250.0f
vec3_t last_player_position;

void draw_entity_nametag(librg_ctx_t *ctx, librg_entity_t *entity) {
    if (entity->type != TYPE_PED) return;
    if (entity->id == mod.player->id) return;

    auto ped = get_ped(entity);
    auto textlen = zpl_strlen(ped->name);

    if (textlen < 1) return;
    if (!ped->CHuman) return;
    if (zplm_vec3_mag2(last_player_position - entity->position) > MOD_USERNAME_DRAW_DISTANCE) return;

    vec3_t screen;
    graphics_world_to_screen(&screen, ped->CHuman->GetPos());
    if (screen.z > 1) return;

    auto fm   = (CFontManager *)mod.graphics.font_manager;
    auto font = fm->GetFont("Ingame");

    if (font) fm->DrawTextA(ped->name, screen.x - textlen * 4, screen.y, D3DCOLOR_XRGB(255, 255, 255), *font, true);
    //if (font)  fm->DrawTextW(ped->cached_name, screen.x, screen.y, D3DCOLOR_XRGB(179, 48, 48), *font, true);
}

void debug_state_render() {
    debug_console_render();

#if _DEBUG
    if (nk_begin(nk_ctx, "a great debug", nk_rect(2, 20, 200, 105), NK_WINDOW_TITLE | NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_NO_SCROLLBAR)) {
        /* fixed widget pixel width */
        nk_layout_row_dynamic(nk_ctx, 30, 1);

        if (nk_button_label(nk_ctx, "spawn ped")) {
            librg_message_send(ctx, MOD_PED_CREATE, nullptr, 0);
        }

        if (nk_button_label(nk_ctx, "spawn car")) {
            librg_message_send(ctx, MOD_CAR_CREATE, nullptr, 0);
        }
    }
    nk_end(nk_ctx);
#endif

    last_player_position = mod.player->position;
    librg_entity_iterate(ctx, LIBRG_ENTITY_ALIVE, draw_entity_nametag);
}

#define MOD_DEBUG_STATE { nullptr, nullptr, debug_state_render }
