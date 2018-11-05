#pragma once
#include <cstdint>

#include "../filesystem.h"

namespace ue::sys::filesystem {

	class C_XmlFile {
	public:
		// ctor 0x694E60

		virtual ~C_XmlFile() = 0;

		int32_t Load(const char* file) {
			return nio::call(0x69A580, file);
		}

		int32_t LoadXML(ue::sys::utils::C_HashName* name) {
			return nio::call(0x699270, name);
		}
	};
}
