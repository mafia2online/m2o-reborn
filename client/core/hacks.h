/**
 * TEMP file for testing some stuff etc
 * TODO: remove someday
 */

M2::C_Human2 *human = nullptr;

vec3_t mrot;
vec3_t next_pos;
bool test_ex = false;


vec3_t GetEntityRot(M2::C_Entity* ent)
{
    M2::C_Frame* human_frame = *(M2::C_Frame**)((DWORD)ent + 0x60);

    return { *(float*)((DWORD)human_frame + 0x44), *(float*)((DWORD)human_frame + 0x54), *(float*)((DWORD)human_frame + 0x64) };
}


void hackit() {
    if (test_ex) return;
    test_ex = true;
    mod_log("doin stuff\n");

    const char *directory = "/sds/traffic/";
    const char *model = "cvezjon";

    M2::Wrappers::ModelManager *pModelManager = new M2::Wrappers::ModelManager();
    if (!pModelManager) {
        return;
    }

    M2::Wrappers::GameModelManager *pPedModelManager = pModelManager->Load(directory, model);
    if (!pPedModelManager) {
        return;
    }

    human = M2::C_EntityFactory::Get()->CreateEntity<M2::C_Human2>(M2::EntityTypes::Entity_Human);
    if (!human) {
        return;
    }

    M2::C_Model *pModel = M2::C_Core::Get()->AllocateModel(2);
    if (!pModel) {
        return;
    }

    pModel->CloneHierarchy(pPedModelManager->GetModelManager()->m_pModel);
    if (!pModel) {
        return;
    }

    pModel->SetName("m2online_ped");
    pModel->MarkForNotify(2);

    reinterpret_cast<M2::C_Entity *>(human)->SetModel(pModel);
    reinterpret_cast<M2::C_Entity *>(human)->Setup();

    DWORD flags = reinterpret_cast<M2::C_Entity *>(human)->m_dwFlags &= 0xFFFFB7BF | 0x4800;
    reinterpret_cast<M2::C_Entity *>(human)->m_dwFlags = flags;

    if (reinterpret_cast<M2::C_Entity *>(human)->m_dwFlags & 0x20)
        mod_log("Flags set sucessfully!\n");
    else {
        reinterpret_cast<M2::C_Entity *>(human)->Release();
    }

    reinterpret_cast<M2::C_Entity *>(human)->Activate();

    if (reinterpret_cast<M2::C_Entity *>(human)->IsActive())
        mod_log("Entity active !\n");

    auto pos = ((M2::C_Entity*)M2::C_Game::Get()->GetLocalPed())->GetPosition();
    reinterpret_cast<M2::C_Entity *>(human)->SetPosition(pos);

    mod_log("Created at %x!\n", human);

    //mrot = GetEntityRot((M2::C_Entity*)M2::C_Game::Get()->GetLocalPed());
    next_pos = ((M2::C_Entity*)M2::C_Game::Get()->GetLocalPed())->GetPosition() + vec3(2.0f, 3.0f, 0.0f);
}


void hackitick() {
    if (!human) return;


    static int i = 0;

    //quat_t mrot = { 0 };


    //M2::C_Frame* human_frame = *(M2::C_Frame**)((DWORD)human + 0x60);
    //*(float*)((DWORD)human_frame + 0x44) = 0.0f;
    //*(float*)((DWORD)human_frame + 0x54) = 0.0f;
    //*(float*)((DWORD)human_frame + 0x64) = 0.5f;
    //human_frame->MarkForNotify(2);

    //mod_log("%x\n", ((DWORD)human_frame + 0x44));

    // mrot.x += 5.0f;
    // //mrot.z = 120.0f;
    // vec3_t direction; 
    // auto ped2 = M2::C_Game::Get()->GetLocalPed();
    // unsigned long CHuman_GetDirRequest = 0x0094F7A0;

    // __asm
    // {
    //     lea eax, direction
    //     push eax
    //     mov ecx, ped2
    //     call CHuman_GetDirRequest
    //     add esp, 0x4
    // }

    // //auto p = ((M2::C_Human2*)M2::C_Game::Get()->GetLocalPed())->GetWeaponController

    // auto d =((M2::C_Entity*)M2::C_Game::Get()->GetLocalPed())->GetDirection();
    // d.x = 0.0f;
    // print_pos(direction);

    //((M2::C_Entity*)human)->SetDirection(d);

    //auto mrot = ((M2::C_Entity*)M2::C_Game::Get()->GetLocalPed())->GetRotation();


    auto pos = ((M2::C_Entity*)M2::C_Game::Get()->GetLocalPed())->GetPosition() + vec3(2.0f, 3.0f, 0.0f);
    ((M2::C_Entity*)human)->SetPosition(pos);

    // each 60 ticks
    if (i++ > 32) {
        print_posm(next_pos, "going to place\n");
        i = 0;


        //((M2::C_Entity*)human)->SetPosition(next_pos);


        auto curpos = ((M2::C_Entity*)M2::C_Game::Get()->GetLocalPed())->GetPosition();
        M2::C_SyncObject *pSyncObject = nullptr;
        human->GetScript()->ScrMoveV(&pSyncObject, curpos, M2::HUMAN_MOVE_MODE_WALK, curpos, true);

        next_pos = ((M2::C_Entity*)M2::C_Game::Get()->GetLocalPed())->GetPosition() + vec3(2.0f, 3.0f, 0.0f);
    }
    

    //mod_log("value iof human 0x8F00: %d\n", *(DWORD*)((DWORD)human + 0x8F00));
    //mod_log("value iof human 0x2E0: %d\n", *(DWORD*)((DWORD)human + 0x2E0));

    *(DWORD*)((DWORD)human + 0x8F00) = 2049;
    *(DWORD*)((DWORD)human + 0x2E0) = 515;
}
