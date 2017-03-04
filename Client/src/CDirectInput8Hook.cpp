#include <stdafx.h>
#include <Shared\Client\CPatcher.h>

#include "CDirectInput8Hook.h"
#include "CDirectInput8Proxy.h"

bool								CDirectInput8Hook::m_bInstalled = false;
DirectInput8Create_t				CDirectInput8Hook::m_pDirectInput8Create = NULL;
IDirectInput8*						g_pDirectInput8 = NULL;

HRESULT WINAPI DirectInput8Create__Hook( HINSTANCE hInst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter )
{
	HRESULT hr = CDirectInput8Hook::m_pDirectInput8Create( hInst, dwVersion, riidltf, ppvOut, punkOuter );
	if( SUCCEEDED( hr ) )
	{
		IDirectInput8 * pInput = static_cast<IDirectInput8 *> (*ppvOut);

		CDirectInput8Proxy * proxy = new CDirectInput8Proxy(pInput);
		proxy->AddRef();

		g_pDirectInput8 = proxy;
		*ppvOut = proxy;
	}

	return hr;
}

void CDirectInput8Hook::Install( )
{
	if( !m_bInstalled )
	{
		m_pDirectInput8Create = reinterpret_cast<DirectInput8Create_t>(Mem::Hooks::InstallDetourPatch("dinput8.dll", "DirectInput8Create", (DWORD)DirectInput8Create__Hook));
		m_bInstalled = true;
	}
}

void CDirectInput8Hook::Uninstall( )
{
	if( m_bInstalled )
	{
		Mem::Hooks::UninstallDetourPatch( m_pDirectInput8Create, (DWORD)DirectInput8Create__Hook );
		m_bInstalled = false;
	}
}
