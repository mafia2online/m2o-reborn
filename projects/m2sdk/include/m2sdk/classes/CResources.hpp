/** @file CResources.cpp
*  @brief Game's CResources wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
    class ICResources
    {
    public:
    };

    class C_Resources : public ICResources
    {
    public:
        int GetCityAreaName(vec2_t pos, bool unk)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x860D20, this, &pos, unk);
        }
    };
};
