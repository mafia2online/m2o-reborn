
/** @file CPoliceRootCoordinator.cpp
*  @brief Game's Police AI wrapper
*
*
*  @author Tyldar (darreugne@gmail.com)
*/
#pragma once

namespace M2
{
    enum E_OffenceType : int
    {
        OFFENCE_CARIMPACT = 35,
        OFFENCE_SPEEDING = 15,
        OFFENCE_RESISTARREST = 210,
        OFFENCE_SUSPICIOUS = 4,
    };

    class ICPoliceRootCoordinator
    {
    public:
    };

    class C_PoliceRootCoordinator : public GameClassWrapper<C_PoliceRootCoordinator, ICPoliceRootCoordinator, 0x1CC80E0>
    {
    public:
        int CreateOffence(E_OffenceType type, C_Entity *ent, int unk, float unk2)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0xBFA200, this, type, ent, unk, unk2);
        }

        bool DeactivateOffence(int offenceID)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0xBCF630, this, offenceID);
        }

        bool DeactivateOffence(int offenceID, int unk)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0xBCF5B0, this, offenceID, unk);
        }

        bool RemoveAllOffences(int unk)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0xBF1F50, this, unk);
        }

        int RemoveWantedCars()
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0xBB3ED0);
        }
    };
};
