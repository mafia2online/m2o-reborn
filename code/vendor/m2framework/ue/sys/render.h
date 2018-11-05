#pragma once
#include <Common.h>

#define FILESYSTEM_VERBOSE /*_DEBUG &&*/ 1

namespace ue::sys::render {
    enum class E_SeekTypes : int32_t {
        UNKNOWN = 0
    };
}

#include "core/C_Texture.h"
#include "core/C_TextureManager.h"

#include "render/C_TextureDesc.h"
