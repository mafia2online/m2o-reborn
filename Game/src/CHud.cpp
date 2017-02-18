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