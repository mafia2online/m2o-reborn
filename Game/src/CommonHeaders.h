#pragma once
#include <Shared/Common.h>
#include <Shared/Client/CPatcher.h>

namespace M2
{
#define pad(p,n,s) Byte p##__##n##[s]
#define VTBLCall(retn,name,...) Mem::InvokeFunction<Mem::call_this, retn>(GetInterface()->m_pVFTable->##name, this, __VA_ARGS__)

	template < typename T, typename I, Address A > class GameClassWrapperStatic
	{
	public:
		inline I* GetInterface() { return reinterpret_cast<I*>(this); }

		static T *Get()
		{
			return reinterpret_cast<T*>(A);
		}
	};

	template < typename T, typename I, Address A > class GameClassWrapper
	{
	public:
		inline I* GetInterface() { return reinterpret_cast<I*>(this); }

		static T *Get()
		{
			return reinterpret_cast<T*>(*(Address*)A);
		}
	};

	template < typename T > class GameFakeClassWrapper
	{
	public:
		static T *Get()
		{
			static T classs;
			return &classs;
		}
	};
}