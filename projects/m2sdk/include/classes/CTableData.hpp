
#pragma once

namespace M2
{
    class ICTableData
    {
    public:
        void* m_pVFTable;           //0000 - 0004
        void* m_pUnkPtr;            //0004 - 0008
        int m_iUsedLines;           //0008 - 000C
        int m_iAvailableLines;      //000C - 0010
    };

    class C_TableData : public ICTableData
    {
    public:

        //I can't determine the return type of that function for the moment
        template < typename T >
        T GetLine(unsigned int line)
        {
            return Mem::InvokeFunction<Mem::call_this, T>(0xF32410, this, line);
        }
    };
}
