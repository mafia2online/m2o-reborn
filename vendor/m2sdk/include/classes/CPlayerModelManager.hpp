/** @file CPlayerModelManager.hpp
*  @brief Game's model class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CSlot.hpp"
#include "CModel.hpp"

namespace M2
{
	class ICPlayerModelManager
	{
	public:
        DWORD       m_pVFTable;                 // 0000 - 0004
        C_Slot      *m_pSlot;                   // 0004 - 0008
        int         m_iRefCount;                // 0008 - 000C
        pad(ICPlayerModelManager, pad0, 0x4);   // 000C - 0010
        C_Entity    *m_pEntity;                 // 0010 - 0014
        C_Model     *m_pModel;                  // 0014 - 0018
        void        *m_pUnknown;                // 0018 - 001C
        pad(ICPlayerModelManager, pad1, 0x10);  // 001C - 002C
        char        m_sModelName[32];           // 002C - 004C
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
