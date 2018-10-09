/** @file SAgentCommandLookAt.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

#include "CEntityPos.hpp"

namespace M2
{
    enum E_Style : int
    {
        STYLE_UNK = 0
    };

    enum E_DiffEffect : int
    {
        EFFECT_UNK = 0
    };

    class ISAgentCommandLookAt
    {

    };

    class S_AgentCommandLookAt : public ISAgentCommandLookAt
    {
    public:
        bool Enable(bool enable)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x18AC700, this, enable);
        }

        bool IsEnabled()
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x18AC760, this);
        }

        void SetDiffEffect(void *agent, int effect)
        {
            Mem::InvokeFunction<Mem::call_cdecl, int>(0x18B4210, agent, effect);
        }

        void SetIntSpeed(int speed)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x18AC770, this, speed);
        }

        void SetStyle(E_Style style)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x18AC780, this, style);
        }

        void SetTarget(C_EntityPos const *ent, zpl_vec3_t const &vec)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x18AC710, this, ent, vec);
        }

        void SetTargetCallback(void *callback)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x18AC730, this, callback);
        }
    };
};
