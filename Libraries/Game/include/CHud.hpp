#pragma once
#include "CommonHeaders.h"

/*
#include "CHud.h"

using namespace M2;

#define C_Hud_Ptr 0x1CBA618

C_Hud::C_Hud() : m_interface(*(ICHud**)C_Hud_Ptr)
{

}

void M2::C_Hud::FaderFadeIn(float time)
{
static uint32_t dwReturn;
float test;
Mem::InvokeFunction<Mem::call_this, void>(0x08910C0, &m_interface->m_pEnumerator, &dwReturn, &test, time, 0, 0);
}

void M2::C_Hud::FaderFadeOut(float time)
{
static uint32_t dwReturn;
float test;
Mem::InvokeFunction<Mem::call_this, void>(0x0890F10, &m_interface->m_pEnumerator, &dwReturn, &test, time, 0, 0);
}
*/

namespace M2
{
	class ICcEnumator
	{
	public:
		void *m_pVFTable;
	};

	class ICHud
	{
	public:
		void *m_pVFTable;
		ICcEnumator *m_pEnumerator; // 0004
	};

	class C_Hud : public GameClassWrapper<C_Hud, 0x1CBA618>
	{
	public:
		void FaderFadeIn(float time)
		{
			static uint32_t dwReturn;
			float test;
			Mem::InvokeFunction<Mem::call_this, void>(0x08910C0, this, &dwReturn, &test, time, 0, 0);
		}

		void FaderFadeOut(float time)
		{
			static uint32_t dwReturn;
			float test;
			Mem::InvokeFunction<Mem::call_this, void>(0x0890F10, this, &dwReturn, &test, time, 0, 0);
		}

		inline ICHud* GetInterface() { return reinterpret_cast<ICHud*>(this); }
	};
};