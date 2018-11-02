/** @file CEntity.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CModel.hpp"

namespace M2
{
    class CEntityVFTable
    {
    public:
        DWORD Constructor;
        pad(CEntityVFTable, pad0, 0x78);
        DWORD SetPosition;
        DWORD SetDirection;
        DWORD SetRotation;
        DWORD m88;
        DWORD GetPosition;
        DWORD GetDirection;
        DWORD GetRotation;
        DWORD GetScale;
        pad(CEntityVFTable, pad1, 0x4);
        DWORD GetModel;
        pad(CEntityVFTable, pad2, 0xC);
        DWORD SetModel;
    };
    class ICEntity
    {
    public:
        CEntityVFTable  *m_pVFTable;            // 0000 - 0004
        pad(ICEntity, pad0, 0x18);              // 0004 - 001C
        DWORD           m_dwGUID;               // 001C - 0020
        DWORD           m_dwFlags;              // 0020 - 0024
        pad(ICEntity, pad1, 0x3C);              // 0024 - 0060
        C_Model         *m_pModel;              // 0060 - 0064
    };
    class C_Entity : public ICEntity
    {
    public:

        //TODO: Put in vTable
        void Destructor()
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x11A76D0, this);
        }

        void Activate()
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x1192170, this);
        }

        void Deactivate()
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x11921C0, this);
        }

        Vector3 GetPosition()
        {
            Vector3 position;
            Mem::InvokeFunction<Mem::call_this, void>(m_pVFTable->GetPosition, this, &position);
            return position;
        }

        Quaternion GetRotation()
        {
            Quaternion rotation;
            Mem::InvokeFunction<Mem::call_this, void>(m_pVFTable->GetRotation, this, &rotation);
            return rotation;
        }

        Vector3 GetDirection()
        {
            Vector3 direction;
            Mem::InvokeFunction<Mem::call_this, void>(m_pVFTable->GetDirection, this, &direction);
            return direction;
        }

        bool IsActive()
        {
            return (Mem::InvokeFunction<Mem::call_this, bool>(0x11665A0, this));
        }

        void SetModel(C_Model *model)
        {
            Mem::InvokeFunction<Mem::call_this, void*>(m_pVFTable->SetModel, this, model);
        }

        void SetPosition(Vector3 pos)
        {
            Mem::InvokeFunction<Mem::call_this, void>(m_pVFTable->SetPosition, this, &pos);
        }

        void SetRotation(Quaternion rot)
        {
            Mem::InvokeFunction<Mem::call_this, void>(m_pVFTable->SetRotation, this, &rot);
        }

        void SetDirection(Vector3 dir)
        {
            Mem::InvokeFunction<Mem::call_this, void>(m_pVFTable->SetDirection, this, &dir);
        }

        void SetName(const char *name)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x1192020, this, name);
        }

        void Release()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0x119DDD0, this);
        }

        void Setup()
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x99F400, this);
        }
    };
};
