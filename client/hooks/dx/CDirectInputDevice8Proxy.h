/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CDirectInputDevice8Proxy.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

enum eDIDeviceType
{
    DIDEVICE_TYPE_UNKNOWN,
    DIDEVICE_TYPE_MOUSE,
    DIDEVICE_TYPE_KEYBOARD,
    DIDEVICE_TYPE_JOYSTICK
};

interface CDirectInputDevice8Proxy : public IDirectInputDevice8
{
public:
    CDirectInputDevice8Proxy::CDirectInputDevice8Proxy(IDirectInputDevice8* pOriginal, eDIDeviceType devicetype);
    CDirectInputDevice8Proxy::~CDirectInputDevice8Proxy();

    HRESULT APIENTRY QueryInterface(REFIID iid, void ** ppvObject);
    ULONG APIENTRY AddRef(void);
    ULONG APIENTRY Release(void);
    HRESULT APIENTRY Acquire();
    HRESULT APIENTRY BuildActionMap(LPDIACTIONFORMAT lpdiaf, LPCTSTR lpszUserName, DWORD dwFlags);
    HRESULT APIENTRY CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT *ppdeff, LPUNKNOWN punkOuter);
    HRESULT APIENTRY EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl);
    HRESULT APIENTRY EnumEffects(LPDIENUMEFFECTSCALLBACK lpCallback, LPVOID pvRef, DWORD dwEffType);
    HRESULT APIENTRY EnumEffectsInFile(LPCSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags);
    HRESULT APIENTRY EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD dwFlags);
    HRESULT APIENTRY Escape(LPDIEFFESCAPE pesc);
    HRESULT APIENTRY GetCapabilities(LPDIDEVCAPS lpDIDevCaps);
    HRESULT APIENTRY GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags);
    HRESULT APIENTRY GetDeviceInfo(LPDIDEVICEINSTANCE pdidi);
    HRESULT APIENTRY GetDeviceState(DWORD cbData, LPVOID lpvData);
    HRESULT APIENTRY GetEffectInfo(LPDIEFFECTINFO pdei, REFGUID rguid);
    HRESULT APIENTRY GetForceFeedbackState(LPDWORD pdwOut);
    HRESULT APIENTRY GetImageInfo(LPDIDEVICEIMAGEINFOHEADER lpdiDevImageInfoHeader);
    HRESULT APIENTRY GetObjectInfo(LPDIDEVICEOBJECTINSTANCE pdidoi, DWORD dwObj, DWORD dwHow);
    HRESULT APIENTRY GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph);
    HRESULT APIENTRY Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid);
    HRESULT APIENTRY Poll();
    HRESULT APIENTRY RunControlPanel(HWND hwndOwner, DWORD dwFlags);
    HRESULT APIENTRY SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl);
    HRESULT APIENTRY SendForceFeedbackCommand(DWORD dwFlags);
    HRESULT APIENTRY SetActionMap(LPDIACTIONFORMAT lpdiActionFormat, LPCTSTR lptszUserName, DWORD dwFlags);
    HRESULT APIENTRY SetCooperativeLevel(HWND hwnd, DWORD dwFlags);
    HRESULT APIENTRY SetDataFormat(LPCDIDATAFORMAT lpdf);
    HRESULT APIENTRY SetEventNotification(HANDLE hEvent);
    HRESULT APIENTRY SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph);
    HRESULT APIENTRY Unacquire();
    HRESULT APIENTRY WriteEffectToFile(LPCSTR lpszFileName, DWORD dwEntries, LPDIFILEEFFECT rgDiFileEft, DWORD dwFlags);

private:
    IDirectInputDevice8 *m_pIDirectInputDevice8;
    eDIDeviceType m_DeviceType;
};

CDirectInputDevice8Proxy::CDirectInputDevice8Proxy(IDirectInputDevice8* pOriginal, eDIDeviceType devicetype)
{
    m_pIDirectInputDevice8 = pOriginal;
    m_DeviceType = devicetype;
}

CDirectInputDevice8Proxy::~CDirectInputDevice8Proxy() { }

HRESULT APIENTRY CDirectInputDevice8Proxy::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
{
    return m_pIDirectInputDevice8->GetDeviceData(cbObjectData, rgdod, pdwInOut, dwFlags);
}

ULONG APIENTRY CDirectInputDevice8Proxy::Release(void)
{
    return m_pIDirectInputDevice8->Release();
}

HRESULT APIENTRY CDirectInputDevice8Proxy::QueryInterface(REFIID iid, void ** ppvObject)
{
    return m_pIDirectInputDevice8->QueryInterface(iid, ppvObject);
}

ULONG APIENTRY CDirectInputDevice8Proxy::AddRef(void)
{
    return m_pIDirectInputDevice8->AddRef();
}

HRESULT APIENTRY CDirectInputDevice8Proxy::Acquire()
{
    return m_pIDirectInputDevice8->Acquire();
}

HRESULT APIENTRY CDirectInputDevice8Proxy::BuildActionMap(LPDIACTIONFORMAT lpdiaf, LPCTSTR lpszUserName, DWORD dwFlags)
{
    return m_pIDirectInputDevice8->BuildActionMap(lpdiaf, lpszUserName, dwFlags);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT *ppdeff, LPUNKNOWN punkOuter)
{
    return m_pIDirectInputDevice8->CreateEffect(rguid, lpeff, ppdeff, punkOuter);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl)
{
    return m_pIDirectInputDevice8->EnumCreatedEffectObjects(lpCallback, pvRef, fl);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::EnumEffects(LPDIENUMEFFECTSCALLBACK lpCallback, LPVOID pvRef, DWORD dwEffType)
{
    return m_pIDirectInputDevice8->EnumEffects(lpCallback, pvRef, dwEffType);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::EnumEffectsInFile(LPCSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags)
{
    return m_pIDirectInputDevice8->EnumEffectsInFile(lpszFileName, pec, pvRef, dwFlags);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD dwFlags)
{
    return m_pIDirectInputDevice8->EnumObjects(lpCallback, pvRef, dwFlags);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::Escape(LPDIEFFESCAPE pesc)
{
    return m_pIDirectInputDevice8->Escape(pesc);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::GetCapabilities(LPDIDEVCAPS lpDIDevCaps)
{
    return m_pIDirectInputDevice8->GetCapabilities(lpDIDevCaps);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::GetDeviceInfo(LPDIDEVICEINSTANCE pdidi)
{
    return m_pIDirectInputDevice8->GetDeviceInfo(pdidi);
}

struct mod_di_keys_t {
    byte state[256];
};

HRESULT APIENTRY CDirectInputDevice8Proxy::GetDeviceState(DWORD cbData, LPVOID lpvData)
{
    HRESULT hResult = m_pIDirectInputDevice8->GetDeviceState(cbData, lpvData);

    if (FAILED(hResult)) {
        hResult = m_pIDirectInputDevice8->Acquire();

        while (hResult == DIERR_INPUTLOST)
            hResult = m_pIDirectInputDevice8->Acquire();

        if (FAILED(hResult))
            return hResult;

        hResult = m_pIDirectInputDevice8->GetDeviceState(cbData, lpvData);
    }

    if (hResult == DI_OK) {
        if (m_DeviceType == DIDEVICE_TYPE_MOUSE) {
            mod.mouse.state = *(DIMOUSESTATE*)lpvData;

            mod.mouse.x += mod.mouse.state.lX;
            mod.mouse.y += mod.mouse.state.lY;

            int screenWidth, screenHeight;
            graphics_dimensions(&screenWidth, &screenHeight);

            if (mod.mouse.x <= 0)
                mod.mouse.x = 0;
            if (mod.mouse.x > screenWidth)
                mod.mouse.x = screenWidth;
            if (mod.mouse.y <= 0)
                mod.mouse.y = 0;
            if (mod.mouse.y > screenHeight)
                mod.mouse.y = screenHeight;

            for (usize i = 0; i < 3; i++) {
                auto button = &mod.mouse.buttons[i];
                auto btn_id = NK_BUTTON_LEFT;

                if (i == 1) btn_id = NK_BUTTON_RIGHT;
                if (i == 2) btn_id = NK_BUTTON_MIDDLE;

                button->id    = btn_id;
                button->state = (mod.mouse.state.rgbButtons[i]) ? 1 : 0;
            }

            if (mod.input_blocked) {
                ((DIMOUSESTATE*)lpvData)->lX = 0;
                ((DIMOUSESTATE*)lpvData)->lY = 0;

                for (usize i = 0; i < 4; i++) {
                    ((DIMOUSESTATE*)lpvData)->rgbButtons[i] = 0;
                }
            }
        }

        if (nk_ctx && m_DeviceType == DIDEVICE_TYPE_KEYBOARD) {
            if (mod.input_blocked && nk_ctx->active && nk_ctx->active->edit.active) {
                for (usize i = 0; i < 256; i++) {
                    ((mod_di_keys_t*)lpvData)->state[i] = 0;
                }
            }
        }
    }

    return hResult;
}

HRESULT APIENTRY CDirectInputDevice8Proxy::GetEffectInfo(LPDIEFFECTINFO pdei, REFGUID rguid)
{
    return m_pIDirectInputDevice8->GetEffectInfo(pdei, rguid);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::GetForceFeedbackState(LPDWORD pdwOut)
{
    return m_pIDirectInputDevice8->GetForceFeedbackState(pdwOut);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::GetImageInfo(LPDIDEVICEIMAGEINFOHEADER lpdiDevImageInfoHeader)
{
    return m_pIDirectInputDevice8->GetImageInfo(lpdiDevImageInfoHeader);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::GetObjectInfo(LPDIDEVICEOBJECTINSTANCE pdidoi, DWORD dwObj, DWORD dwHow)
{
    return m_pIDirectInputDevice8->GetObjectInfo(pdidoi, dwObj, dwHow);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph)
{
    return m_pIDirectInputDevice8->GetProperty(rguidProp, pdiph);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid)
{
    return m_pIDirectInputDevice8->Initialize(hinst, dwVersion, rguid);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::Poll()
{
    return m_pIDirectInputDevice8->Poll();
}

HRESULT APIENTRY CDirectInputDevice8Proxy::RunControlPanel(HWND hwndOwner, DWORD dwFlags)
{
    return m_pIDirectInputDevice8->RunControlPanel(hwndOwner, dwFlags);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl)
{
    return m_pIDirectInputDevice8->SendDeviceData(cbObjectData, rgdod, pdwInOut, fl);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::SendForceFeedbackCommand(DWORD dwFlags)
{
    return m_pIDirectInputDevice8->SendForceFeedbackCommand(dwFlags);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::SetActionMap(LPDIACTIONFORMAT lpdiActionFormat, LPCTSTR lptszUserName, DWORD dwFlags)
{
    return m_pIDirectInputDevice8->SetActionMap(lpdiActionFormat, lptszUserName, dwFlags);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::SetCooperativeLevel(HWND hwnd, DWORD dwFlags)
{
    DWORD dwCustomFlags = dwFlags;

    if (m_DeviceType == DIDEVICE_TYPE_MOUSE) {
        mod_assert(hwnd);

        // idk wtf is dis, but also kinda helps ?
        // mod_log("dxinput: setting non-exclusive\n");
        // if ((dwCustomFlags & DISCL_EXCLUSIVE) != 0) {
        //     dwCustomFlags &= ~(DISCL_EXCLUSIVE);
        // }
        // dwCustomFlags |= DISCL_NONEXCLUSIVE;

        // // enabled native-feel mouse
        // // good for windwed mode, with cursor showing
        // mod_log("dxinput: setting background mode\n");
        // if ((dwCustomFlags & DISCL_FOREGROUND) != 0) {
        //     dwCustomFlags &= ~(DISCL_FOREGROUND);
        // }
        // dwCustomFlags |= DISCL_BACKGROUND;

        // TODO: move wndproc hook into here maybe
        // wndproc_install(hwnd);
    }

    return m_pIDirectInputDevice8->SetCooperativeLevel(hwnd, dwCustomFlags);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::SetDataFormat(LPCDIDATAFORMAT lpdf)
{
    return m_pIDirectInputDevice8->SetDataFormat(lpdf);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::SetEventNotification(HANDLE hEvent)
{
    return m_pIDirectInputDevice8->SetEventNotification(hEvent);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph)
{
    return m_pIDirectInputDevice8->SetProperty(rguidProp, pdiph);
}

HRESULT APIENTRY CDirectInputDevice8Proxy::Unacquire()
{
    return m_pIDirectInputDevice8->Unacquire();
}

HRESULT APIENTRY CDirectInputDevice8Proxy::WriteEffectToFile(LPCSTR lpszFileName, DWORD dwEntries, LPDIFILEEFFECT rgDiFileEft, DWORD dwFlags)
{
    return m_pIDirectInputDevice8->WriteEffectToFile(lpszFileName, dwEntries, rgDiFileEft, dwFlags);
}
