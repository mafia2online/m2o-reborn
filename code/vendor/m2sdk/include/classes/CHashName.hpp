/** @file CHashName.cpp
*  @brief Game's hashing Class
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
    class ICHashName
    {
    public:
        void *m_pVFTable;
    };

    class C_HashName : public ICHashName
    {
    public:
        void SetName(const char *str)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x181D0D0, this, str);
        }
    };
};
