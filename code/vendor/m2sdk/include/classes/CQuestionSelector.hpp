#pragma once

namespace M2
{
    class ICQuestionSelector
    {
    };

    class C_QuestionSelector : public ICQuestionSelector
    {
        void SetArrowFlags(BYTE flags)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x893A00, this, flags);
        }

        void SetQuestionID(unsigned int id)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x893B60, this, id);
        }

        void SetTextId(int question, int id)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x893A20, this, question, id);
        }
    };
};
