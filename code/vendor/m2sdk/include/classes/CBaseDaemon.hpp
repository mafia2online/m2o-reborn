#pragma once

#include "CRefPtr.hpp"
namespace M2
{
    class ICBaseDaemon
    {
    public:
    };

    class C_BaseDaemon : public ICBaseDaemon
    {
    public:
        template <typename T>
        C_RefPtr<T>    GetHelper(C_HashName *name)
        {
            return Mem::InvokeFunction<Mem::call_this, C_RefPtr<T>>(0x878380, this, name);
        }
    };
};
