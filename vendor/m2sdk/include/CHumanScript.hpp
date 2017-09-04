/** @file CHumanScript.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"
#include "CEntity.hpp"
#include "CSyncObject.hpp"

namespace M2
{
	enum ePhysState : BYTE
	{
		E_DYNAMIC = 0,
		E_ENABLED = 1,
		E_KINEMATIC = 2,
		E_DISABLED = 3
	};

    enum eHumanMoveMode
    {
        HUMAN_MOVE_MODE_NONE = -1,
        HUMAN_MOVE_MODE_BREATH = 0,
        HUMAN_MOVE_MODE_STEP = 1,
        HUMAN_MOVE_MODE_WALK = 2,
        HUMAN_MOVE_MODE_RUN = 3,
        HUMAN_MOVE_MODE_SPRINT = 4,
        HUMAN_MOVE_MODE_END = 5
    };

	class ICHumanScript
	{
	public:
		pad(ICHumanScript, pad0, 0xD0);		//0000 - 00D0
		float		m_fHealth;				//00D0 - 00D4
		float		m_fHealthMax;			//00D4 - 00D8

	};

	class C_HumanScript : public ICHumanScript
	{
	public:
		bool AreControlsLocked()
		{
			return Mem::InvokeFunction<Mem::call_this, bool>(0x091DE40, this);
		}

		void EnableBloodParticles(bool enable)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x091E410, this, enable);
		}

		void EnableBounceByPlayer(bool enable)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x093BB10, this, enable);
		}

		void EnableDragBody(bool enable)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x091E1E0, this, enable);
		}

		void EnableInjury(bool enable)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x091DC20, this, enable);
		}

		void EnableInjuryDiff(bool enable)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x091DC50, this, enable);
		}

		void EnablePushByPlayer(bool enable)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x093BB00, this, enable);
		}

		void EnableStealthKill(bool enable)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x093BA80, this, enable);
		}

		void ForceBloodPool(bool force)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x0D6F350, this, force);
		}

		ePhysState GetPhysState()
		{
			return Mem::InvokeFunction<Mem::call_this, ePhysState>(0x091AB80, this);
		}

		void InventoryArmed(bool armed)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x09954A0, this, armed);
		}

		void InventoryDrop()
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x0D6D490, this);
		}

		void InventoryHolster(bool holster)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x0995560, this, holster);
		}

		void InventorySelect(int weapon, bool animation, bool unk)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x09953A0, this, weapon, animation, unk);
		}

		void InventorySetUnlimitedAmmo(bool set)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x0924080, this, set);
		}

		bool IsCoverCrouch()
		{
			return Mem::InvokeFunction<Mem::call_this, bool>(0x091E190, this);
		}

		int IsInCover()
		{
			return Mem::InvokeFunction<Mem::call_this, int>(0x0D6E320, this);
		}

		bool IsModelVisible()
		{
			return Mem::InvokeFunction<Mem::call_this, bool>(0x0D6DDB0, this);
		}

		void LockControls(bool lock, bool forAnim)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x095CD80, this, lock, forAnim);
		}

		C_Entity *RegisterFollower(C_Entity *ent)
		{
			return Mem::InvokeFunction<Mem::call_this, C_Entity*>(0x08F7B40, this, ent);
		}

		void ScriptAnimEffectStop()
		{
			Mem::InvokeFunction<Mem::call_this, unsigned int>(0x0D6D0D0, this);
		}

		void ScriptAnimPlay(C_SyncObject **syncObject, const char *const animName, const bool repeat, int unk, int unk2, float unk3, float unk4, float unk5)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x0994630, this, syncObject, animName, repeat, unk, unk2, unk3, unk4, unk5);
		}

		void ScriptAnimPlayEffect(C_SyncObject **syncObject, const char *const effectName, const bool repeat, int unk)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x0962AD0, this, syncObject, effectName, repeat, unk);
		}

		void ScriptAnimStop()
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x0D6D040, this);
		}

		void SetAnimDeathTag(char const *tag)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x09221A0, this, tag);
		}

		void SetPhysState(ePhysState state)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x093B8A0, this, state);
		}

		void SetStealthMove(bool set)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x091E120, this, set);
		}

		void ShowHat(bool show)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x091DCC0, this, show);
		}

		void ShowModel(bool show)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x091DD30, this, show);
		}

		C_Entity *UnregisterFollower(C_Entity *ent)
		{
			return Mem::InvokeFunction<Mem::call_this, C_Entity*>(0x093B830, this, ent);
		}

        void UseAB(C_SyncObject **syncObject, M2::C_Vehicle *veh, bool bEnter, M2::E_VehicleSeat seat, int iAnimate)
        {
            int iEnter = (bEnter) ? 1 : 0;
            bool bAnimate = (int)!iAnimate;
            int iSeat = (int)seat;

            /* Are we exiting the vehicle ? */
            if (!bEnter) {
                iSeat = 0;
            }
            else {
                iSeat -= 1;
                iSeat = (iSeat < 0) ? 0 : iSeat;
            }

            Mem::InvokeFunction<Mem::call_this, int>(0x099E1E0, this, syncObject, veh, iEnter, iSeat, bAnimate, 0, 2);
        }

        /*C_SyncObject *ScrAimAt(C_SyncObject **syncObject, M2::C_Entity *ent, hmm_vec3 const &pos, const bool smooth)
        {
            return Mem::InvokeFunction<Mem::call_this, C_SyncObject *>(0x0993B00, this, syncObject, ent, pos, smooth);
        }

        C_SyncObject *ScrLookAt(C_SyncObject **syncObject, M2::C_Entity *ent, hmm_vec3 const &pos, const bool smooth)
        {
            return Mem::InvokeFunction<Mem::call_this, C_SyncObject *>(0x9931D0, this, syncObject, ent, pos, smooth);
        }

        C_SyncObject *ScrMoveV(C_SyncObject **syncObject, const hmm_vec3 &begin, const eHumanMoveMode moveMode, const hmm_vec3 &target, const bool smoothStop)
        {
            return Mem::InvokeFunction<Mem::call_this, C_SyncObject *>(0x9910B0, this, *syncObject, begin, moveMode, target, smoothStop);
        }

        C_SyncObject *ScrShootAt(C_SyncObject **syncObject, M2::C_Entity *ent, hmm_vec3 const &dir, const bool smooth)
        {
            return Mem::InvokeFunction<Mem::call_this, C_SyncObject *>(0x993C30, this, syncObject, ent, dir, smooth);
        }*/

        C_SyncObject *ScrAimAt(C_SyncObject **syncObject, M2::C_Entity *ent, vec3_t const &pos, const bool smooth);
        C_SyncObject *ScrLookAt(C_SyncObject **syncObject, M2::C_Entity *ent, vec3_t const &pos, const bool smooth);
        C_SyncObject *ScrMoveV(C_SyncObject **syncObject, const vec3_t &begin, const eHumanMoveMode moveMode, const vec3_t &target, const bool smoothStop);
        C_SyncObject *ScrShootAt(C_SyncObject **syncObject, M2::C_Entity *ent, vec3_t const &dir, const bool smooth);
	};

    /* TODO: Fix previous natives and delete those ones */
    DWORD dwAddress = 0x0993B00;
    C_SyncObject _declspec(naked) *C_HumanScript::ScrAimAt(C_SyncObject **syncObject, M2::C_Entity *ent, vec3_t const &pos, const bool smooth)
    {
        _asm jmp dwAddress
    }
    DWORD dwAddress2 = 0x9931D0;
    C_SyncObject _declspec(naked) *C_HumanScript::ScrLookAt(C_SyncObject **syncObject, M2::C_Entity *ent, vec3_t const &pos, const bool smooth)
    {
        _asm jmp dwAddress2;
    }
    DWORD dwAddress3 = 0x9910B0;
    C_SyncObject _declspec(naked) *C_HumanScript::ScrMoveV(C_SyncObject **syncObject, const vec3_t &begin, const eHumanMoveMode moveMode, const vec3_t &target, const bool smoothStop)
    {
        _asm jmp dwAddress3;
    }
    DWORD dwAddress4 = 0x993C30;
    C_SyncObject _declspec(naked) *C_HumanScript::ScrShootAt(C_SyncObject **syncObject, M2::C_Entity *ent, vec3_t const &dir, const bool smooth)
    {
        _asm jmp dwAddress4;
    }
};
