/** @file CPlayerModelManager.hpp
*  @brief Game's model class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

#include "CModel.hpp"

namespace M2
{
	class ICPlayerModelManager
	{
	public:
		pad(ICPlayerModelManager, pad0, 0x14);
		C_Model *localPlayerModel;
	};


	class C_PlayerModelManager : public GameClassWrapper<C_PlayerModelManager, ICPlayerModelManager, 0x1ABFE5C>
	{
	public:
		void ChangeModel(const char *model, int unk)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x05CBE30, this, model, unk);
		}

		void ChangePlayerModelByShopID(int unk1, int unk2)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x05CC0F0, this, unk1, unk2);
		}

		void CreatePlayer()
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x05CD320, this);
		}

		void OpenModel(const char *model)
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x05CBBE0, this, model);
		}
	};
};