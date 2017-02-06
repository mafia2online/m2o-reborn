#include <stdafx.h>
#include "CTitleState.h"
#include <CGraphicsManager.h>

void CTitleState::InitializeResources(void *userptr)
{
	if (!userptr)
		return;

	CGraphicsManager *pGraphicsManager = reinterpret_cast<CGraphicsManager*>(userptr);
	CFontManager *pFontManager = pGraphicsManager->GetFontManager();
	if (!pFontManager)
		return;

	pFontManager->AddFont("TitleUIFont", "Aurora BdCn BT", 26, false);
	pFontManager->AddFont("TitleUIFontBig", "Aurora BdCn BT", 32, false);
	
}

void CTitleState::Render(void * userptr)
{
	if (!userptr)
		return;

	CGraphicsManager *pGraphicsManager = reinterpret_cast<CGraphicsManager*>(userptr);
	CFontManager *pFontManager = pGraphicsManager->GetFontManager();

	if (!pFontManager)
		return;

	int iScreenW{}, iScreenH{};
	pGraphicsManager->GetScreenDimensions(&iScreenW, &iScreenH);

	auto font = pFontManager->GetFont("TitleUIFont");
	auto iFontHeight = pFontManager->GetFontHeight(*font) + 5 ; // lawl +5

	if (font != nullptr)
	{



		int iYOffset = iScreenH - 50;

		pFontManager->DrawTextA("Q", 25, iYOffset, D3DCOLOR_XRGB(179, 48, 48), *font, true);
		pFontManager->DrawTextA("uit", 25 + pFontManager->GetTextWidthA(*font, "Q"), iYOffset, D3DCOLOR_XRGB(255, 255, 255), *font, true);
		iYOffset -= iFontHeight;

		pFontManager->DrawTextA("S", 25, iYOffset, D3DCOLOR_XRGB(179, 48, 48), *font, true);
		pFontManager->DrawTextA("ettings", 25 + pFontManager->GetTextWidthA(*font, "S"), iYOffset, D3DCOLOR_XRGB(255, 255, 255), *font, true);
		iYOffset -= iFontHeight;

		pFontManager->DrawTextA("F", 25, iYOffset, D3DCOLOR_XRGB(179, 48, 48), *font, true);
		pFontManager->DrawTextA("ind Servers", 25 + pFontManager->GetTextWidthA(*font, "F"), iYOffset, D3DCOLOR_XRGB(255, 255, 255), *font, true);
		iYOffset -= iFontHeight;

		pFontManager->DrawTextA("Q", 25, iYOffset, D3DCOLOR_XRGB(179, 48, 48), *font, true);
		pFontManager->DrawTextA("uick connect", 25 + pFontManager->GetTextWidthA(*font, "Q"), iYOffset, D3DCOLOR_XRGB(255, 255, 255), *font, true);
		iYOffset -= iFontHeight;

		pFontManager->DrawTextA("MAFIA II ", 25, iYOffset, D3DCOLOR_XRGB(179, 48, 48), *pFontManager->GetFont("TitleUIFontBig"), true);
		pFontManager->DrawTextA("Multiplayer", 25 + pFontManager->GetTextWidthA(*pFontManager->GetFont("TitleUIFontBig"), "MAFIA II "), iYOffset, D3DCOLOR_XRGB(255, 255, 255), *pFontManager->GetFont("TitleUIFontBig"), true);
	}
}
