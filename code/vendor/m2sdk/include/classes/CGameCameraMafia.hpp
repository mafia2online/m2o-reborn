/** @file CCameraModule.cpp
*  @brief Game's GameCameraMafia wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CRenderCamera.hpp"

namespace M2
{
    class ICGameCameraMafia
    {
    public:
        pad(ICGameCameraMafia, pad0, 0x84);     // 0000 - 0084
        float m_fov;                            // 0084 - 0088
        float m_fNear;                          // 0088 - 008C
        float m_fFar;                           // 008C - 0090
        float m_fAspect;                        // 0090 - 0094
        pad(ICGameCameraMafia, pad1, 0x18);         // 0094 - 00AC
        float m_viewMatrix[12];                 // 00AC - 00DC
        Utils::Matrix34 m_projectionMatrix;          // 00DC - 011C
        Utils::Matrix34 m_worldViewProjection;       // 011C - 015C
    };

    class C_GameCameraMafia : public ICGameCameraMafia
    {
    public:
        int ScriptCameraLockLookAt(const char *unk1, const char *unk2, float unk3)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x0B3D900, this, unk1, unk2, unk3);
        }

        void ScriptCameraLockSetActorIntern(void *actor, float unk)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x0B3D730, this, actor, unk);
        }

        void ScriptSetCameraRotationTowards(Vector3 vec)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x0B55B90, this, vec);
        }
    };
};
