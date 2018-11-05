#pragma once
#include <cstdint>
#include <Windows.h>

#include "T_TimerValType.h"

namespace ue::os {
  #pragma message(__FILE__ ": replace addreesses with patterns");

	class C_Timer {
		static int64_t GetTime() {
			auto countStart = *(LARGE_INTEGER *)0x23257B8;

			LARGE_INTEGER li;
			QueryPerformanceFrequency(&li);

			return countStart.QuadPart - li.QuadPart;
		}

		static int32_t GetTimeInMicroSeconds() {
			return nio::call<int32_t>(0x39A8C0);
		}

		static long double ConvertToSeconds(T_TimerValType<long long, false> const& time) {
			return nio::call<long double>(0x0039A950, time);
		}

		static long double ConvertToMiliSeconds(T_TimerValType<long long, false> const& time) {
			return nio::call<long double>(0x0039A990, time);
		} 

		static int32_t ConverToTimeVal(C_Timer* timer) {
			return nio::call<int32_t>(0x0039A9D0, timer);
		} 
	};
}
