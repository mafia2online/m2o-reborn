/** @file CGameCamera.cpp
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
	class ICGameCamera
	{
	public:
		void *m_pVFTable;
		C_Camera *pCamera1;
		C_Camera *pCamera2;
	};

	class C_GameCamera : public GameClassWrapper<C_GameCamera, ICGameCamera, 0x1ABCAAC>
	{
	public:
        void CreateCamera(int cameraID, int cameraInitType, C_Camera *createdCamera)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x01083C00, this, cameraID, cameraInitType, createdCamera);
        }

		C_Camera * GetCamera(int index)
		{
			return (Mem::InvokeFunction<Mem::call_this, C_Camera *>(0x10822C0, this, index));
		}
	};
};
