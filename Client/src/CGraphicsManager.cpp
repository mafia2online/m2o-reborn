#include <stdafx.h>
#include <CCore.h>
#include <CDirect3D9Hook.h>
#include "CGraphicsManager.h"

CGraphicsManager::CGraphicsManager()
{
	// derp
}

CGraphicsManager::~CGraphicsManager()
{
	CDirect3D9Hook::Uninstall();
}

bool CGraphicsManager::Init()
{
	CCore::Instance().GetLogger().Writeln("CGraphicsManager::Init()");
	CDirect3D9Hook::Install();
	return true;
}

void CGraphicsManager::OnDeviceCreate(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters)
{
	CCore::Instance().GetLogger().Writeln("CGraphicsManager::OnDeviceCreate(%x, %x)", pDevice, pPresentationParameters);
	m_pfontmanager = new CFontManager(pDevice);

	m_pdevice = pDevice;
	memcpy(&m_presentparams, pPresentationParameters, sizeof(D3DPRESENT_PARAMETERS));

	CCore::Instance().GetStateManager().InitializeResources(this);
}

void CGraphicsManager::OnDeviceLost(IDirect3DDevice9 * pDevice)
{
	CCore::Instance().GetLogger().Writeln("CGraphicsManager::OnDeviceLost(%x)", pDevice);

	if (m_pfontmanager)
		m_pfontmanager->OnDeviceLost();
}

void CGraphicsManager::OnDeviceReset(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters)
{
	CCore::Instance().GetLogger().Writeln("CGraphicsManager::OnDeviceReset(%x, %x)", pDevice, pPresentationParameters);

	m_pdevice = pDevice;
	memcpy(&m_presentparams, pPresentationParameters, sizeof(D3DPRESENT_PARAMETERS));

	if (m_pfontmanager)
		m_pfontmanager->OnDeviceReset();
}

void CGraphicsManager::OnDevicePreRender(void)
{
}

void CGraphicsManager::OnDeviceRender(void)
{
	CCore::Instance().GetStateManager().Render(this);
}
