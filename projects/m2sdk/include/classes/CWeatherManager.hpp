#pragma once
#include "CScene.hpp"

namespace M2
{
    class ICWeatherManager
    {
    public:
        pad(ICWeatherManager, pad1, 0x34);  // 0000 - 0034
        long double     m_dTime;            // 0034 - 0044
    };
    class C_WeatherManager : public ICWeatherManager
    {
    public:

        long double GetTime()
        {
            return Mem::InvokeFunction<Mem::call_this, long double>(0x132CCB0, this);
        }

        bool IsCurrentlyBadWeather()
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x13563D0, this);
        }

        bool IsNightTime()
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x1356320, this);
        }

        bool IsWinterTime()
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x1356390, this);
        }

        void SetDayTemplate(const char *pszName)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x13869D0, this, &pszName);
        }

        void SetTime(float time)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x1369390, this, time);
        }

        void ResetAllClients()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x13817C0, this);
        }
    };
}
