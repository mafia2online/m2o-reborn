/** @file CPoliceWrapper.cpp
*  @brief Game's Police AI wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/
#pragma once

namespace M2
{
    class ICPoliceWrapper
    {
    public:
    };

    class C_PoliceWrapper : public ICPoliceWrapper
    {
    public:
        bool IsCarKnown(int unk)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0xBB3F50, unk);
        }
    };
};
