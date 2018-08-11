/** @file CLockpick.hpp
*  @brief Game's Lockpick wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
    class ICLockpick
    {
    public:
    };

    class C_Lockpick : public ICLockpick
    {
    public:
        void Start()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x455C00, this);
        }
    };
};
