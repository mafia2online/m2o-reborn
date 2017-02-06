/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CDirect3D9Hook.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

#pragma once

#include <d3d9.h>

class CDirect3D9Hook
{

private:

	typedef	IDirect3D9 * (WINAPI * Direct3DCreate9_t)( UINT );
	static	Direct3DCreate9_t			m_pfnDirect3DCreate9;

	static	IDirect3D9 * WINAPI		Direct3DCreate9_Hook(UINT SDKVersion);

	static	bool					m_bInstalled;

public:

	static	bool					Install( void );
	static	void					Uninstall( void );

};
