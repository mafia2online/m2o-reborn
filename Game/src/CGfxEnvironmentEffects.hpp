#pragma once
#include "CommonHeaders.h"

namespace M2
{
	/// todo: move me later - myu
	class C_WeatherManager
	{
	public:
		void SetDayTemplate(const char *pszName)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x13869D0, this, &pszName);
		}
		void SetTime(float time)
		{
			Mem::InvokeFunction<Mem::call_this, void>(0x1369390, this, time);
		}
		
	};

	/// todo: do vtbl stuff and so blablal - myu
	class ICGfxEnvironmentEffect
	{
	public:
		void *m_pVFTable; // 0-4
		pad(ICGfxEnvironmentEffect, pad0, 12);
		C_WeatherManager *pWeatherManager; // 16-20
	};

	class C_GfxEnvironmentEffects : public GameClassWrapper<C_GfxEnvironmentEffects, ICGfxEnvironmentEffect, 0x1AC1A94>
	{
	public:
		C_WeatherManager *GetWeatherManager() { return GetInterface()->pWeatherManager; }
	};
};