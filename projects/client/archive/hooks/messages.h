#pragma once

enum E_PlayerMessage
{
    MESSAGE_MOD_ENTER_CAR,
    MESSAGE_MOD_LEAVE_CAR,
    MESSAGE_MOD_BREAKIN_CAR
};

/* Messages from the game that we catched */
void player_game_message(M2::C_EntityMessage *message)
{
    switch (message->m_dwMessage)
    {
        case M2::E_HumanMessage::MESSAGE_GAME_ENTER_EXIT_VEHICLE_DONE:
        {
            mod_log("[game-event] Enter/Exit Vehicle done\n");

            librg_event_t event = { 0 }; event.entity = mod.player;
            librg_event_trigger(ctx, MOD_CAR_INTERACTION_FINISH, &event);
        }
        break;

        default:
        {
            mod_log("[game-event] unknow event %d\n", (int)message->m_dwMessage);
        }
        break;
    }
}

/* Messages from the mod own hooking */
void player_mod_message(E_PlayerMessage message)
{
    switch (message)
    {
        case E_PlayerMessage::MESSAGE_MOD_ENTER_CAR:
        {
            mod_log("[game-event] ped start to enter vehicle\n");
        }
        break;

        case E_PlayerMessage::MESSAGE_MOD_BREAKIN_CAR:
        {
            mod_log("[game-event] Start to breakin vehicle\n");
        }
        break;

        case E_PlayerMessage::MESSAGE_MOD_LEAVE_CAR:
        {
            mod_log("[game-event] Start to leave car\n");

            librg_event_t event = { 0 }; event.entity = mod.player;
            librg_event_trigger(ctx, MOD_CAR_EXIT_START, &event);
        }
    }
}

/* When the player want to enter into vehicle */
/* Return true and the player will enter, false and the player will be stuck */
bool player_request_vehicle_enter(M2::C_Car *car)
{
    mod_log("[game-event] ped request vehicle enter\n");

    return true;
}

void game_player_enter_car(M2::C_Player2 *player, M2::C_Actor *car, u8 seat) {
    mod_log("[game-event] ped entering the car on seat: %d\n", seat);

    librg_event_t event = { 0 };
    u8 iseat = seat + 1;

    event.entity = mod.player;
    event.user_data = (void *)car;
    event.data = (librg_data_t*)&iseat;

    librg_event_trigger(ctx, MOD_CAR_ENTER_START, &event);
}
