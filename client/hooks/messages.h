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
            mod_log("Enter/Exit Vehicle done\n");
        }
        break;

        default:
        {
            mod_log("unknow event %d\n", (int)message->m_dwMessage);
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
            mod_log("Start to enter vehicle\n");
        }
        break;

        case E_PlayerMessage::MESSAGE_MOD_BREAKIN_CAR:
        {
            mod_log("Start to breakin vehicle\n");
        }
        break;

        case E_PlayerMessage::MESSAGE_MOD_LEAVE_CAR:
        {
            mod_log("Start to leave car\n");
        }
    }
}

/* When the player want to enter into vehicle */
/* Return true and the player will enter, false and the player will be stuck */
bool player_request_vehicle_enter(M2::C_Car *car)
{
    return true;
}
