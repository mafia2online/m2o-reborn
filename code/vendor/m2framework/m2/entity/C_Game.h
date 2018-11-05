
#include <m2/I_Game.h>
#include <m2/entity/C_Player2.h>


class C_Game : public I_Game
{
    char pad[0x118 - 4];
    C_Player2 *local_player;

public:

    static C_Game *GetInstance()
    {
        return *(C_Game **)(0x01ABFE14);
    }

    inline C_Player2 *GetLocalPlayer()
    { 
        return local_player;
    }
};
