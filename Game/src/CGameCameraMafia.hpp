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

	class ICGameCameraMafiaModule
	{
	public:
		void *m_pVFTable;						// 0000 - 0008
	};

	class C_GameCameraMafia : public ICGameCameraMafiaModule
	{
	public:
		int BroadcastCommand(eCameraCommand command, void *data, void *unknow)
		{
			return (Mem::InvokeFunction<Mem::call_this, int>(0x1082590, this, command, data, unknow));
		}
	};
};