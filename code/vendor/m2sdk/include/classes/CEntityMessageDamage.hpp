/** @file CEntityMessageDamage.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
	class ICEntityMessageDamage
    {
	};

	class C_EntityMessageDamage : public ICEntityMessageDamage
	{
    public:
        double GetDamage()
        {
            return Mem::InvokeFunction<Mem::call_this, double>(0x1166AC0, this);
        }

        int GetDamageType()
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x1166A60, this);
        }

        Vector3& GetDir()
        {
            return Mem::InvokeFunction<Mem::call_this, Vector3&>(0x1166A80, this);
        }

        M2::C_Frame *GetFrame()
        {
            return Mem::InvokeFunction<Mem::call_this, M2::C_Frame*>(0x1166AF0, this);
        }

        char *GetNormal()
        {
            return Mem::InvokeFunction<Mem::call_this, char*>(0x1166A90, this);
        }

        DWORD GetOwnerGuid()
        {
            return Mem::InvokeFunction<Mem::call_this, DWORD>(0x1166AE0, this);
        }

        Vector3& GetPos()
        {
            return Mem::InvokeFunction<Mem::call_this, Vector3&>(0x1166A70, this);
        }

        double GetPower()
        {
            return Mem::InvokeFunction<Mem::call_this, double>(0x1166AA0, this);
        }

        int GetWeaponId()
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x1166AB0, this);
        }
	};
};
