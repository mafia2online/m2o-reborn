#pragma once
#include "CommonHeaders.h"

namespace M2
{
    class ICWrappersList
    {

    };

    class C_WrappersList : public GameClassWrapper<C_WrappersList, ICWrappersList, 0x1CC14A8>
    {
    public:
        C_Entity *GetEntityByGUID(unsigned int guid)
        {
            return Mem::InvokeFunction<Mem::call_this, C_Entity*>(0xD95450, this, guid);
        }

        C_Entity *GetEntityByName(const char *name)
        {
            return Mem::InvokeFunction<Mem::call_this, C_Entity*>(0xD955A0, this, name);
        }
    };
};
