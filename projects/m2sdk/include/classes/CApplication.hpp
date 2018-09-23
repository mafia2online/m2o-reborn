/** @file CGuiGame.cpp
*  @brief Game's Main Class
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
    class ICApplication
    {
    public:
        void *m_pVFTable;               //0000 - 0004
        pad(ICApplication, pad0, 0x88); //0004 - 008C
        int m_gameState;                //008C - 0090
    };

    class C_Application : public GameClassWrapper<C_Application, ICApplication, 0x1AB2838>
    {
    public:

        void ContinueGame()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x450C10, this);
        }

        void Start(int unk)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x450A00, this, unk);
        }
    };
};
