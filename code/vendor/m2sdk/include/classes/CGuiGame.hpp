/** @file CGuiGame.cpp
*  @brief Game's Main Class
*
*  @author Tyldar (darreugne@gmail.com)
*  @author Vektor
*/

#pragma once

namespace M2
{
    enum E_iLoadType : int
    {
        TYPE_NEW_GAME = 0,
        TYPE_CONTINUE = 1,
        TYPE_UNK = 2,
        TYPE_RESTART = 3
    };

    class ICGuiGame
    {
    public:
        void *m_pVFTable;
    };

    class C_GuiGame : public GameClassWrapper<C_GuiGame, ICGuiGame, 0x18F6C08>
    {
    public:
        void StartGame(E_iLoadType loadType, int a3, int a4, char *missionName, unsigned int a6)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x476250, this, loadType, a3, a4, missionName, a6);
        }
    };
};
