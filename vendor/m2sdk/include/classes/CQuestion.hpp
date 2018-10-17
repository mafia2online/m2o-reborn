#pragma once

#include "CHashName.hpp"
#include "CQuestionSelector.hpp"
#include "CMenu.hpp"

namespace M2
{
    class ICQuestion
    {
    public:
        pad(ICQuestion, pad0, 0x68);
        C_QuestionSelector *m_pSelector;
    };

    class C_Question : public ICQuestion
    {
    public:
        char* CreateParametrizedString(const char *id)
        {
            return Mem::InvokeFunction<Mem::call_this, char *>(0x87A1A0, this, id);
        }

        void ForceClose()
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x87A4B0, this);
        }

        void SetAnswers(unsigned long int a1, unsigned long int a1ID, unsigned long int a2, unsigned long int a2ID, unsigned long int a3, unsigned long int a3ID, unsigned long int a4, unsigned long int a4ID, float delay)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x8D2DC0, this, a1, a1ID, a2, a2ID, a3, a3ID, a4, a4ID, delay);
        }

        void SetQuestion(unsigned int question)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x8D2D70, this, question);
        }
    };
};
