/** @file CSDSModelManager.hpp
*  @brief Game's class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"
#include "CModel.hpp"
#include "CFrame.hpp"

namespace M2
{
	class ICSDSModelManager
	{
	public:
	};

	class C_SDSModelManager : public GameClassWrapper<C_SDSModelManager, ICSDSModelManager, 0x1CBD060>
	{
	public:
		C_Model *CreateModel(char const *file)
		{
			return Mem::InvokeFunction<Mem::call_this, C_Model*>(0x05C8090, this, file);
		}

		C_Model *CreateModel(int unk)
		{
			return Mem::InvokeFunction<Mem::call_this, C_Model*>(0x05C7FF0, this, unk);
		}

		void DestroyModel(C_Frame *frame)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x05AD500, this, frame);
		}
	};
};