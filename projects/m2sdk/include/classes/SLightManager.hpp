/** @file SLightManager.hpp
*  @brief Game's traffic lights class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
    class ISLightManager
    {
    public:
    };

    class S_LightManager : public ISLightManager
    {
    public:
        void ApplySemaphoreToAllRed(int unk)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x9BFFE0, this, unk);
        }

        void ApplySemaphoreToCurrent(int unk)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x9C0050, this, unk);
        }

        void ApplySemaphoreToOrange(int unk)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x9BFF70, this, unk);
        }

        // Should be C_LightEntity which inherit C_Entity
        C_Entity *CreateLight()
        {
            return Mem::InvokeFunction<Mem::call_this, C_Entity*>(0x9E82B0, this);
        }

        // Should be C_LightEntity which inherit C_Entity
        void ReleaseLight(C_Entity *light)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xA07050, this, light);
        }

        void SemaphoreToState(int unk, int unk2)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x9BFD30, this, unk, unk2);
        }
    };
};
