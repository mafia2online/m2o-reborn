
#include "CTableData.hpp"
#include "CEntityFactory.hpp"

#pragma once

namespace M2
{
    class ICTables
    {
    public:
        pad(ICTables, pad0, 0x1C);                  //0000 - 001C
        C_TableData* m_pPoliceOffencesTable;        //001C - 0020
        C_TableData* m_pWeaponsTable;               //0020 - 0024
        pad(ICTables, pad1, 0x4);                   //0024 - 0028
        C_TableData* m_pAttackParamsTable;          //0028 - 002C
        pad(ICTables, pad2, 0x4);                   //002C - 0030
        C_TableData* m_pVehiclesTable;              //0030 - 0034
        pad(ICTables, pad3, 0x28);                  //0034 - 005C
        C_TableData* m_pPhysObjSounds2Table;        //005C - 0060
        pad(ICTables, pad4, 0x4);                   //0060 - 0064
        C_TableData* m_pMaterialsPhysicsTable;      //0064 - 0068
        C_TableData* m_pMaterialsShotsTable;        //0068 - 006C
        C_TableData* m_pMusicTable;                 //006C - 0070
        C_TableData* m_pGlassBreakingTable;         //0070 - 0074
        C_TableData* m_pGlassMatTemplatesTable;     //0074 - 0078
        pad(ICTables, pad5, 0x4);                   //0078 - 007C
        C_TableData* m_pHumanDmgZonesTable;         //007C - 0080
        C_TableData* m_pPinupsGalleriesTable;       //0080 - 0084
        C_TableData* m_pPinupsTable;                //0084 - 0088
        pad(ICTables, pad6, 0x2C);                  //0088 - 00B4
        C_TableData* m_pRamboActionsTable;          //00B4 - 00B8
    };

    class C_Tables : public GameClassWrapper<C_Tables, ICTables, 0x1AB5A60>
    {
    public:
        int GetFrameColors(int frame)
        {
            return Mem::InvokeFunction<Mem::call_this, int>(0x4AA0A0, this, frame);
        }

        char *GetModelFileName(EntityTypes type, unsigned int modelID)
        {
            return Mem::InvokeFunction<Mem::call_this, char *>(0x478BF0, this, type, modelID);
        }
    };
}
