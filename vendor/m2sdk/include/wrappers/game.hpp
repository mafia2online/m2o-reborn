#pragma once

namespace M2
{
    namespace Wrappers
    {
        static inline void SwitchGenerators(bool bSwitch)
        {
            DWORD dwFunc = 0xAC6320;
            _asm push bSwitch;
            _asm call dwFunc;
        }

        static inline void SwitchFarAmbiants(bool bSwitch)
        {
            DWORD dwFunc = 0xAD2DA0;
            _asm push bSwitch;
            _asm call dwFunc;
        }

        static void SwitchSeason(bool winter)
        {
            if (winter) {
                M2::C_RangeMeter::Get()->OpenSeason(50);
            }
            else {
                M2::C_RangeMeter::Get()->OpenSeason(140);

            }
        }

        static void SwitchCarLock(C_Car *car)
        {
            auto player = reinterpret_cast<M2::C_Entity*>(M2::C_Game::Get()->GetLocalPed());
            auto ent = reinterpret_cast<M2::C_Entity*>(car);

            if (M2::C_CarOwnerDB::Get()->GetCarOwner(ent) == player) {
                car->LockEntryPoints();
                M2::C_CarOwnerDB::Get()->RemoveCar(ent);
            }
            else {
                M2::C_CarOwnerDB::Get()->MakeOwnership(ent, player);
                car->UnlockEntryPoints();
            }
        }
    };
};
