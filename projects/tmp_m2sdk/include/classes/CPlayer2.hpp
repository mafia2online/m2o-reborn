/** @file CPlayer2.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CEntity.hpp"
#include "CHuman2.hpp"

namespace M2
{
    enum E_PlayerMovementType : BYTE
    {
        E_MOVEMENT_WALK = 0,
        E_MOVEMENT_JOG = 1,
        E_MOVEMENT_SPRINT = 2,
        E_MOVEMENT_IDLE = 3,
        E_MOVEMENT_STOPPING = 5
    };

    class S_InputCtrl
    {
    public:
        pad(S_InputCtrl, pad0, 0xA0);                           // 0000 - 00A0
        E_PlayerMovementType    m_ePlayerMovementState;         // 00A0 - 00A1
        pad(S_InputCtrl, pad1, 0x5F);                           // 00A1 - 0100
        bool                    m_bCrouching;                   // 0100 - 0104

    };

	class ICPlayer2
	{
	public:
        pad(ICPlayer2, pad0, 0x240);            // 0000 - 0240
        S_InputCtrl    m_pPlayerControls;       // 0240 - 0244
	};

	class C_Player2 : public ICPlayer2
	{
	public:
		void LockControls(bool lock)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x438230, this, lock);
		}

        void UpdateMoveCmd(S_HumanCommandMoveDir *command, S_InputCtrl &input, Vector3 const &pos, Vector2 &dir, float unk)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x4268C0, this, command, &input, &pos, &dir, unk);
        }
	};
};
