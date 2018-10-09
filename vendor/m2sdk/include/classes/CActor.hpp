/** @file CActor.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CEntityPos.hpp"

namespace M2
{
    class ICActor : public C_EntityPos
    {
    public:
    };

    class C_Actor : public ICActor
    {
    public:
        void SetPos(Vector3 pos)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x1164D40, this, &pos);
        }

        void SetRandomPosition()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x1164D90, this);
        }

        void SetRot(Quaternion rot)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x1080E90, this, &rot);
        }
    };
};
