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
		pad(ICCamera, pad0, 0x12C);								// 0000 - 012C
		C_GameCameraMafia	*m_pGameCamera;						// 012C - 0130

	};

	class C_Camera : public ICCamera
	{
	public:
		int BroadcastCommand(eCameraCommand command, void *data, void *unknow)
		{
			return (Mem::InvokeFunction<Mem::call_this, int>(0x1082590, this, command, data, unknow));
		}

		void EnableBlendMode(int mode, bool enable)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x1083060, this, mode, enable);
		}

		long double GetFOV()
		{
			return this->m_pGameCamera->m_fov * 57.29577791868205;
		}

		float *MapScreenTarget(float *x, float *y)
		{
			return Mem::InvokeFunction<Mem::call_this, float *>(0x107FB30, this, x, y);
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

		void SetRotation(float x, float y, bool unk)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x1083010, x, y, unk);
		}
	};
};