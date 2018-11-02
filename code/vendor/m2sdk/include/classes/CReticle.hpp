#pragma once

namespace M2
{
    class C_Reticle
    {
    public:

        void SetCircleSize(float size)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x875A80, this, size);
        }

        void SetColor(int c1, int c2, int c3)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x8611D0, this, c1, c2, c3);
        }

        void SetCrossSize(float size)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x8759D0, this, size);
        }

        void SetPosition(float x, float y)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x875960, this, x, y);
        }

        void Show(bool show)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x88BDE0, this, show);
        }
    };
}
