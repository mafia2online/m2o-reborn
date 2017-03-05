/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CDirectInputDevice8Proxy.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include "CDirectInput8Hook.h"
#include "CDirectInput8Proxy.h"

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
