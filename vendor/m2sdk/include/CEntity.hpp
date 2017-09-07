/** @file CEntity.hpp
*  @brief Game's Entity class
*
*
*  @author Tyldar (darreugne@gmail.com)
*/

#pragma once
#include "CommonHeaders.h"
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
        CEntityVFTable  * m_pVFTable;
    };
    class C_Entity : public ICEntity
    {
    public:

        void Activate()
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x1192170, this);
        }

        void Deactivate()
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x11921C0, this);
        }

        bool IsActive()
        {
            return (Mem::InvokeFunction<Mem::call_this, bool>(0x11665A0, this));
        }

        void SetModel(C_Model *model)
        {
            Mem::InvokeFunction<Mem::call_this, void*>(m_pVFTable->SetModel, this, model);
        }

        void SetPosition(vec3_t pos)
        {
            Mem::InvokeFunction<Mem::call_this, void>(m_pVFTable->SetPosition, this, &pos);
        }

        void SetRotation(quat_t rot)
        {
            Mem::InvokeFunction<Mem::call_this, void>(m_pVFTable->SetRotation, this, &rot);
        }

        void SetDirection(vec3_t dir)
        {
            Mem::InvokeFunction<Mem::call_this, void>(m_pVFTable->SetDirection, this, &dir);
        }

        vec3_t GetPosition()
        {
            vec3_t position;
            Mem::InvokeFunction<Mem::call_this, void>(m_pVFTable->GetPosition, this, &position);
            return position;
        }

        quat_t GetRotation()
        {
            quat_t rotation;
            Mem::InvokeFunction<Mem::call_this, void>(m_pVFTable->GetRotation, this, &rotation);
            return rotation;
        }

        vec3_t GetDirection()
        {
            vec3_t direction;
            Mem::InvokeFunction<Mem::call_this, void>(m_pVFTable->GetDirection, this, &direction);
            return direction;
        }

        void Setup()
        {
            Mem::InvokeFunction<Mem::call_this, void>(0x99F400, this);
        }
    };
};
