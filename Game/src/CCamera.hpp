/** @file CCamera.cpp
*  @brief Game's CCamera wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

#include "CRenderCamera.hpp"
#include "CGameCameraMafia.hpp"

namespace M2
{
	struct ShakeCommandData
	{
		float speed;
		float strength;
		float duration;
	};

	enum eCameraCommand
	{
		CAMCOMMAND_LOCKC = 1282368363, //Lock control
		CAMCOMMAND_GOVER = 1381191748, //Start game over sequence
		CAMCOMMAND_SHAKE = 1399349587, //Shake the screen
	};

	enum eCameraMode
	{
		CAMMODE_FPV = 121,
	};

	class ICCamera
	{
	public:
		void				*m_pVFTable;						// 0000 - 0008
		Matrix43			m_matWorld;							// 0008 - 0038
		Vector3				m_vecCamPos;						// 0038 - 0044
		Vector3				m_vecCamUp;							// 0044 - 0050
		pad(ICCamera, pad1, 0xC);		// 0050 - 005C
		float				m_fFovAngle;						// 005C - 0060
		pad(ICCamera, pad2, 0x8C);		// 0060 - 00EC
		int					m_iUnknown1;						// 00EC - 00F0
		pad(ICCamera, pad3, 0x34);		// 00F0 - 0124
		int					m_iWindowWidth;						// 0124 - 0128
		int					m_iWindowHeight;					// 0128 - 012C
		C_GameCameraMafia	*m_pCamera;							// 012C - 0130

	};

	class C_Camera : public ICCamera
	{
	public:
		int BroadcastCommand(eCameraCommand command, void *data, void *unknow)
		{
			return (Mem::InvokeFunction<Mem::call_this, int>(0x1082590, this, command, data, unknow));
		}

		int ModeChange(eCameraMode mode, void *actor, bool unk1, bool unk2)
		{
			return (Mem::InvokeFunction<Mem::call_this, int>(0x1084150, this, mode, actor, unk1, unk2));
		}

		int ModeGetActiveTypeTop()
		{
			return (Mem::InvokeFunction<Mem::call_this, int>(0x1082530, this));
		}

		int ModePop(int unk1, int unk2)
		{
			return (Mem::InvokeFunction<Mem::call_this, int>(0x1082B30, this, unk1, unk2));
		}
	};
};