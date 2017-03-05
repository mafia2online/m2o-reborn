#pragma once
#include <CFontManager.h>
#include <GwenManager.h>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

class CGraphicsManager 
{
public:
	CGraphicsManager();
	~CGraphicsManager();
	bool Init();

public:
	void OnDeviceCreate(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	void OnDeviceLost(IDirect3DDevice9* pDevice);
	void OnDeviceReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	void OnDevicePreRender(void);
	void OnDeviceRender(void);

	CFontManager* GetFontManager() { return m_pfontmanager; }
	GwenManager* GetGwenManager() { return m_pGwenManager; }


	void GetScreenDimensions(int *w, int *h)
	{
		*w = static_cast<int>(m_presentparams.BackBufferWidth);
		*h = static_cast<int>(m_presentparams.BackBufferHeight);
	}

private:
	GwenManager* m_pGwenManager;
	CFontManager* m_pfontmanager;
	IDirect3DDevice9* m_pdevice;
	D3DPRESENT_PARAMETERS m_presentparams;
};