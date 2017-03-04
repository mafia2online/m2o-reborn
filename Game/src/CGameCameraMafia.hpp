/** @file CCameraModule.cpp
*  @brief Game's GameCameraMafia wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
	class ICGameCameraMafiaModule
	{
	public:
		void *m_pVFTable;						// 0000 - 0008
	};

	class C_GameCameraMafia : public ICGameCameraMafiaModule
	{
	public:
	};
};