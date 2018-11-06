#pragma once
#include <cstdint>

#include "../filesystem.h"

namespace ue::sys::filesystem {

	class C_FileInputChunk {
	public:
		virtual ~C_FileInputChunk() = 0;
		virtual int32_t TellImpl() = 0;
		virtual int32_t SeekImpl(int32_t, E_SeekTypes) = 0;
		virtual bool ReadImpl(void *, uint32_t) = 0;
	};
}
