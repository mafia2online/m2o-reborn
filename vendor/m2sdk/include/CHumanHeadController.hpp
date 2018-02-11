/** @file CHumanHeadController.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"

namespace M2
{
    class ICHumanHeadController
    {
    public:
    };

    class C_HumanHeadController : public ICHumanHeadController
    {
    public:
        void SetFaceFxController(void *C_Human2FaceController)
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x421AE0, this, C_Human2FaceController);
        }

        void ShupUp()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x428850, this);
        }
    };
};
