#include <stdafx.h>
#include <CCore.h>

CFontManager::CFontManager(IDirect3DDevice9* pDevice) : m_pdevice(pDevice)
{
	D3DXCreateSprite(m_pdevice, &m_pdxsprite);
}

CFontManager::~CFontManager()
{
}

bool CFontManager::AddFont(SString strIdentifier, SString strFontFace, uint16_t size, bool bold)
{
	const char *szIdentifier = strIdentifier.GetCStr();
	if (m_fontmap.find(szIdentifier) == m_fontmap.end())
	{
		Font &info = m_fontmap[szIdentifier] = Font{ strFontFace, size, bold, nullptr };
		if (!SUCCEEDED(D3DXCreateFont(m_pdevice, info.size, 0, (info.bold ? FW_BOLD : FW_NORMAL), 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, (DEFAULT_PITCH | FF_DONTCARE), info.face.GetCStr(), &info.pfont)))
		{
			info.pfont = nullptr;
			return false;
		}

		return true;
	}

	return true;
}

void CFontManager::DrawTextA(const char* szText, int iX, int iY, unsigned long ulColor, Font& font, bool bShadow)
{
	DrawTextA(font, iX, iY, iX, iY, ulColor, szText, 1.0f, 1.0f, DT_NOCLIP, bShadow);
}

void CFontManager::DrawTextW(const wchar_t* szText, int iX, int iY, unsigned long ulColor, Font& font, bool bShadow)
{
	DrawTextW(font, iX, iY, iX, iY, ulColor, szText, 1.0f, 1.0f, DT_NOCLIP, bShadow);
}

void CFontManager::DrawTextA(const char* szText, int iX, int iY, unsigned long ulColor, const char *szFont, bool bShadow)
{
	const auto& font = m_fontmap.find(szFont);

	if(font != m_fontmap.end())
		DrawTextA(font->second, iX, iY, iX, iY, ulColor, szText, 1.0f, 1.0f, DT_NOCLIP, bShadow);
}

void CFontManager::DrawTextW(const wchar_t* szText, int iX, int iY, unsigned long ulColor, const char *szFont, bool bShadow)
{
	const auto& font = m_fontmap.find(szFont);

	if (font != m_fontmap.end())
		DrawTextW(font->second, iX, iY, iX, iY, ulColor, szText, 1.0f, 1.0f, DT_NOCLIP, bShadow);
}

void CFontManager::DrawTextA(Font& font, int uiLeft, int uiTop, int uiRight, int uiBottom, unsigned long ulColor, const char *szText, float fScaleX, float fScaleY, unsigned long ulFormat, bool bShadow)
{
	if (!m_pdevice || !m_pdxsprite || !szText)
		return;

	uiLeft = unsigned int((float)uiLeft * (1.0f / fScaleX));
	uiTop = unsigned int((float)uiTop * (1.0f / fScaleY));
	uiRight = unsigned int((float)uiRight * (1.0f / fScaleX));
	uiBottom = unsigned int((float)uiBottom * (1.0f / fScaleY));

	RECT rect;
	SetRect(&rect, uiLeft, uiTop, uiRight, uiBottom);

	D3DXMATRIX matrix;
	D3DXVECTOR2 scaling(fScaleX, fScaleY);

	m_pdxsprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, &scaling, NULL, 0.0f, NULL);
	m_pdxsprite->SetTransform(&matrix);

	if (bShadow)
	{
		RECT shadow_rect{};
		SetRect(&shadow_rect, uiLeft, uiTop, uiRight, uiBottom);
		/*SetRect(&shadow_rect, uiLeft + 1, uiTop + 1, uiRight + 1, uiBottom + 1);
		m_pFont->DrawTextA(pDXSprite, szText, -1, &shadow_rect, ulFormat, D3DCOLOR_ARGB(255, 0, 0, 0));*/


		// above
		shadow_rect.top -= 1;
		font.pfont->DrawText(m_pdxsprite, szText, -1, &shadow_rect, ulFormat, 0xFF000000);
		shadow_rect.top += 1;

		// below
		shadow_rect.top += 1;
		font.pfont->DrawText(m_pdxsprite, szText, -1, &shadow_rect, ulFormat, 0xFF000000);
		shadow_rect.top -= 1;

		// left
		shadow_rect.left -= 1;
		font.pfont->DrawText(m_pdxsprite, szText, -1, &shadow_rect, ulFormat, 0xFF000000);
		shadow_rect.left += 1;

		// right
		shadow_rect.left += 1;
		font.pfont->DrawText(m_pdxsprite, szText, -1, &shadow_rect, ulFormat, 0xFF000000);
		shadow_rect.left -= 1;
	}

	font.pfont->DrawTextA(m_pdxsprite, szText, -1, &rect, ulFormat, ulColor);

	m_pdxsprite->End();
}

void CFontManager::DrawTextW(Font& font, int uiLeft, int uiTop, int uiRight, int uiBottom, unsigned long ulColor, const wchar_t *szText, float fScaleX, float fScaleY, unsigned long ulFormat, bool bShadow)
{
	if (!m_pdevice || !m_pdxsprite || !szText)
		return;

	uiLeft = unsigned int((float)uiLeft * (1.0f / fScaleX));
	uiTop = unsigned int((float)uiTop * (1.0f / fScaleY));
	uiRight = unsigned int((float)uiRight * (1.0f / fScaleX));
	uiBottom = unsigned int((float)uiBottom * (1.0f / fScaleY));

	RECT rect;
	SetRect(&rect, uiLeft, uiTop, uiRight, uiBottom);

	D3DXMATRIX matrix;
	D3DXVECTOR2 scaling(fScaleX, fScaleY);

	m_pdxsprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, &scaling, NULL, 0.0f, NULL);
	m_pdxsprite->SetTransform(&matrix);

	if (bShadow)
	{
		RECT shadow_rect;
		SetRect(&shadow_rect, uiLeft + 1, uiTop + 1, uiRight + 1, uiBottom + 1);

		font.pfont->DrawTextW(m_pdxsprite, szText, -1, &shadow_rect, ulFormat, D3DCOLOR_ARGB(255, 0, 0, 0));
	}

	font.pfont->DrawTextW(m_pdxsprite, szText, -1, &rect, ulFormat, ulColor);

	m_pdxsprite->End();
}

void CFontManager::OnDeviceLost()
{
	for (auto &font : m_fontmap)
	{
		if (font.second.pfont)
			font.second.pfont->OnLostDevice();
	}

	if (m_pdxsprite)
		m_pdxsprite->OnLostDevice();
}

void CFontManager::OnDeviceReset()
{
	for (auto &font : m_fontmap)
	{
		if (font.second.pfont)
			font.second.pfont->OnResetDevice();
	}

	if (m_pdxsprite)
		m_pdxsprite->OnResetDevice();
}

