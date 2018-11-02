/** @file CSlot.hpp
*  @brief Game's class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once

namespace M2
{
    enum E_SlotTypes
    {
        SLOT_CITY = 1,
        SLOT_CITY_GROUND = 2,
        SLOT_CITY_UNIVERS = 3,
        SLOT_SHOP = 4,
        SLOT_CHAR_UNIVERS = 5,
        SLOT_PLAYER = 6,
        SLOT_POLICE_CHAR = 9,
        SLOT_CAR_UNIVERS = 10,
        SLOT_BASE_ANIM = 15,
        SLOT_WEAPONS = 16,
        SLOT_GUI = 17,
        SLOT_SKY = 18,
        SLOT_TABLES = 19,
        SLOT_DEFAULT_SOUND = 20,
        SLOT_PARTICLES = 21,
        SLOT_GAME_SCRIPT = 22,
        SLOT_CITY_CRASH = 31,
        SLOT_GENERATE = 32,
        SLOT_SMALL = 33,
        SLOT_SCRIPT_SOUNDS = 34,
        SLOT_DIRECTOR_LUA = 35,
        SLOT_MAPA = 36,
        SLOT_SOUND_CITY = 37,
        SLOT_ANIMS_CITY = 38,
        SLOT_GENERIC_SPEECH_NORMAL = 41,
        SLOT_GENERIC_SPEECH_GANGSTER = 42,
        SLOT_GENERIC_SPEECH_VARIOUS = 43,
        SLOT_GENERIC_SPEECH_STORY = 44,
        SLOT_GENERIC_SPEECH_POLICE = 47,
        SLOT_TEXT = 48,
        SLOT_INGAME = 50,
        SLOT_INGAME_GUI = 51,
        SLOT_DABING = 52
    };

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
        bool LoadData(const char *path, int unk, int unk2, int unk3)
        {
            return Mem::InvokeFunction<Mem::call_this, bool>(0x5AA780, this, &path, unk, unk2, unk3);
        }

        C_Model *GetModelByFileName(const char *file)
        {
            return Mem::InvokeFunction<Mem::call_this, C_Model*>(0x5991C0, this, file);
        }

        int GetState(char unk)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x598F40, this, unk);
        }
    };
};
