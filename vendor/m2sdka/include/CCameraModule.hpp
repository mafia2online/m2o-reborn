/** @file CCameraModule.cpp
*  @brief Game's CameraModule wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

#include "CCamera.hpp"

namespace M2
{
	class ICCameraModuleModule
	{
	public:
		void *m_pVFTable;
		C_Camera *pCamera1;
		C_Camera *pCamera2;
	};

	class C_CameraModule : public GameClassWrapper<C_CameraModule, ICCameraModuleModule, 0x1ABCAAC>
	{
	public:
		C_Camera * GetCamera(int index)
		{
			return (Mem::InvokeFunction<Mem::call_this, C_Camera *>(0x10822C0, this, index));
		}
	};
};