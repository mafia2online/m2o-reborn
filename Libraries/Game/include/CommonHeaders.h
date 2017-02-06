#pragma once
#include <Shared/Common.h>
#include <Shared/Client/CPatcher.h>

namespace M2
{

	template < typename T > class ClassWrapper
	{
	public:
		static T &Instance()
		{
			static T instance;
			return instance;
		}
	};
}