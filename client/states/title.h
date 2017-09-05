void title_state_init()
{
    if (!mod.graphics.font_manager) {
        return;
    }

    auto fm = (CFontManager *)mod.graphics.font_manager;

    fm->AddFont("TitleUIFont", "Aurora BdCn BT", 26, false);
    fm->AddFont("TitleUIFontBig", "Aurora BdCn BT", 32, false);
}

void title_state_render()
{
    if (!mod.graphics.font_manager) {
        return;
    }

    auto fm = (CFontManager *)mod.graphics.font_manager;

    int iScreenW{}, iScreenH{};
    graphics_dimensions(&iScreenW, &iScreenH);

    auto font = fm->GetFont("TitleUIFont");
    int iFontHeight = (int)fm->GetFontHeight(*font) + 5; // lawl +5

    if (font != nullptr) {
        int iYOffset = iScreenH - 50;

        fm->DrawTextA("Q", 25, iYOffset, D3DCOLOR_XRGB(179, 48, 48), *font, true);
        fm->DrawTextA("uit", 25 + (int)fm->GetTextWidthA(*font, "Q"), iYOffset, D3DCOLOR_XRGB(255, 255, 255), *font, true);
        iYOffset -= iFontHeight;

        fm->DrawTextA("S", 25, iYOffset, D3DCOLOR_XRGB(179, 48, 48), *font, true);
        fm->DrawTextA("ettings", 25 + (int)fm->GetTextWidthA(*font, "S"), iYOffset, D3DCOLOR_XRGB(255, 255, 255), *font, true);
        iYOffset -= iFontHeight;

        fm->DrawTextA("F", 25, iYOffset, D3DCOLOR_XRGB(179, 48, 48), *font, true);
        fm->DrawTextA("ind Servers", 25 + (int)fm->GetTextWidthA(*font, "F"), iYOffset, D3DCOLOR_XRGB(255, 255, 255), *font, true);
        iYOffset -= iFontHeight;

        fm->DrawTextA("Q", 25, iYOffset, D3DCOLOR_XRGB(179, 48, 48), *font, true);
        fm->DrawTextA("uick connect", 25 + (int)fm->GetTextWidthA(*font, "Q"), iYOffset, D3DCOLOR_XRGB(255, 255, 255), *font, true);
        iYOffset -= iFontHeight;

        fm->DrawTextA("MAFIA II ", 25, iYOffset, D3DCOLOR_XRGB(179, 48, 48), *fm->GetFont("TitleUIFontBig"), true);
        fm->DrawTextA("Online", 25 + (int)fm->GetTextWidthA(*fm->GetFont("TitleUIFontBig"), "MAFIA II "), iYOffset, D3DCOLOR_XRGB(255, 255, 255), *fm->GetFont("TitleUIFontBig"), true);
    }
}

#define MOD_TITLE_STATE { title_state_init, nullptr, title_state_render }
