#pragma once
#include <Shared/Common.h>
#include <Shared/Client/CPatcher.h>

namespace M2
{

	template < typename T, Address A > class GameClassWrapper
	{
	public:
		static T *Get()
		{
			return reinterpret_cast<T*>(A);
		}
	};
}