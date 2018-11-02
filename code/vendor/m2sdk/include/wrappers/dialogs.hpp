#pragma once

namespace M2
{
    namespace Wrappers
    {
        namespace Dialogs
        {
            bool ShowQuestDialog()
            {
                //Internal references of the quest screen
                M2::C_HashName *unk1 = (M2::C_HashName*)0x1CBBBE8;
                M2::C_HashName *unk2 = (M2::C_HashName*)0x1CBCEB0;

                auto menu = M2::C_Menu::Get();
                if (!menu) {
                    printf("unable to fetch menu instance\n");
                    return false;
                }

                M2::C_BaseDaemon *daemon = M2::C_Menu::Get()->GetDaemon(unk1);
                if (!daemon) {
                    printf("null ptr\n");
                    return false;
                }

                M2::C_StartQuest *quest = nullptr;

                __asm {
                    // double derefence ? meye C_RefPtr stuff ?
                    mov eax, daemon
                    mov     ecx, [eax]
                    mov     edx, [ecx]

                    // Get vtable of GetHelper
                    mov     edx, [edx + 0x18]

                    // Push first arg of GetHelper function ( hash of name )
                    push    unk2

                    // weird stuff ? this should be this ptr ( why its daemon - 0x4 again C_RefPtr stuff ?)
                    sub eax, 0x4
                    push    eax
                    call    edx

                    // this doesent seems to return eax !
                    // meybe function modify it trough parameter ( most likely )

                    // derefence result
                    mov esi, dword ptr ds : [eax]
                    mov quest, esi
                }
                if (!quest) {
                    printf("null ptr2\n");
                    return false;
                }

                *(char **)*(DWORD *)(quest + 0x90) = "pinup01_ps";
                *(DWORD *)(quest + 0x98) = strtoul("0059230008", 0, 10);
                *(DWORD *)(quest + 0x9C) = strtoul("0059230008", 0, 10);
                *(DWORD *)(quest + 0xA0) = strtoul("0059230008", 0, 10);

                printf("quest = 0x%p\n", quest);

                M2::C_SyncObject *pSyncObject = nullptr;
                quest->Show(&pSyncObject);
            }

            bool AskQuestion(const char *quest, const char *option1, const char *option2, const char *option3, const char *option4)
            {
                //Internal references of the question screen
                M2::C_HashName *unk1 = (M2::C_HashName*)0x1CBBBE8;
                M2::C_HashName *unk2 = (M2::C_HashName*)0x1CBC598;

                //Someone may try to play with it and see what are those params
                M2::S_FadeParam params;
                params.a = 0xBF800000;
                params.b = 0xBF800000;
                params.c = 0xBF800000;

                auto menu = M2::C_Menu::Get();
                if (!menu) {
                    printf("unable to fetch menu instance\n");
                    return false;
                }

                int activate = menu->ActivateScreen(unk1, unk2, M2::E_ScreenOperationType::SCREEN_OPERATION_TYPE_QUESTION, params);
                if (activate <= 0) {
                    printf("Unable to activate the screen\n");
                    return false;
                }
                M2::C_BaseDaemon *daemon = M2::C_Menu::Get()->GetDaemon(unk1);
                if (!daemon) {
                    printf("null ptr\n");
                    return false;
                }

                printf("menu : 0x%p\n", menu);
                printf("daemon : 0x%p\n", daemon);

                M2::C_Question *question = nullptr;

                __asm {
                    // double derefence ? meye C_RefPtr stuff ?
                    mov eax, daemon
                    mov     ecx, [eax]
                    mov     edx, [ecx]

                    // Get vtable of GetHelper
                    mov     edx, [edx + 0x18]

                    // Push first arg of GetHelper function ( hash of name )
                    push    unk2

                    // weird stuff ? this should be this ptr ( why its daemon - 0x4 again C_RefPtr stuff ?)
                    sub eax, 0x4
                    push    eax
                    call    edx

                    // this doesent seems to return eax !
                    // meybe function modify it trough parameter ( most likely )

                    // derefence result
                    mov esi, dword ptr ds : [eax]
                    mov question, esi
                }

                if (!question) {
                    printf("null ptr2\n");
                    return false;
                }

                question->SetQuestion(strtoul(quest, 0, 10));
                question->SetAnswers(strtoul(option1, 0, 10), 1, strtoul(option2, 0, 10), 2, strtoul(option3, 0, 10), 3, strtoul(option4, 0, 10), 4, 20.0);
                return true;
            }

            int GetLastScreenAnswer()
            {
                //TODO: To be done
                return 1;
            }
        }
    }
}
