#pragma once
#include <unordered_map>
#include <d3d9.h>
#include <d3dx9.h>

struct Font
{
	SString face;
	uint16_t size;
	bool bold;

	ID3DXFont *pfont;
};

class CFontManager
{
public:
	CFontManager(IDirect3DDevice9* pDevice);
	~CFontManager();

	bool AddFont(SString strIdentifier, SString strFontFace, uint16_t size, bool bold);
	Font* GetFont(SString strIdentifier) {

		const auto& font = m_fontmap.find(strIdentifier.GetSTLString());

		if (font != m_fontmap.end())
			return &font->second;
		else
			return nullptr;
	}

	void DrawTextA(const char * szText, int iX, int iY, unsigned long ulColor, Font & font, bool bShadow);
	void DrawTextW(const wchar_t * szText, int iX, int iY, unsigned long ulColor, Font & font, bool bShadow);

	void DrawTextA(const char * szText, int iX, int iY, unsigned long ulColor, const char *szFont, bool bShadow);
	void DrawTextW(const wchar_t * szText, int iX, int iY, unsigned long ulColor, const char *szFont, bool bShadow);

	void OnDeviceLost();
	void OnDeviceReset();

	float GetTextWidthA(Font& font, const char *szText)
	{
		if (!font.pfont)
			return 0.0f;

		HDC dc = font.pfont->GetDC();
		SIZE size;
		GetTextExtentPoint32A(dc, szText, strlen(szText), &size);
		return static_cast<float>(size.cx);
	}

	float GetTextWidthW(Font& font, const wchar_t *szText)
	{
		if (!font.pfont)
			return 0.0f;

		HDC dc = font.pfont->GetDC();
		SIZE size;
		GetTextExtentPoint32W(dc, szText, wcslen(szText), &size);
		return static_cast<float>(size.cx);
	}

	float GetFontHeight(Font& font)
	{
		if (!font.pfont)
			return 0.0f;

		D3DXFONT_DESC desc;
		font.pfont->GetDesc(&desc);
		return static_cast<float>(desc.Height);
	}

private:
	void DrawTextA(Font& font, int uiLeft, int uiTop, int uiRight, int uiBottom, unsigned long ulColor, const char * szText, float fScaleX, float fScaleY, unsigned long ulFormat, bool bShadow);
	void DrawTextW(Font& font, int uiLeft, int uiTop, int uiRight, int uiBottom, unsigned long ulColor, const wchar_t * szText, float fScaleX, float fScaleY, unsigned long ulFormat, bool bShadow);



private:
	IDirect3DDevice9 *m_pdevice;
	ID3DXSprite *m_pdxsprite;
	SString m_strDefaultFont;
	std::unordered_map<std::string, Font> m_fontmap;
};