/** @file CMenu.hpp
*  @brief Game's menu class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

#include "CHashName.hpp"

namespace M2
{
    struct S_FadeParam
    {
        unsigned int a;
        unsigned int b;
        unsigned int c;
    };

    enum E_ScreenOperationType: int
    {
        SCREEN_OPERATION_TYPE_QUESTION = 2
    };

    class I_ScreenDaemon
    {

    };

    class ICMenu
    {
    public:
    };

    class C_Menu : public GameClassWrapper<C_Menu, ICMenu, 0x1CBAC28>
    {
    public:
        int ActivateScreen(C_HashName const *unk, C_HashName const *unk2, E_ScreenOperationType type, S_FadeParam const fade)
        {
           return Mem::InvokeFunction<Mem::call_this, int>(0x8D9760, this, unk, unk2, type, &fade);
        }

        I_ScreenDaemon *GetDaemon(C_HashName const *name)
        {
            static char idk;
            return Mem::InvokeFunction<Mem::call_this, I_ScreenDaemon*>(0x876110, this, &idk, &name);
        }
    };
};
