namespace M2 {
    /* TODO: Fix previous natives and delete those ones */
    DWORD dwAddress = 0x0993B00;
    C_SyncObject _declspec(naked) *C_HumanScript::ScrAimAt(C_SyncObject **syncObject, M2::C_Entity *ent, Vector3 const &pos, const bool smooth)
    {
        _asm jmp dwAddress
    }
    DWORD dwAddress2 = 0x9931D0;
    C_SyncObject _declspec(naked) *C_HumanScript::ScrLookAt(C_SyncObject **syncObject, M2::C_Entity *ent, Vector3 const &pos, const bool smooth)
    {
        _asm jmp dwAddress2;
    }
    DWORD dwAddress6 = 0x951920;
    C_SyncObject _declspec(naked) *C_HumanScript::ScrLookAtAngles(C_SyncObject **syncObject, M2::C_Entity *ent, float x, float y, float z)
    {
        _asm jmp dwAddress6;
    }
    DWORD dwAddress7 = 0x993330;
    C_SyncObject _declspec(naked) *C_HumanScript::ScrLookAtDir(C_SyncObject **syncObject, M2::C_Entity *ent, Vector3 const &pos)
    {
        _asm jmp dwAddress7;
    }
    DWORD dwAddress3 = 0x9910B0;
    C_SyncObject _declspec(naked) *C_HumanScript::ScrMoveV(C_SyncObject **syncObject, const Vector3 &begin, const eHumanMoveMode moveMode, const Vector3 &target, const bool smoothStop)
    {
        _asm jmp dwAddress3;
    }
    DWORD dwAddress4 = 0x993C30;
    C_SyncObject _declspec(naked) *C_HumanScript::ScrShootAt(C_SyncObject **syncObject, M2::C_Entity *ent, Vector3 const &dir, const bool smooth)
    {
        _asm jmp dwAddress4;
    }
    DWORD dwAddress5 = 0x9950E0;
    C_SyncObject _declspec(naked) *C_HumanScript::ScrPanicRunAway(C_SyncObject **syncObject, const Vector3 &dir)
    {
        __asm jmp dwAddress5;
    }
}
