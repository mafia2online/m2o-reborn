/** @file CScriptMachine.hpp
*  @brief Game's script machine wrapper
*
*
*  @author Tyldar
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
    class ScriptUnknow
    {
    public:
        pad(ScriptUnknow, pad0, 0x50);      // 0000 - 0050
        void    *m_pLuaState;               // 0050 - 0054
    };

    class ScriptUnknowPool
    {
    public:
        ScriptUnknow    *m_pUnknow[0xC];    // 0000 - 0004
    };

    class ICScriptMachine
    {
    public:
        pad(ICScriptMachine, pad0, 0x4);    // 0000 - 0004
        ScriptUnknowPool    *m_pScriptPool; // 0004 - 0008
    };

    class C_ScriptMachine : public ICScriptMachine
    {
    };
};
