/** @file CEntity.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
	class CEntityVFTable
	{
	public:
		DWORD Constructor;
		pad(CEntityVFTable, pad0, 0x78);
		DWORD SetPosition;
		DWORD SetDirection;
		DWORD SetRotation;
		DWORD m88;
		DWORD GetPosition;
		DWORD GetDirection;
		DWORD GetRotation;
	};
	class ICEntity
	{
	public:
		CEntityVFTable	* m_pVFTable;
	};
	class C_Entity : public ICEntity
	{
	public:

		void Activate()
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x1192170, this);
		}

		void Deactivate()
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x11921C0, this);
		}

		bool IsActive()
		{
			return (Mem::InvokeFunction<Mem::call_this, bool>(0x11665A0, this));
		}

		void Setup()
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x99F400, this);
		}
	};
};