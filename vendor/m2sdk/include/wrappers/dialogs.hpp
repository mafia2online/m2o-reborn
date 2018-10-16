#pragma once

namespace M2
{
    namespace Wrappers
    {
        namespace Dialogs
        {
            void ShowQuestDialog()
            {
                //Internal references of the quest screen
                M2::C_HashName *unk1 = (M2::C_HashName*)0x1CBBBE8;
                M2::C_HashName *unk2 = (M2::C_HashName*)0x1CBCEB0;

                M2::I_ScreenDaemon *daemon = M2::C_Menu::Get()->GetDaemon(unk1);
                if (!daemon) {
                    printf("null daemon\n");
                    return;
                }
                static char idk;
                void *quest = *(DWORD **)(*(int(__stdcall **)(char*, void*))(**(DWORD**)daemon + 0x18))(&idk, &unk2);
            }

            void AskQuestion()
            {
                //Internal references of the question screen
                M2::C_HashName *unk1 = (M2::C_HashName*)0x1CBBBE8;
                M2::C_HashName *unk2 = (M2::C_HashName*)0x1CBC598;

                //Someone may try to play with it and see what are those params
                M2::S_FadeParam params;
                params.a = 0xBF800000;
                params.b = 0xBF800000;
                params.c = 0xBF800000;

                int activate = M2::C_Menu::Get()->ActivateScreen(unk1, unk2, M2::E_ScreenOperationType::SCREEN_OPERATION_TYPE_QUESTION, params);
                if (activate <= 0) {
                    printf("Unable to activate the screen\n");
                    return;
                }
                M2::I_ScreenDaemon *daemon = M2::C_Menu::Get()->GetDaemon(unk1);
                if (!daemon) {
                    printf("null ptr\n");
                    return;
                }

                /*M2::C_Question *question = new M2::C_Question(unk2, daemon);
                static char idk;
                //M2::C_Question *question = (M2::C_Question*)*(DWORD **)(*(int(__stdcall **)(char*, void*))(**(DWORD**)daemon + 0x18))(&idk, &unk2);
                if (!question) {
                    mod_log("null ptr2\n");
                    return;
                }*/
                //question->SetQuestion(strtoul("0057000005", 0, 10));
                //question->SetAnswers(strtoul("0000000010", 0, 10), 1, strtoul("0059000014", 0, 10), 2, strtoul("0059000015", 0, 10), 3, strtoul("0059000005", 0, 10), 4, 10.0);
            }
        }
    }
}
