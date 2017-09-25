/** @file CNavigation.cpp
*  @brief Game's CNavigation wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
	class ICNavigationModule
	{
	public:
		void *m_pVFTable;
	};

	class C_Navigation : public GameClassWrapper<C_Navigation, ICNavigationModule, 0x1AB6440>
	{
	public:
        void DeleteMarkUser()
        {
            Mem::InvokeFunction<Mem::call_this, bool>(0x4E6B70, this);
        }

		bool HasMarkUser()
		{
			return Mem::InvokeFunction<Mem::call_this, bool>(0x4566F0, this);
		}

        int RegisterHuman(C_Human2 *human, bool unk)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x4E6D20, this, human, unk);
        }

        int RegisterHumanPolice(C_Human2 *human)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x4EE9B0, this, human);
        }

        int RegisterIconEntity(C_Entity *ent, int lib, int icon)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x4E7350, this, ent, lib, icon, 0, 0, 0);
        }

        int RegisterIconPos(const vec2_t pos, int library, int icon)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x4E72E0, this, &pos, library, icon, 0, 0);
        }

        int RegisterVehicleCommon(C_Car *vehicle)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x4E6B90, this, vehicle);
        }

        int RegisterVehiclePolice(C_Car *vehicle)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x4E6BD0, this, vehicle);
        }

        void ReloadMap()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x478AB0, this);
        }

        void SetEntityBackColor(int icon, int color)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x49F5B0, this, icon, color);
        }

        void SetEntityForeColor(int icon, int color)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x49F630, this, icon, color);
        }

		void SetMarkUser(const vec2_t &vecPosition)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x4E6A70, this, &vecPosition);
		}

        void UnregisterHuman(C_Human2 *human)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x4E6EE0, this, human, 0);
        }

        void UnregisterId(int icon)
        {
            Mem::InvokeFunction<Mem::call_this, bool>(0x4D7770, this, icon, 1);
        }

        void UnregisterVehicle(C_Car *car)
        {
            Mem::InvokeFunction<Mem::call_this, bool>(0x4D78A0, this, car, 0);
        }
	};
};
