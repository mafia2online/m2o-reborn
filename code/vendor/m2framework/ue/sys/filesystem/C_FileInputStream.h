#pragma once
#include <cstdint>

#include "../filesystem.h"

namespace ue::sys::filesystem {
#pragma message(__FILE__ ": replace addreesses with patterns");

	class C_FileInputStream {
	public:
		// ctor missing

		virtual ~C_FileInputStream() = 0;
		virtual uint32_t Read(void *, uint32_t, uint32_t *) = 0;
		virtual int32_t Skip(uint32_t, uint32_t *) = 0;
		virtual int32_t GetStatus() = 0;
		virtual int32_t Abort() = 0;
		virtual const char* DebugGetName() = 0;

		int32_t GetFileSize() {
			return nio::call<int32_t>(0xF4E3A0, this);
		}
		
		int32_t GetLastModificationTime() {
			return nio::call<int32_t>(0xF4E3D0, this);
		}

		int32_t AllowReadBehindEOF(bool allow) {
			return nio::call<int32_t>(0xF4E420, allow);
		}

		void CreateCache(uint32_t size) {
			return nio::call(0xF4DBF0, size);
		}
	};
}
