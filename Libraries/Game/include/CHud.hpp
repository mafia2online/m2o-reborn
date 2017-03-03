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
	class ICGameGuiModule
	{
	public:
		void *m_pVFTable;
	};

	class C_GameGuiModule : public GameClassWrapper<C_GameGuiModule, 0x1AB64F0>
	{
	public:
		/*void _declspec(naked) FaderFadeIn(int un)
		{

		}*/

		void FaderFadeIn(float time)
		{
			static float dwReturn;
			Mem::InvokeFunction<Mem::call_this, void>(0x472300, this, &dwReturn, time, 0);
		}
		/*
		void FaderFadeOut(float time)
		{
			static uint32_t dwReturn;
			float test;
			Mem::InvokeFunction<Mem::call_this, void>(0x0890F10, this, &dwReturn, &test, time, 0, 0);
		}
		*/

		inline ICGameGuiModule* GetInterface() { return reinterpret_cast<ICGameGuiModule*>(this); }
	};
};