#pragma once
#include "CommonHeaders.h"
#include "CWeatherManager.hpp"

namespace M2
{
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

        void CreateRainOrSnowBuffers()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x1345F00, this);
        }
	};
};
