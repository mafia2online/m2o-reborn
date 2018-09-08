/** @file CSlotWrapper.cpp
*  @brief Game's slots manager wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
    class ICSlotWrapper
    {
    public:
        DWORD *vTable;
        C_Slot* m_pSlot;
        int m_iCount;
        char *m_szName;
    };

    class C_SlotWrapper : public ICSlotWrapper
    {
    public:

    };
};
