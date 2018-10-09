/** @file CTelephone.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
    class ICTelephone
    {
    public:
    };

    class C_Telephone : public ICTelephone
    {
    public:
        void CallFinished(int finished)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x44C310, this, finished);
        }

        void InterruptCall()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xD6FD30, this);
        }

        bool IsBroken()
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x44C200, this);
        }

        bool IsCalling()
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0xD6FC90, this);
        }

        bool IsRinging()
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x44C280, this);
        }

        bool NumberDialed(int number)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x48B4A0, this, number);
        }
    };
};
