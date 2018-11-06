#pragma once
#include <ue/I_BaseObject.h>
#include <Common.h>
#include <ue/sys/render.h>

namespace ue::sys::core
{
	class C_Texture { // class size 64 bytes
	public:
    ue::sys::core::C_TextureManager* textureManager; //4-7
    uint32_t unk_1; //8-11
    uint32_t unk_2 = 8; //12-15
    uint32_t unk_3 = 0; //16-19
    uint32_t unk_4 = 0; //20-23
    uint32_t unk_5 = 0; //24-27
    uint32_t unk_6 = 0; //28-31
    uint32_t unk_7 = 0; //32-35
    uint32_t unk_8 = 0; //36-39
    uint8_t unk_9 = 0; //40
    char pad_0[3]; // align for byte
    uint32_t unk_10 = 0; //44-47
    uint32_t unk_11 = 1; //48-51
    ue::sys::render::device::I_VRAMAllocator* vramAllocator; //52-55
    uint32_t unk_12 = 0; //56-59
    uint32_t unk_13 = 0; //60-63

	public:
    C_Texture(ue::sys::render::device::I_VRAMAllocator* vramAllocator) {
      ::vramAllocator = vramAllocator;
    }

		virtual ~C_Texture() = default;
		virtual uintptr_t GetSyncManager() = 0;
		virtual uintptr_t GetSyncTarget() = 0;
		virtual int32_t Synchronize() = 0;
		virtual uint32_t MarkObjectForSync(uint32_t) = 0;
    virtual void nullsub_0() = 0;
    virtual void nullsub_1() = 0;
    virtual void nullsub_2() = 0;
    virtual void nullsub_3() = 0;

		static C_Texture* CreateTexture(ue::sys::render::device::I_VRAMAllocator* vramAllocator) {
      return new C_Texture(vramAllocator);
		}

    uint64_t* Create(ue::sys::utils::C_HashName const*, ue::sys::render::C_TextureDesc const&, uint8_t) {

    }

    int32_t Load(int, ue::sys::utils::C_HashName *, uint64_t *, int32_t, uint8_t, int32_t, int32_t) {

    }

    int32_t Reload(ue::I_InputStream &, ue::sys::render::E_TextureFileFormat) {

    }

    ue::sys::render::device::I_VRAMAllocator *Unload(ue::sys::core::C_Texture *this) {

    }

    int32_t ue::sys::core::C_Texture::Write2D(int, int, int, int, unsigned int, unsigned int, void *, ue::sys::render::device::C_PixelFormat *, int) {

    }
  };
}
