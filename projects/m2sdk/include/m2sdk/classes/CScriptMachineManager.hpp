/** @file CScriptMachineManager.hpp
*  @brief Game's script machine wrapper
*
*
*  @author Tyldar
*/

#pragma once
#include "CommonHeaders.h"
#include "CScriptMachine.hpp"

namespace M2
{
    class ICScriptMachineManager
    {
    public:
        pad(ICScriptMachineManager, pad0, 0x4);
        C_ScriptMachine *m_pScriptMachine; // 0004 - 0008
    };
    class C_ScriptMachineManager : public GameClassWrapper<C_ScriptMachineManager, ICScriptMachineManager, 0x1AC0C78>
    {
        C_ScriptMachine *GetScriptMachine(int machine)
        {
            return Mem::InvokeFunction<Mem::call_this, C_ScriptMachine*>(0x11D2300, this, machine);
        }
    };
};
