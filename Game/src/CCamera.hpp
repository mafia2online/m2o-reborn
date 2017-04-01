/** @file CCamera.cpp
*  @brief Game's CCamera wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

#include "CRenderCamera.hpp"

namespace M2
{
	class ICCamera
	{
		pad(ICCamera, pad0, 0x38);			// 0000 - 0038
		Matrix43 m_worldMatrix;				// 0038 - 0068
		pad(ICCamera, pad1, 0x4);			// 0068 - 006C
		C_RenderCamera* m_pRenderCamera;	// 006C - 0070
		pad(ICCamera, pad2, 0x14);			// 0070 - 0084
		float m_fUnknown1;					// 0084 - 0088
		float m_fNear;						// 0088 - 008C
		float m_fFar;						// 008C - 0090
		float m_fAspect;					// 0090 - 0094
		pad(ICCamera, pad3, 0x18);			// 0094 - 00AC
		Matrix43 m_viewMatrix;				// 00AC - 00DC
	};

	class C_Camera : public ICCamera
	{

	};
};