#pragma once
#include "CommonHeaders.h"

namespace M2
{
	class ICEnumator
	{
	public:
		void *m_pVFTable;
	};

	class ICHud
	{
	public:
		void *m_pVFTable;
		ICEnumator *m_pEnumerator; // 0004
	};

	class C_Hud : public ClassWrapper<C_Hud>
	{
	public:
		C_Hud();

		void FaderFadeIn(float time);
		void FaderFadeOut(float time);

		inline ICHud* GetInterface() { return m_interface; }
	private:
		ICHud* m_interface;
	};
};