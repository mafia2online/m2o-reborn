/** @file CTimer.hpp
*  @brief Game's CTimer wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CHudScriptW.h"

namespace M2
{
	class ICTimer
	{
	public:
		pad(ICTimer, pad0, 0x40);		//0000 - 0040
		void *m_pHudComponent;			//0040 - 0044
		pad(ICTimer, pad1, 0x4);		//0044 - 0048
		float m_fTime;					//0048 - 004C
		float m_fRemainingTime;			//004C - 0050
		pad(ICTimer, pad2, 0x2);		//0050 - 0052
		bool m_bIsPaused;				//0052 - 0054
		bool m_bIsRunning;				//0054 - 0055
		bool m_bIsStopped;				//0055 - 0056
	};

	class C_Timer : public ICTimer
	{
	public:
		void SetTime(float time)
		{
			m_fTime = time;
			m_fRemainingTime = time;
		}

		void Start()
		{
			m_bIsPaused = false;
			m_bIsRunning = true;
			m_bIsStopped = false;
		}

		void Stop()
		{
			m_bIsPaused = false;
			m_bIsRunning = false;
			m_bIsStopped = true;
		}
	};
};
