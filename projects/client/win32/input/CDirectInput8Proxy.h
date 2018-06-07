/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CDirectInput8Proxy.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

class CDirectInput8Proxy : public IDirectInput8
{
private:
    IDirectInput8A *    m_pDevice;

public:
    CDirectInput8Proxy(IDirectInput8 * dinput);

    /*** IUnknown methods ***/
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID * ppvObj);
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();

    /*** IDirectInput8A methods ***/
    HRESULT STDMETHODCALLTYPE CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8A * lplpDirectInputDevice, LPUNKNOWN pUknOuter);
    HRESULT STDMETHODCALLTYPE EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags);
    HRESULT STDMETHODCALLTYPE GetDeviceStatus(REFGUID rguidInstance);
    HRESULT STDMETHODCALLTYPE RunControlPanel(HWND hwndOwner, DWORD dwFlags);
    HRESULT STDMETHODCALLTYPE Initialize(HINSTANCE hinst, DWORD dwVersion);
    HRESULT STDMETHODCALLTYPE FindDevice(REFGUID rguidClass, LPCSTR ptszName, LPGUID pguidInstance);
    HRESULT STDMETHODCALLTYPE EnumDevicesBySemantics(LPCSTR ptszUserName, LPDIACTIONFORMATA lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCBA lpCallback, LPVOID pvRef, DWORD dwFlags);
    HRESULT STDMETHODCALLTYPE ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMSA lpdiCDParams, DWORD dwFlags, LPVOID pvRefData);
};


CDirectInput8Proxy::CDirectInput8Proxy(IDirectInput8 * dinput) {
    m_pDevice = dinput;
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::QueryInterface(REFIID riid, LPVOID * ppvObj) {
    return m_pDevice->QueryInterface(riid, ppvObj);
}

ULONG STDMETHODCALLTYPE CDirectInput8Proxy::AddRef() {
    return m_pDevice->AddRef();
}

ULONG STDMETHODCALLTYPE CDirectInput8Proxy::Release() {
    ULONG uRet = m_pDevice->Release();

    if ( uRet == 0 )
        delete this;

    return uRet;
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8A * lplpDirectInputDevice, LPUNKNOWN pUknOuter) {
    HRESULT hResult = m_pDevice->CreateDevice( rguid, lplpDirectInputDevice, pUknOuter );

    if ( hResult == DI_OK )
    {
        eDIDeviceType type = DIDEVICE_TYPE_UNKNOWN;

        if(rguid == GUID_SysMouse)
            type = DIDEVICE_TYPE_MOUSE;
        else if(rguid == GUID_SysKeyboard)
            type = DIDEVICE_TYPE_KEYBOARD;
        else if(rguid == GUID_Joystick)
            type = DIDEVICE_TYPE_JOYSTICK;

        *lplpDirectInputDevice = new CDirectInputDevice8Proxy( *lplpDirectInputDevice, type );
    }

    return hResult;
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags) {
    return m_pDevice->EnumDevices(dwDevType, lpCallback, pvRef, dwFlags);
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::GetDeviceStatus(REFGUID rguidInstance) {
    return m_pDevice->GetDeviceStatus(rguidInstance);
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::RunControlPanel(HWND hwndOwner, DWORD dwFlags) {
    return m_pDevice->RunControlPanel(hwndOwner, dwFlags);
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::Initialize(HINSTANCE hinst, DWORD dwVersion) {
    return m_pDevice->Initialize(hinst, dwVersion);
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::FindDevice(REFGUID rguidClass, LPCSTR ptszName, LPGUID pguidInstance) {
    return m_pDevice->FindDevice(rguidClass, ptszName, pguidInstance);
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::EnumDevicesBySemantics(LPCSTR ptszUserName, LPDIACTIONFORMATA lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCBA lpCallback, LPVOID pvRef, DWORD dwFlags) {
    return m_pDevice->EnumDevicesBySemantics(ptszUserName, lpdiActionFormat, lpCallback, pvRef, dwFlags);
}

HRESULT STDMETHODCALLTYPE CDirectInput8Proxy::ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMSA lpdiCDParams, DWORD dwFlags, LPVOID pvRefData) {
    return m_pDevice->ConfigureDevices(lpdiCallback, lpdiCDParams, dwFlags, pvRefData);
}
