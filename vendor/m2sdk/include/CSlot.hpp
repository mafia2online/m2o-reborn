/** @file CSlot.hpp
*  @brief Game's class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
    class ICSlot
    {
    public:
        pad(ICSlot, pad0, 0x18);            // 0000 - 0018
        char            m_sName[32];        // 0018 - 0038
        pad(ICSlot, pad1, 0x8);             // 0038 - 0040
        int             m_iType;            // 0040 - 0044
        pad(ICSlot, pad2, 0x10);            // 0044 - 0054
        int             m_iRefCount;        // 0054 - 0058
        pad(ICSlot, pad3, 0x1C);            // 0058 - 0074
        char            m_sModelPath[260];  // 0074 - 0178
        pad(ICSlot, pad4, 0xC);             // 0178 - 0184
        void            *m_pUnknown;        // 0184 - 0188
        pad(ICSlot, pad5, 0x20);            // 0188 - 01A8
        int             m_iSlotNumber;      // 01A8 - 01AC
    };

    class C_Slot : public ICSlot
    {
    public:
        C_Model *GetModelByFileName(const char *file)
        {
            return Mem::InvokeFunction<Mem::call_this, C_Model*>(0x5991C0, this, file);
        }
    };
};
