#pragma once
#include "CommonHeaders.h"

namespace M2
{
	class ICNavigationModule
	{
	public:
		void *m_pVFTable;
	};

	class C_Navigation : public GameClassWrapper<C_Navigation, ICNavigationModule, 0x1AB6440>
	{
	public:
		bool HasMarkUser()
		{
			return Mem::InvokeFunction<Mem::call_this, bool>(0x4566F0, this);
		}

		void SetMarkUser(const Vector2& vecPosition)
		{
			Mem::InvokeFunction<Mem::call_this, int>(0x4E6A70, this, &vecPosition);
		}

		void DeleteMarkUser()
		{
			Mem::InvokeFunction<Mem::call_this, bool>(0x4E6B70, this);
		}
	};
};