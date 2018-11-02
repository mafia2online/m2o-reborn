/** @file SAgentCommandAim.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

#include "CEntityPos.hpp"

namespace M2
{
    class ISAgentCommandAim
    {

    };

    class S_AgentCommandAim : public ISAgentCommandAim
    {
    public:
        void DisableAimTemp()
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x18AC3A0, this);
        }

        void GetAimAngles(void *agent, float *x, float *y)
        {
            return Mem::InvokeFunction<Mem::call_this, void>(0x18AC3E0, agent, x, y);
        }

        bool IsEnabled()
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x18AC3D0, this);
        }

        unsigned int SetupAim(bool aim, C_EntityPos const* ent, zpl_vec3_t const &vec)
        {
            return Mem::InvokeFunction<Mem::call_this, unsigned int>(0x18AC320, this, aim, ent, vec);
        }

        unsigned int SetupAimDir(bool aim, zpl_vec3_t const &vec)
        {
            return Mem::InvokeFunction<Mem::call_this, unsigned int>(0x18AC360, this, aim, vec);
        }

        void SetTargetCallback(void *callback)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x18AC3B0, this, callback);
        }
    };
}
