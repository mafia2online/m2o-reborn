/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CDirectInput8Hook.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

typedef HRESULT		( WINAPI * DirectInput8Create_t ) ( HINSTANCE, DWORD, REFIID, LPVOID *, LPUNKNOWN );

class CDirectInput8Hook
{
	
private:

	static	bool						m_bInstalled;

public:

	static	DirectInput8Create_t		m_pDirectInput8Create;

	static	void						Install				( void );
	static	void						Uninstall			( void );

};
