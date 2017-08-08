/** @file CModel.hpp
*  @brief Game's class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"
#include "CFrame.hpp"
#include "CSingleMesh.hpp"

namespace M2
{
	class ICModel : public C_Frame
	{
	public:
		pad(ICModel, pad0, 0xC);
		DWORD m_dwFlags;
		pad(ICModel, pad1, 0x14);
		C_SingleMesh* m_pSingleMesh;
	};

	class C_Model : public ICModel
	{
	public:
		void CloneHierarchy(C_Model *model)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x14EC8F0, this, model);
		}
	};
};