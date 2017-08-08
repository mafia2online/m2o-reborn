/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CDirect3D9Hook.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

class CDirect3D9Hook
{

private:

    typedef IDirect3D9 * (WINAPI * Direct3DCreate9_t)( UINT );
    static  Direct3DCreate9_t           m_pfnDirect3DCreate9;

    static  IDirect3D9 * WINAPI     Direct3DCreate9_Hook(UINT SDKVersion);

    static  bool                    m_bInstalled;

public:

    static  bool                    Install( void );
    static  void                    Uninstall( void );

};

bool                                CDirect3D9Hook::m_bInstalled = false;
CDirect3D9Hook::Direct3DCreate9_t   CDirect3D9Hook::m_pfnDirect3DCreate9 = NULL;

IDirect3D9 * WINAPI CDirect3D9Hook::Direct3DCreate9_Hook( UINT SDKVersion )
{
    // Call the real device
    IDirect3D9 * pD3D = m_pfnDirect3DCreate9( SDKVersion );

    // If the call was successful return our proxy device
    if ( pD3D )
    {
        return new CDirect3D9Proxy( pD3D );
    }

    MessageBox ( NULL, "Unable to create Direct3D9 interface.", "Fatal error", MB_ICONERROR );
    TerminateProcess( GetCurrentProcess(), 0 );

    return NULL;
}

bool CDirect3D9Hook::Install( void )
{
    if( !m_bInstalled )
    {
        m_pfnDirect3DCreate9 = reinterpret_cast<Direct3DCreate9_t>(Mem::Hooks::InstallDetourPatch( "d3d9.dll", "Direct3DCreate9", (DWORD)Direct3DCreate9_Hook ));
        return m_bInstalled = true;
    }
    return false;
}

void CDirect3D9Hook::Uninstall( void )
{
    if( m_bInstalled )
    {
        Mem::Hooks::UninstallDetourPatch( m_pfnDirect3DCreate9, (DWORD)Direct3DCreate9_Hook );
    }
}
