#pragma once
#include <cstdint>

#include "../filesystem.h"

namespace ue::sys::filesystem {
	class C_FileOutputStream {
	public:
		// ctor missing

		virtual ~C_FileOutputStream() = 0;
		virtual uint32_t Write(void const*, uint32_t, uint32_t *) = 0;
		virtual bool GetStatus() = 0;
		virtual const char* DebugGetName() = 0;
		virtual int32_t Seek(int32_t, E_SeekTypes) = 0;
		virtual int32_t Tell(uint32_t*) = 0;
	};
}
