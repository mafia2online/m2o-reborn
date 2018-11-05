#pragma once
#include <Common.h>

#define FILESYSTEM_VERBOSE /*_DEBUG &&*/ 1

namespace ue::sys::filesystem {
    enum class E_SeekTypes : uint32_t {
        UNKNOWN = 0
    };

    enum class E_VFS_VerboseMode : uint32_t {
        UNKNOWN = 0
    };

	class S_Init;
	class I_File;
	class I_VirtualFileSystem;
}


#include "filesystem/C_File.h"

#include "filesystem/C_VirtualFileSystem.h"
#include "filesystem/C_VirtualFileSystemCache.h"
