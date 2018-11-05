#pragma once
#include <cstdint>

#include "../filesystem.h"

namespace ue::sys::filesystem {
	class C_File { // size 12 (0xC)
	public:
		// vtable 0-3
		uint32_t unk1; // 4-7
		uint16_t unk2; // 8-12

	public:
		// no virtual deconstructor in vtable

		/*0*/  virtual int32_t Read(void *, uint32_t, uint32_t) = 0;
		/*4*/  virtual int32_t Write(void const*, uint32_t, uint32_t ) = 0;
		/*8*/  virtual int32_t Seek(long long, ue::sys::filesystem::E_SeekTypes) = 0;
		/*12*/ virtual int32_t Tell() = 0;
		/*16*/ virtual bool Eof() = 0;
		/*20*/ virtual bool Flush() = 0;
		/*24*/ virtual int32_t GetFileSize() = 0;
		/*28*/ virtual int32_t Close() = 0;
		/*32*/ virtual void nullsub_0() = 0;
		/*36*/ virtual void nullsub_1() = 0;
		/*40*/ virtual bool SetBuffer(uint32_t) = 0;
		/*44*/ virtual void nullsub_2() = 0;
		/*48*/ virtual void nullsub_3() = 0;
		/*52*/ virtual void nullsub_4() = 0;
		/*56*/ virtual void nullsub_5() = 0;
		/*60*/ virtual void nullsub_6() = 0;
	};
}
