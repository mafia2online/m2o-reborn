#pragma once

#include <windows.h>
#include <list>

#include "m2sdk.h"

#define MODELMGR_MAX			1000
#define SDS_LOAD_DIR_ADDR		0x190ABDC
#define SDS_LOAD_DIR_CARS		"/sds/cars/"
#define SDS_LOAD_DIR_HCHAR		"/sds/hchar/"
#define SDS_LOAD_DIR_PLAYER		"/sds/player/"
#define SDS_LOAD_DIR_TRAFFIC    "/sds/traffic/"

static char * VehicleModels[] =
{
    "ascot_baileys200_pha",			"berkley_kingfisher_pha",			"fuel_tank",
    "gai_353_military_truck",		"hank_b",							"hank_fueltank",					"hot_rod_1",
    "hot_rod_2",					"hot_rod_3",						"houston_wasp_pha",					"isw_508",					"jeep",
    "jeep_civil",					"jefferson_futura_pha",				"jefferson_provincial",
    "lassiter_69",					"lassiter_69_destr",				"lassiter_75_fmv",					"lassiter_75_pha",
    "milk_truck",					"parry_bus",						"parry_prison",						"potomac_indian",			"quicksilver_windsor_pha",
    "quicksilver_windsor_taxi_pha",	"shubert_38",						"shubert_38_destr",					"shubert_armoured",
    "shubert_beverly",				"shubert_frigate_pha",				"shubert_hearse",					"shubert_panel",			"shubert_panel_m14",
    "shubert_taxi",					"shubert_truck_cc",					"shubert_truck_ct",
    "shubert_truck_ct_cigar",		"shubert_truck_qd",					"shubert_truck_sg",
    "shubert_truck_sp",				"sicily_military_truck",			"smith_200_pha",					"smith_200_p_pha",			"smith_coupe",
    "smith_mainline_pha",			"smith_stingray_pha",				"smith_truck",
    "smith_v8",						"smith_wagon_pha",
    "trailer_sg",					"ulver_newyorker",					"ulver_newyorker_p",                "walker_rocket",            "walter_coupe"
};

static char * PlayerModels[][2] =
{
    // Player
    { "/sds/player/", "vitarmy" },{ "/sds/player/", "vitksl" },{ "/sds/player/", "vitksl2" },
    { "/sds/player/", "vitnah_t" },{ "/sds/player/", "vitod1" },{ "/sds/player/", "vitod1neup" },
    { "/sds/player/", "vitod2" },{ "/sds/player/", "vitol1" },{ "/sds/player/", "vitol2" },
    { "/sds/player/", "vitoveral" },{ "/sds/player/", "vitpra" },{ "/sds/player/", "vitspo" },
    { "/sds/player/", "vittel" },{ "/sds/player/", "vitukl" },{ "/sds/player/", "vitukl_kn" },
    { "/sds/player/", "vitvez" },{ "/sds/player/", "vitvop" },{ "/sds/player/", "vitvov" },
    { "/sds/player/", "vitvov2i" },{ "/sds/player/", "vitvov3" },

    // Mission Characters
    { "/sds/hchar/", "albert" },{ "/sds/hchar/", "brianc" },{ "/sds/hchar/", "consig" },
    { "/sds/hchar/", "derek" },{ "/sds/hchar/", "desmond" },{ "/sds/hchar/", "joeoblf" },
    { "/sds/hchar/", "eddies" },{ "/sds/hchar/", "franca" },{ "/sds/hchar/", "frank" },
    { "/sds/hchar/", "joebryl" },{ "/sds/hchar/", "joeciv" },{ "/sds/hchar/", "joeneup" },
    { "/sds/hchar/", "joeobl" },{ "/sds/hchar/", "leospo" },{ "/sds/hchar/", "steve" },
    { "/sds/hchar/", "joeruka" },{ "/sds/hchar/", "marty" },

    // Traffic
    { "/sds/traffic/", "cbarma" },{ "/sds/traffic/", "cbarma2" },{ "/sds/traffic/", "ccerb1" },
    { "/sds/traffic/", "ccerb2" },{ "/sds/traffic/", "ccerb3" },{ "/sds/traffic/", "ccerml" },
    { "/sds/traffic/", "ccerml2" },{ "/sds/traffic/", "ccerml3" },{ "/sds/traffic/", "ccermz" },
    { "/sds/traffic/", "ccerzl" },{ "/sds/traffic/", "ccerzl1" },{ "/sds/traffic/", "ccinbg1" },
    { "/sds/traffic/", "ccinbg2" },{ "/sds/traffic/", "ccinbl" },{ "/sds/traffic/", "ccinga" },
    { "/sds/traffic/", "ccinga2" },{ "/sds/traffic/", "ccinku" },{ "/sds/traffic/", "ccinml" },
    { "/sds/traffic/", "ccinri" },{ "/sds/traffic/", "ccinzl" },{ "/sds/traffic/", "ccinzl2" },
    { "/sds/traffic/", "ccinzz" },{ "/sds/traffic/", "ccinzz2" },{ "/sds/traffic/", "ccisni" },
    { "/sds/traffic/", "ccisni2" },{ "/sds/traffic/", "cdel1" },{ "/sds/traffic/", "cdel2" },
    { "/sds/traffic/", "cdel3" },{ "/sds/traffic/", "cdel4" },{ "/sds/traffic/", "cdel5" },
    { "/sds/traffic/", "cdelje" },{ "/sds/traffic/", "cdete2" },{ "/sds/traffic/", "cdetek" },
    { "/sds/traffic/", "cerzz" },{ "/sds/traffic/", "cfetak1" },{ "/sds/traffic/", "cfetak2" },
    { "/sds/traffic/", "cguard1" },{ "/sds/traffic/", "cguard2" },{ "/sds/traffic/", "chlid1" },
    { "/sds/traffic/", "chlid2" },{ "/sds/traffic/", "chudrz" },{ "/sds/traffic/", "chudrz2" },
    { "/sds/traffic/", "chudrz3" },{ "/sds/traffic/", "cinfge" },{ "/sds/traffic/", "cirand" },
    { "/sds/traffic/", "cirand" },{ "/sds/traffic/", "cirga1" },{ "/sds/traffic/", "cirga2" },
    { "/sds/traffic/", "cirinf" },{ "/sds/traffic/", "cirjoh" },{ "/sds/traffic/", "cirpat" },
    { "/sds/traffic/", "cirsea" },{ "/sds/traffic/", "citant" },{ "/sds/traffic/", "citga1c" },
    { "/sds/traffic/", "citga1f" },{ "/sds/traffic/", "citga1v" },{ "/sds/traffic/", "citga2c" },
    { "/sds/traffic/", "citga2f" },{ "/sds/traffic/", "citga2v" },{ "/sds/traffic/", "citga3c" },
    { "/sds/traffic/", "citga3f" },{ "/sds/traffic/", "citga3v" },{ "/sds/traffic/", "citga4c" },
    { "/sds/traffic/", "citga4f" },{ "/sds/traffic/", "citga4v" },{ "/sds/traffic/", "citga5v" },
    { "/sds/traffic/", "citga8" },{ "/sds/traffic/", "citga9" },{ "/sds/traffic/", "citga10c" },
    { "/sds/traffic/", "citga11f" },{ "/sds/traffic/", "citga12f" },{ "/sds/traffic/", "citga13c" },
    { "/sds/traffic/", "citkas" },{ "/sds/traffic/", "citklav" },{ "/sds/traffic/", "citseb" },
    { "/sds/traffic/", "citukl" },{ "/sds/traffic/", "citvov1" },{ "/sds/traffic/", "citvov2" },
    { "/sds/traffic/", "citzst" },{ "/sds/traffic/", "cjoan" },{ "/sds/traffic/", "cmesmz" },
    { "/sds/traffic/", "cmeszl" },{ "/sds/traffic/", "cmeszl2" },{ "/sds/traffic/", "cmeszz" },
    { "/sds/traffic/", "cmeszz2" },{ "/sds/traffic/", "cmeszz3" },{ "/sds/traffic/", "cmot1" },
    { "/sds/traffic/", "cmot2" },{ "/sds/traffic/", "cmot3" },{ "/sds/traffic/", "cmot4" },
    { "/sds/traffic/", "cmotbo" },{ "/sds/traffic/", "cmrtvl" },{ "/sds/traffic/", "cpr1ce" },
    { "/sds/traffic/", "cpr2st" },{ "/sds/traffic/", "cpr3zm" },{ "/sds/traffic/", "cpr4" },
    { "/sds/traffic/", "cpr5" },{ "/sds/traffic/", "cpr7" },{ "/sds/traffic/", "cpros1l" },
    { "/sds/traffic/", "cpros1n_low" },{ "/sds/traffic/", "cpros1z" },{ "/sds/traffic/", "cpros2l" },
    { "/sds/traffic/", "cpros2z" },{ "/sds/traffic/", "cpros3l" },{ "/sds/traffic/", "cpros3z" },
    { "/sds/traffic/", "cpros6l" },{ "/sds/traffic/", "cpros6z" },{ "/sds/traffic/", "cpump1" },
    { "/sds/traffic/", "csicde" },{ "/sds/traffic/", "csicde2" },{ "/sds/traffic/", "csicde3" },
    { "/sds/traffic/", "csicmu" },{ "/sds/traffic/", "csicmu3" },{ "/sds/traffic/", "csicze" },
    { "/sds/traffic/", "csicze2" },{ "/sds/traffic/", "csicze3" },{ "/sds/traffic/", "csomra" },
    { "/sds/traffic/", "csomra2" },{ "/sds/traffic/", "cvez1" },{ "/sds/traffic/", "cvez2" },
    { "/sds/traffic/", "cvez3" },{ "/sds/traffic/", "cvez4" },{ "/sds/traffic/", "cvez5" },
    { "/sds/traffic/", "cvez6" },{ "/sds/traffic/", "cvez7" },{ "/sds/traffic/", "cvezci1" },
    { "/sds/traffic/", "cvezci2" },{ "/sds/traffic/", "cvezci3" },{ "/sds/traffic/", "cvezedv" },
    { "/sds/traffic/", "cvezga1" },{ "/sds/traffic/", "cvezga2" },{ "/sds/traffic/", "cvezjim" },
    { "/sds/traffic/", "cvezjon" },{ "/sds/traffic/", "cvezvin" },{ "/sds/traffic/", "driver" },

    // Police chars
    { "/sds/police_char", "m11police" },{ "/sds/police_char", "m14china_pol" },{ "/sds/police_char", "police_char" }
};

namespace M2
{
    namespace Models
    {
        void GetVehicleModelFromID(const int id, std::string *model)
        {
            model->assign(VehicleModels[id]);
        }

        void GetPlayerModelFromiD(const int id, std::string *directory, std::string *model)
        {
            directory->assign(PlayerModels[id][0]);
            model->assign(PlayerModels[id][1]);
        }
    };

    namespace Wrappers
    {
        static char *GetDir()
        {
            return (char *)(SDS_LOAD_DIR_ADDR);
        }

        static void SetDir(const char *directory)
        {
            char *loadPath = (char *)(SDS_LOAD_DIR_ADDR);

            DWORD dwOldProtect = 0;
            VirtualProtect(loadPath, 0x20, PAGE_READWRITE, &dwOldProtect);

            sprintf(loadPath, "%s%%s.sds", directory);

            VirtualProtect(loadPath, 0x20, dwOldProtect, &dwOldProtect);
        }

        class GameModelManager
        {
        private:
            ICPlayerModelManager    *m_pModelManager;

        public:
            ICPlayerModelManager    *GetModelManager() { return m_pModelManager; }

        public:
            GameModelManager(): m_pModelManager(nullptr)
            {
                ICPlayerModelManager *pModelManager = (ICPlayerModelManager *)M2::Alloc(0xA0);
                if (pModelManager)
                {
                    DWORD address = 0x5B6680;
                    __asm
                    {
                        mov ecx, pModelManager;
                        call address;
                    }
                    m_pModelManager = pModelManager;
                }
            }

            GameModelManager(ICPlayerModelManager * pModelManager)
            { 
                m_pModelManager = pModelManager; 
            }

            ~GameModelManager()
            {
                if (m_pModelManager) {
                    Free();
                    m_pModelManager = nullptr;
                }
            }

            bool Load(const char *directory, const char *model)
            {
                if (!m_pModelManager)
                    return false;

                bool result = false;
                ICPlayerModelManager *pModelManager = m_pModelManager;

                SetDir(directory);

                DWORD address = 0x5CBBE0;
                __asm
                {
                    push model;
                    mov ecx, pModelManager;
                    call address;
                    mov result, al;
                }

                strcpy(pModelManager->m_sModelName, model);

                SetDir(SDS_LOAD_DIR_PLAYER);
                
                return result;
            }

            void Free()
            {
                if (!m_pModelManager)
                    return;

                ICPlayerModelManager *pModelManager = m_pModelManager;

                DWORD address = 0x5AF310;

                __asm
                {
                    mov ecx, pModelManager;
                    call address;
                }
            }
        };
        class ModelManager
        {
            friend class GameModelManager;

        private:
            std::list< GameModelManager *>  m_pModelManagers;

        public:
            ModelManager() : m_pModelManagers()
            {

            }

            ~ModelManager()
            {
                m_pModelManagers.clear();
            }

            GameModelManager    *Load(const char *directory, const char *model)
            {
                if (m_pModelManagers.size() >= MODELMGR_MAX) {
                    return nullptr;
                }

                GameModelManager *pModelManager = new GameModelManager();
                if (!pModelManager)
                    return nullptr;

                if (!pModelManager->Load(directory, model)) {
                    delete(pModelManager);
                    return nullptr;
                }

                m_pModelManagers.push_back(pModelManager);
                return pModelManager;
            }

            bool    Free(const char *model)
            {
                for (std::list<GameModelManager *>::iterator iter = m_pModelManagers.begin(); iter != m_pModelManagers.end(); iter++)
                {
                    GameModelManager *manager = (*iter);
                    if (!manager)
                        continue;

                    if (!manager->GetModelManager()->m_pSlot)
                        continue;

                    if (manager->GetModelManager()->m_pSlot->GetModelByFileName(model))
                    {
                        m_pModelManagers.remove(manager);
                        delete(manager);
                        return true;
                    }
                }
                return false;
            }

            bool Free(GameModelManager * pModelManager)
            {
                if (!pModelManager || !pModelManager->GetModelManager())
                    return false;

                return Free(pModelManager->GetModelManager()->m_sModelName);
            }

            void Clear()
            {
                for (std::list < GameModelManager * >::iterator iter = m_pModelManagers.begin(); iter != m_pModelManagers.end(); iter++)
                {
                    delete(*iter);
                }

                m_pModelManagers.clear();
            }

            GameModelManager *GetModelManagerByName(const char * szModelName)
            {
                for (std::list < GameModelManager * >::iterator iter = m_pModelManagers.begin(); iter != m_pModelManagers.end(); iter++)
                {
                    GameModelManager * manager = (*iter);

                    if (!manager->GetModelManager() || !manager->GetModelManager()->m_pSlot)
                        continue;

                    if (!strcmp(szModelName, manager->GetModelManager()->m_sModelName))
                        return manager;
                }

                return nullptr;
            }
        };
    };
};
