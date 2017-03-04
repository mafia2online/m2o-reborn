#pragma once
#include <Gwen/Gwen.h>
#include <Gwen/Skins/Simple.h>
#include <Gwen/Skins/TexturedBase.h>
#include <Gwen/Controls/Base.h>
#include <Gwen/Input/Windows.h>
#include <Gwen/Renderers/DirectX9.h>

class GwenManager
{
public:
	GwenManager(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters);

	void OnDeviceRender();
	Gwen::Input::Windows& GetInput() { return m_gweninput; }
	Gwen::Controls::Canvas* GetCanvas() { return m_pcanvas; }

private:
	IDirect3DDevice9 *m_pdevice;

private:
	Gwen::Renderer::DirectX9* m_prenderer;
	Gwen::Skin::TexturedBase* m_pskin;
	Gwen::Controls::Canvas* m_pcanvas;
	Gwen::Input::Windows m_gweninput;
};