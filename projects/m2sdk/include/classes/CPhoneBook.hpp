/** @file CPhoneBook.cpp
*  @brief Game's PhoneBook wrapper
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
    class ICPhoneBook
    {
    public:
        void *m_pVFTable;
    };

    class C_PhoneBook : public GameClassWrapper<C_PhoneBook, ICPhoneBook, 0x1CBF918>
    {
    public:
        bool AddHiddenNumber(int a2, int a3, char *a4)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0xA126B0, this, a2, a3, a4);
        }

        bool AddNumber(int a2, int a3, int a4, int a5, int a6, char *a7, int hidden)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0xA125E0, this, a2, a3, a4, a5, a6, a7, hidden);
        }

        int ChooseRandomNumber(int unk)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x9DCA50, this, unk);
        }

        void Clear()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xA175A0, this);
        }

        bool ContainsNumber(int number)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x9E3C80, this, number);
        }

        int GetEntriesCount()
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x9C0BD0, this);
        }

        int GetEntry(int entry)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x9C0BE0, this, entry);
        }

        int GetEntryByNumber(int number)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x9E3CB0, this, number);
        }

        void RemoveNumber(int number)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x9F91D0, this, number);
        }

        void RemoveNumberByTextId(int number)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x9F9260, this, number);
        }
    };
};
