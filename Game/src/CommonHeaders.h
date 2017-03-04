#pragma once
#include <Shared/Common.h>
#include <Shared/Client/CPatcher.h>

namespace M2
{
	#define pad(p,n,s) Byte p##__##n##[s]

	template < typename T, Address A > class GameClassWrapperStatic
	{
	public:
		static T *Get()
		{
			return reinterpret_cast<T*>(A);
		}
	};

	template < typename T, Address A > class GameClassWrapper
	{
	public:
		static T *Get()
		{
			return reinterpret_cast<T*>(*(Address*)A);
		}
	};
}