#pragma once
#include <cstdint>

#include <Common.h>
#include <ue/sys/filesystem.h>

#include "C_VirtualFileSystem.h"

namespace ue::sys::filesystem {
	class C_VirtualFileSystemCache: public C_VirtualFileSystem { // class size 240 bytes
    private:
        char pad[76]; // 224 bytes alloc - 148 bytes base class (C_VirtualFileSystem)

	public:
		// ctor missing
		/*0*/   virtual ~C_VirtualFileSystemCache() = default;

        static C_VirtualFileSystemCache * GetInstance()
        {
            return *(C_VirtualFileSystemCache**)(0x1BDC918);
        }
	};
}
