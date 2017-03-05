/** @file CCameraModule.cpp
*  @brief Game's CameraModule wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

#include "CGameCameraMafia.hpp"

namespace M2
{
	class ICCameraModuleModule
	{
	public:
		void *m_pVFTable;
		C_GameCameraMafia *pCamera1;
		C_GameCameraMafia *pCamera2;
	};

	class C_CameraModule : public GameClassWrapper<C_CameraModule, ICCameraModuleModule, 0x1ABCAAC>
	{
	public:
		C_GameCameraMafia * GetCamera(int index)
		{
			return (Mem::InvokeFunction<Mem::call_this, C_GameCameraMafia *>(0x10822C0, this, index));
		}
	};
};