#pragma once
#include <cstdint>

#include "T_TimerValType.h"

namespace ue::os {
  #pragma message(__FILE__ ": replace addreesses with patterns");
  
	class C_TimerFast {
		static uint64_t GetTime() {
		  return nio::call<uint64_t>(0x039AAB0);
		}

		static uint64_t GetTimeInMicroSeconds() {
		  return nio::call<int32_t>(0x39AAE0);
		}

		static long double ConvertToSeconds(T_TimerValType<unsigned long unsigned long, true> const& time) {
		  return nio::call<long double>(0x39AB30, time);
		}

		static long double ConvertToMilliSeconds(T_TimerValType<unsigned long unsigned long, true> const& time) {
		  return nio::call<long double>(0x39AB70, time);
		} 

		static long double ConverToMicroSeconds(T_TimerValType<unsigned long unsigned long, true> const& time) {
		  return nio::call<long double>(0x39ABB0, time);
		} 
	};
}
