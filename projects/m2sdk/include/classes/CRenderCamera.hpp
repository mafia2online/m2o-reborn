/** @file CRenderCamera.cpp
*  @brief Game's CRenderCamera wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
	class ICRenderCamera
	{
	public:
		pad(ICRenderCamera, pad0, 0x1E8);		// 0000 - 01E8
	};

	class C_RenderCamera : public ICRenderCamera
	{

	};
};
