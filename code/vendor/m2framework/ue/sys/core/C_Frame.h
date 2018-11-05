#pragma once
#include <ue/I_BaseObject.h>

namespace ue::sys::core
{
	class C_Frame :
		public ue::I_BaseObject,
		public ue::sys::memorymanager::I_WeakReferencable
	{};
}
