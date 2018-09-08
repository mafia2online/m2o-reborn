/*************************************************************
*
* Solution   : Mafia 2 Multiplayer
* Project    : Client
* File       : CDirect3DDevice9Proxy.h
* Developers : AaronLad <aaron@m2-multiplayer.com>
*
***************************************************************/

class CDirect3DDevice9Proxy : public IDirect3DDevice9
{
private:

    IDirect3D9          * m_pD3D;
    IDirect3DDevice9    * m_pD3DDevice;

public:

    CDirect3DDevice9Proxy(IDirect3D9 * pD3D, IDirect3DDevice9 * pDevice);

    /*** IUnknown methods ***/
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObj);
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();

    /*** IDirect3DDevice9 methods ***/
    HRESULT STDMETHODCALLTYPE TestCooperativeLevel();
    UINT STDMETHODCALLTYPE GetAvailableTextureMem();
    HRESULT STDMETHODCALLTYPE EvictManagedResources();
    HRESULT STDMETHODCALLTYPE GetDirect3D(IDirect3D9 ** ppD3D9);
    HRESULT STDMETHODCALLTYPE GetDeviceCaps(D3DCAPS9 * pCaps);
    HRESULT STDMETHODCALLTYPE GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE * pMode);
    HRESULT STDMETHODCALLTYPE GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS * pParameters);
    HRESULT STDMETHODCALLTYPE SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 * pCursorBitmap);
    void STDMETHODCALLTYPE SetCursorPosition(int X, int Y, DWORD Flags);
    BOOL STDMETHODCALLTYPE ShowCursor(BOOL bShow);
    HRESULT STDMETHODCALLTYPE CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS * pPresentationParameters, IDirect3DSwapChain9 ** pSwapChain);
    HRESULT STDMETHODCALLTYPE GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9 ** pSwapChain);
    UINT STDMETHODCALLTYPE GetNumberOfSwapChains();
    HRESULT STDMETHODCALLTYPE Reset(D3DPRESENT_PARAMETERS * pPresentationParameters);
    HRESULT STDMETHODCALLTYPE Present(const RECT * pSourceRect, const RECT * pDestRect, HWND hDestWindowOverride, const RGNDATA * pDirtyRegion);
    HRESULT STDMETHODCALLTYPE GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9 ** ppBackBuffer);
    HRESULT STDMETHODCALLTYPE GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS * pRasterStatus);
    HRESULT STDMETHODCALLTYPE SetDialogBoxMode(BOOL bEnableDialogs);
    void STDMETHODCALLTYPE SetGammaRamp(UINT iSwapChain, DWORD Flags, const D3DGAMMARAMP * pRamp);
    void STDMETHODCALLTYPE GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP * pRamp);
    HRESULT STDMETHODCALLTYPE CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9 ** ppTexture, HANDLE * pSharedHandle);
    HRESULT STDMETHODCALLTYPE CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9 ** ppVolumeTexture, HANDLE * pSharedHandle);
    HRESULT STDMETHODCALLTYPE CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9 ** ppCubeTexture, HANDLE * pSharedHandle);
    HRESULT STDMETHODCALLTYPE CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9 ** ppVertexBuffer, HANDLE * pSharedHandle);
    HRESULT STDMETHODCALLTYPE CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9 ** ppIndexBuffer, HANDLE * pSharedHandle);
    HRESULT STDMETHODCALLTYPE CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle);
    HRESULT STDMETHODCALLTYPE CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle);
    HRESULT STDMETHODCALLTYPE UpdateSurface(IDirect3DSurface9 * pSourceSurface, const RECT * pSourceRect, IDirect3DSurface9 * pDestinationSurface, const POINT * pDestPoint);
    HRESULT STDMETHODCALLTYPE UpdateTexture(IDirect3DBaseTexture9 * pSourceTexture, IDirect3DBaseTexture9 * pDestinationTexture);
    HRESULT STDMETHODCALLTYPE GetRenderTargetData(IDirect3DSurface9 * pRenderTarget, IDirect3DSurface9 * pDestSurface);
    HRESULT STDMETHODCALLTYPE GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9 * pDestSurface);
    HRESULT STDMETHODCALLTYPE StretchRect(IDirect3DSurface9 * pSourceSurface, const RECT * pSourceRect, IDirect3DSurface9 * pDestSurface, const RECT * pDestRect, D3DTEXTUREFILTERTYPE Filter);
    HRESULT STDMETHODCALLTYPE ColorFill(IDirect3DSurface9 * pSurface, const RECT * pRect, D3DCOLOR color);
    HRESULT STDMETHODCALLTYPE CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle);
    HRESULT STDMETHODCALLTYPE SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9 * pRenderTarget);
    HRESULT STDMETHODCALLTYPE GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9 ** ppRenderTarget);
    HRESULT STDMETHODCALLTYPE SetDepthStencilSurface(IDirect3DSurface9 * pNewZStencil);
    HRESULT STDMETHODCALLTYPE GetDepthStencilSurface(IDirect3DSurface9 ** ppZStencilSurface);
    HRESULT STDMETHODCALLTYPE BeginScene();
    HRESULT STDMETHODCALLTYPE EndScene();
    HRESULT STDMETHODCALLTYPE Clear(DWORD Count, const D3DRECT * pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
    HRESULT STDMETHODCALLTYPE SetTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX * pMatrix);
    HRESULT STDMETHODCALLTYPE GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX * pMatrix);
    HRESULT STDMETHODCALLTYPE MultiplyTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX * pMatrix);
    HRESULT STDMETHODCALLTYPE SetViewport(const D3DVIEWPORT9 * pViewport);
    HRESULT STDMETHODCALLTYPE GetViewport(D3DVIEWPORT9 * pViewport);
    HRESULT STDMETHODCALLTYPE SetMaterial(const D3DMATERIAL9 * pMaterial);
    HRESULT STDMETHODCALLTYPE GetMaterial(D3DMATERIAL9 * pMaterial);
    HRESULT STDMETHODCALLTYPE SetLight(DWORD Index, const D3DLIGHT9 * pLight);
    HRESULT STDMETHODCALLTYPE GetLight(DWORD Index, D3DLIGHT9 * pLight);
    HRESULT STDMETHODCALLTYPE LightEnable(DWORD Index, BOOL Enable);
    HRESULT STDMETHODCALLTYPE GetLightEnable(DWORD Index, BOOL * pEnable);
    HRESULT STDMETHODCALLTYPE SetClipPlane(DWORD Index, const float * pPlane);
    HRESULT STDMETHODCALLTYPE GetClipPlane(DWORD Index, float * pPlane);
    HRESULT STDMETHODCALLTYPE SetRenderState(D3DRENDERSTATETYPE State, DWORD Value);
    HRESULT STDMETHODCALLTYPE GetRenderState(D3DRENDERSTATETYPE State, DWORD * pValue);
    HRESULT STDMETHODCALLTYPE CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9 ** ppSB);
    HRESULT STDMETHODCALLTYPE BeginStateBlock();
    HRESULT STDMETHODCALLTYPE EndStateBlock(IDirect3DStateBlock9 ** ppSB);
    HRESULT STDMETHODCALLTYPE SetClipStatus(const D3DCLIPSTATUS9 * pClipStatus);
    HRESULT STDMETHODCALLTYPE GetClipStatus(D3DCLIPSTATUS9 * pClipStatus);
    HRESULT STDMETHODCALLTYPE GetTexture(DWORD Stage, IDirect3DBaseTexture9 ** ppTexture);
    HRESULT STDMETHODCALLTYPE SetTexture(DWORD Stage, IDirect3DBaseTexture9 * pTexture);
    HRESULT STDMETHODCALLTYPE GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD * pValue);
    HRESULT STDMETHODCALLTYPE SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
    HRESULT STDMETHODCALLTYPE GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD * pValue);
    HRESULT STDMETHODCALLTYPE SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value);
    HRESULT STDMETHODCALLTYPE ValidateDevice(DWORD* pNumPasses);
    HRESULT STDMETHODCALLTYPE SetPaletteEntries(UINT PaletteNumber, const PALETTEENTRY * pEntries);
    HRESULT STDMETHODCALLTYPE GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY * pEntries);
    HRESULT STDMETHODCALLTYPE SetCurrentTexturePalette(UINT PaletteNumber);
    HRESULT STDMETHODCALLTYPE GetCurrentTexturePalette(UINT * PaletteNumber);
    HRESULT STDMETHODCALLTYPE SetScissorRect(const RECT * pRect);
    HRESULT STDMETHODCALLTYPE GetScissorRect(RECT * pRect);
    HRESULT STDMETHODCALLTYPE SetSoftwareVertexProcessing(BOOL bSoftware);
    BOOL STDMETHODCALLTYPE GetSoftwareVertexProcessing();
    HRESULT STDMETHODCALLTYPE SetNPatchMode(float nSegments);
    float STDMETHODCALLTYPE GetNPatchMode();
    HRESULT STDMETHODCALLTYPE DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
    HRESULT STDMETHODCALLTYPE DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
    HRESULT STDMETHODCALLTYPE DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void * pVertexStreamZeroData, UINT VertexStreamZeroStride);
    HRESULT STDMETHODCALLTYPE DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, const void * pIndexData, D3DFORMAT IndexDataFormat, const void * pVertexStreamZeroData, UINT VertexStreamZeroStride);
    HRESULT STDMETHODCALLTYPE ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9 * pDestBuffer, IDirect3DVertexDeclaration9 * pVertexDecl, DWORD Flags);
    HRESULT STDMETHODCALLTYPE CreateVertexDeclaration(const D3DVERTEXELEMENT9 * pVertexElements, IDirect3DVertexDeclaration9 ** ppDecl);
    HRESULT STDMETHODCALLTYPE SetVertexDeclaration(IDirect3DVertexDeclaration9 * pDecl);
    HRESULT STDMETHODCALLTYPE GetVertexDeclaration(IDirect3DVertexDeclaration9 ** ppDecl);
    HRESULT STDMETHODCALLTYPE SetFVF(DWORD FVF);
    HRESULT STDMETHODCALLTYPE GetFVF(DWORD * pFVF);
    HRESULT STDMETHODCALLTYPE CreateVertexShader(const DWORD * pFunction, IDirect3DVertexShader9 ** ppShader);
    HRESULT STDMETHODCALLTYPE SetVertexShader(IDirect3DVertexShader9 * pShader);
    HRESULT STDMETHODCALLTYPE GetVertexShader(IDirect3DVertexShader9 ** ppShader);
    HRESULT STDMETHODCALLTYPE SetVertexShaderConstantF(UINT StartRegister, const float * pConstantData, UINT Vector4fCount);
    HRESULT STDMETHODCALLTYPE GetVertexShaderConstantF(UINT StartRegister, float * pConstantData, UINT Vector4fCount);
    HRESULT STDMETHODCALLTYPE SetVertexShaderConstantI(UINT StartRegister, const int * pConstantData, UINT Vector4iCount);
    HRESULT STDMETHODCALLTYPE GetVertexShaderConstantI(UINT StartRegister, int * pConstantData, UINT Vector4iCount);
    HRESULT STDMETHODCALLTYPE SetVertexShaderConstantB(UINT StartRegister, const BOOL * pConstantData, UINT BoolCount);
    HRESULT STDMETHODCALLTYPE GetVertexShaderConstantB(UINT StartRegister, BOOL * pConstantData, UINT BoolCount);
    HRESULT STDMETHODCALLTYPE SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9 * pStreamData, UINT OffsetInBytes, UINT Stride);
    HRESULT STDMETHODCALLTYPE GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9 ** ppStreamData, UINT * pOffsetInBytes, UINT * pStride);
    HRESULT STDMETHODCALLTYPE SetStreamSourceFreq(UINT StreamNumber, UINT Setting);
    HRESULT STDMETHODCALLTYPE GetStreamSourceFreq(UINT StreamNumber, UINT * pSetting);
    HRESULT STDMETHODCALLTYPE SetIndices(IDirect3DIndexBuffer9 * pIndexData);
    HRESULT STDMETHODCALLTYPE GetIndices(IDirect3DIndexBuffer9 ** ppIndexData);
    HRESULT STDMETHODCALLTYPE CreatePixelShader(const DWORD * pFunction, IDirect3DPixelShader9 ** ppShader);
    HRESULT STDMETHODCALLTYPE SetPixelShader(IDirect3DPixelShader9 * pShader);
    HRESULT STDMETHODCALLTYPE GetPixelShader(IDirect3DPixelShader9 ** ppShader);
    HRESULT STDMETHODCALLTYPE SetPixelShaderConstantF(UINT StartRegister, const float * pConstantData, UINT Vector4fCount);
    HRESULT STDMETHODCALLTYPE GetPixelShaderConstantF(UINT StartRegister, float * pConstantData, UINT Vector4fCount);
    HRESULT STDMETHODCALLTYPE SetPixelShaderConstantI(UINT StartRegister, const int * pConstantData, UINT Vector4iCount);
    HRESULT STDMETHODCALLTYPE GetPixelShaderConstantI(UINT StartRegister, int * pConstantData, UINT Vector4iCount);
    HRESULT STDMETHODCALLTYPE SetPixelShaderConstantB(UINT StartRegister, const BOOL * pConstantData, UINT BoolCount);
    HRESULT STDMETHODCALLTYPE GetPixelShaderConstantB(UINT StartRegister, BOOL * pConstantData, UINT BoolCount);
    HRESULT STDMETHODCALLTYPE DrawRectPatch(UINT Handle, const float * pNumSegs, const D3DRECTPATCH_INFO * pRectPatchInfo);
    HRESULT STDMETHODCALLTYPE DrawTriPatch(UINT Handle, const float * pNumSegs, const D3DTRIPATCH_INFO * pTriPatchInfo);
    HRESULT STDMETHODCALLTYPE DeletePatch(UINT Handle);
    HRESULT STDMETHODCALLTYPE CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9 ** ppQuery);
};

CDirect3DDevice9Proxy::CDirect3DDevice9Proxy(IDirect3D9 * pD3D, IDirect3DDevice9 * pDevice) {
    m_pD3D = pD3D;
    m_pD3DDevice = pDevice;
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::QueryInterface(REFIID riid, void ** ppvObj) {
    return m_pD3DDevice->QueryInterface(riid, ppvObj);
}

ULONG STDMETHODCALLTYPE CDirect3DDevice9Proxy::AddRef() {
    return m_pD3DDevice->AddRef();
}

ULONG STDMETHODCALLTYPE CDirect3DDevice9Proxy::Release() {
    ULONG uRet = m_pD3DDevice->Release();

    if(uRet == 0)
        delete this;

    return uRet;
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::TestCooperativeLevel() {
    HRESULT res = m_pD3DDevice->TestCooperativeLevel();
    /*
    if (res == D3D_OK) {
        mod_log("TestCooperativeLevel: D3D_OK");
    } else if (res == D3DERR_DEVICELOST) {
        mod_log("TestCooperativeLevel: D3DERR_DEVICELOST");
    }
    else if (res == D3DERR_DEVICENOTRESET) {
        mod_log("TestCooperativeLevel: D3DERR_DEVICENOTRESET");
    }
    else if (res == D3DERR_DRIVERINTERNALERROR) {
        mod_log("TestCooperativeLevel: D3DERR_DRIVERINTERNALERROR");
    }
    */
    return res;
}

UINT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetAvailableTextureMem() {
    return m_pD3DDevice->GetAvailableTextureMem();
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::EvictManagedResources() {
    return m_pD3DDevice->EvictManagedResources();
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetDirect3D(IDirect3D9 ** ppD3D9) {
    HRESULT hr = m_pD3DDevice->GetDirect3D(ppD3D9);

    if(SUCCEEDED(hr))
        *ppD3D9 = m_pD3D;

    return hr;
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetDeviceCaps(D3DCAPS9 * pCaps) {
    return m_pD3DDevice->GetDeviceCaps(pCaps);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE * pMode) {
    return m_pD3DDevice->GetDisplayMode(iSwapChain, pMode);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS * pParameters) {
    return m_pD3DDevice->GetCreationParameters(pParameters);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 * pCursorBitmap) {
    return m_pD3DDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

void STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetCursorPosition(int X, int Y, DWORD Flags) {
    return m_pD3DDevice->SetCursorPosition(X, Y, Flags);
}

BOOL STDMETHODCALLTYPE CDirect3DDevice9Proxy::ShowCursor(BOOL bShow) {
    return m_pD3DDevice->ShowCursor(bShow);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS * pPresentationParameters, IDirect3DSwapChain9 ** pSwapChain) {
    return m_pD3DDevice->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9 ** pSwapChain) {
    return m_pD3DDevice->GetSwapChain(iSwapChain, pSwapChain);
}

UINT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetNumberOfSwapChains() {
    return m_pD3DDevice->GetNumberOfSwapChains();
}

extern "C" int D3D_Reset(SDL_Renderer * renderer, void *);

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::Reset(D3DPRESENT_PARAMETERS * pPresentationParameters) {
    DWORD device_flags = pPresentationParameters->Flags;

    mod_log("d3d9::reset");

    if (D3D_Reset(gfx_state.rnd, (void *)pPresentationParameters)) {
        mod_log(SDL_GetError());
    }

    return D3D_OK;
    /*
    HRESULT res = m_pD3DDevice->Reset(pPresentationParameters);

    if (res == D3D_OK) {
        mod_log("Reset: D3D_OK");
    }
    else if (res == D3DERR_DEVICELOST) {
        mod_log("Reset: D3DERR_DEVICELOST");
    }
    else if (res == D3DERR_UNSUPPORTEDALPHAARG) {
        mod_log("Reset: D3DERR_UNSUPPORTEDALPHAARG");
    }
    else if (res == D3DERR_INVALIDCALL) {
        mod_log("Reset: D3DERR_INVALIDCALL");
    }
    else if (res == D3DERR_DEVICEHUNG) {
        mod_log("Reset: D3DERR_DEVICEHUNG");
    }
    else {
        mod_log("Reset: D3DERR_WTF");
    }

    return res;
    */
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::Present(const RECT * pSourceRect, const RECT * pDestRect, HWND hDestWindowOverride, const RGNDATA * pDirtyRegion) {
    return m_pD3DDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9 ** ppBackBuffer) {
    return m_pD3DDevice->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS * pRasterStatus) {
    return m_pD3DDevice->GetRasterStatus(iSwapChain, pRasterStatus);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetDialogBoxMode(BOOL bEnableDialogs) {
    return m_pD3DDevice->SetDialogBoxMode(bEnableDialogs);
}

void STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetGammaRamp(UINT iSwapChain, DWORD Flags, const D3DGAMMARAMP * pRamp) {
    return m_pD3DDevice->SetGammaRamp(iSwapChain, Flags, pRamp);
}

void STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP * pRamp) {
    return m_pD3DDevice->GetGammaRamp(iSwapChain, pRamp);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9 ** ppTexture, HANDLE * pSharedHandle) {
    return m_pD3DDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9 ** ppVolumeTexture, HANDLE * pSharedHandle) {
    return m_pD3DDevice->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9 ** ppCubeTexture, HANDLE * pSharedHandle) {
    return m_pD3DDevice->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9 ** ppVertexBuffer, HANDLE * pSharedHandle) {
    return m_pD3DDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9 ** ppIndexBuffer, HANDLE * pSharedHandle) {
    return m_pD3DDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle) {
    return m_pD3DDevice->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle) {
    return m_pD3DDevice->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::UpdateSurface(IDirect3DSurface9 * pSourceSurface, const RECT * pSourceRect, IDirect3DSurface9 * pDestinationSurface, const POINT * pDestPoint) {
    return m_pD3DDevice->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::UpdateTexture(IDirect3DBaseTexture9 * pSourceTexture, IDirect3DBaseTexture9 * pDestinationTexture) {
    return m_pD3DDevice->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetRenderTargetData(IDirect3DSurface9 * pRenderTarget, IDirect3DSurface9 * pDestSurface) {
    return m_pD3DDevice->GetRenderTargetData(pRenderTarget, pDestSurface);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9 * pDestSurface) {
    return m_pD3DDevice->GetFrontBufferData(iSwapChain, pDestSurface);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::StretchRect(IDirect3DSurface9 * pSourceSurface, const RECT * pSourceRect, IDirect3DSurface9 * pDestSurface, const RECT * pDestRect, D3DTEXTUREFILTERTYPE Filter) {
    return m_pD3DDevice->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::ColorFill(IDirect3DSurface9 * pSurface, const RECT * pRect, D3DCOLOR color) {
    return m_pD3DDevice->ColorFill(pSurface, pRect, color);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9 ** ppSurface, HANDLE * pSharedHandle) {
    return m_pD3DDevice->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9 * pRenderTarget) {
    return m_pD3DDevice->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9 ** ppRenderTarget) {
    return m_pD3DDevice->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetDepthStencilSurface(IDirect3DSurface9 * pNewZStencil) {
    return m_pD3DDevice->SetDepthStencilSurface(pNewZStencil);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetDepthStencilSurface(IDirect3DSurface9 ** ppZStencilSurface) {
    return m_pD3DDevice->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::BeginScene( ) {
    // Call our real handler
    HRESULT hr = m_pD3DDevice->BeginScene();

    /*// Possible fix for missing textures on some chipsets
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);*/

    // graphics_device_prerender();

    return hr;
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::EndScene() {
    IDirect3DDevice9 *device = m_pD3DDevice;
    IDirect3DStateBlock9* pStateBlock = NULL;
    device->CreateStateBlock(D3DSBT_ALL, &pStateBlock);

    IDirect3DDevice9_SetVertexShader(device, NULL);
    IDirect3DDevice9_SetFVF(device, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

    IDirect3DDevice9_SetRenderState(device, D3DRS_ZENABLE, D3DZB_FALSE);
    IDirect3DDevice9_SetRenderState(device, D3DRS_CULLMODE, D3DCULL_NONE);
    IDirect3DDevice9_SetRenderState(device, D3DRS_LIGHTING, FALSE);

    /* Enable color modulation by diffuse color */
    IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    /* Enable alpha modulation by diffuse alpha */
    IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    IDirect3DDevice9_SetTextureStageState(device, 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

    /* Enable separate alpha blend function, if possible */
    // if (data->enableSeparateAlphaBlend) {
    //     IDirect3DDevice9_SetRenderState(device, D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
    // }

    /* Disable second texture stage, since we're done */
    IDirect3DDevice9_SetTextureStageState(device, 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    IDirect3DDevice9_SetTextureStageState(device, 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

    IDirect3DDevice9_SetPixelShader(device, NULL);

    gfx_render_draw();

    pStateBlock->Apply();
    pStateBlock->Release();

    return m_pD3DDevice->EndScene();
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::Clear(DWORD Count, const D3DRECT * pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) {
    return m_pD3DDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX * pMatrix) {
    return m_pD3DDevice->SetTransform(State, pMatrix);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX * pMatrix) {
    return m_pD3DDevice->GetTransform(State, pMatrix);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::MultiplyTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX * pMatrix) {
    return m_pD3DDevice->MultiplyTransform(State, pMatrix);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetViewport(const D3DVIEWPORT9 * pViewport) {
    return m_pD3DDevice->SetViewport(pViewport);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetViewport(D3DVIEWPORT9 * pViewport) {
    return m_pD3DDevice->GetViewport(pViewport);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetMaterial(const D3DMATERIAL9 * pMaterial) {
    return m_pD3DDevice->SetMaterial(pMaterial);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetMaterial(D3DMATERIAL9 * pMaterial) {
    return m_pD3DDevice->GetMaterial(pMaterial);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetLight(DWORD Index, const D3DLIGHT9 * pLight) {
    return m_pD3DDevice->SetLight(Index, pLight);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetLight(DWORD Index, D3DLIGHT9 * pLight) {
    return m_pD3DDevice->GetLight(Index, pLight);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::LightEnable(DWORD Index, BOOL Enable) {
    return m_pD3DDevice->LightEnable(Index, Enable);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetLightEnable(DWORD Index, BOOL * pEnable) {
    return m_pD3DDevice->GetLightEnable(Index, pEnable);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetClipPlane(DWORD Index, const float * pPlane) {
    return m_pD3DDevice->SetClipPlane(Index, pPlane);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetClipPlane(DWORD Index, float * pPlane) {
    return m_pD3DDevice->GetClipPlane(Index, pPlane);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) {
    return m_pD3DDevice->SetRenderState(State, Value);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetRenderState(D3DRENDERSTATETYPE State, DWORD * pValue) {
    return m_pD3DDevice->GetRenderState(State, pValue);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9 ** ppSB) {
    return m_pD3DDevice->CreateStateBlock(Type, ppSB);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::BeginStateBlock() {
    return m_pD3DDevice->BeginStateBlock();
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::EndStateBlock(IDirect3DStateBlock9 ** ppSB) {
    return m_pD3DDevice->EndStateBlock(ppSB);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetClipStatus(const D3DCLIPSTATUS9 * pClipStatus) {
    return m_pD3DDevice->SetClipStatus(pClipStatus);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetClipStatus(D3DCLIPSTATUS9 * pClipStatus) {
    return m_pD3DDevice->GetClipStatus(pClipStatus);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetTexture(DWORD Stage, IDirect3DBaseTexture9 ** ppTexture) {
    return m_pD3DDevice->GetTexture(Stage, ppTexture);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetTexture(DWORD Stage, IDirect3DBaseTexture9 * pTexture) {
    return m_pD3DDevice->SetTexture(Stage, pTexture);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD * pValue) {
    return m_pD3DDevice->GetTextureStageState(Stage, Type, pValue);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) {
    return m_pD3DDevice->SetTextureStageState(Stage, Type, Value);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD * pValue) {
    return m_pD3DDevice->GetSamplerState(Sampler, Type, pValue);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) {
    return m_pD3DDevice->SetSamplerState(Sampler, Type, Value);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::ValidateDevice(DWORD* pNumPasses) {
    return m_pD3DDevice->ValidateDevice(pNumPasses);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetPaletteEntries(UINT PaletteNumber, const PALETTEENTRY * pEntries) {
    return m_pD3DDevice->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY * pEntries) {
    return m_pD3DDevice->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetCurrentTexturePalette(UINT PaletteNumber) {
    return m_pD3DDevice->SetCurrentTexturePalette(PaletteNumber);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetCurrentTexturePalette(UINT * PaletteNumber) {
    return m_pD3DDevice->GetCurrentTexturePalette(PaletteNumber);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetScissorRect(const RECT * pRect) {
    return m_pD3DDevice->SetScissorRect(pRect);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetScissorRect(RECT * pRect) {
    return m_pD3DDevice->GetScissorRect(pRect);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetSoftwareVertexProcessing(BOOL bSoftware) {
    return m_pD3DDevice->SetSoftwareVertexProcessing(bSoftware);
}

BOOL STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetSoftwareVertexProcessing() {
    return m_pD3DDevice->GetSoftwareVertexProcessing();
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetNPatchMode(float nSegments) {
    return m_pD3DDevice->SetNPatchMode(nSegments);
}

float STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetNPatchMode() {
    return m_pD3DDevice->GetNPatchMode();
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) {
    return m_pD3DDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
    return m_pD3DDevice->DrawIndexedPrimitive( PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount );
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void * pVertexStreamZeroData, UINT VertexStreamZeroStride) {
    return m_pD3DDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, const void * pIndexData, D3DFORMAT IndexDataFormat, const void * pVertexStreamZeroData, UINT VertexStreamZeroStride) {
    return m_pD3DDevice->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat,pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9 * pDestBuffer, IDirect3DVertexDeclaration9 * pVertexDecl, DWORD Flags) {
    return m_pD3DDevice->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::CreateVertexDeclaration(const D3DVERTEXELEMENT9 * pVertexElements, IDirect3DVertexDeclaration9 ** ppDecl) {
    return m_pD3DDevice->CreateVertexDeclaration(pVertexElements, ppDecl);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetVertexDeclaration(IDirect3DVertexDeclaration9 * pDecl) {
    return m_pD3DDevice->SetVertexDeclaration(pDecl);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetVertexDeclaration(IDirect3DVertexDeclaration9 ** ppDecl) {
    return m_pD3DDevice->GetVertexDeclaration(ppDecl);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetFVF(DWORD FVF) {
    return m_pD3DDevice->SetFVF(FVF);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetFVF(DWORD * pFVF) {
    return m_pD3DDevice->GetFVF(pFVF);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::CreateVertexShader(const DWORD * pFunction, IDirect3DVertexShader9 ** ppShader) {
    return m_pD3DDevice->CreateVertexShader(pFunction, ppShader);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetVertexShader(IDirect3DVertexShader9 * pShader) {
    return m_pD3DDevice->SetVertexShader(pShader);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetVertexShader(IDirect3DVertexShader9 ** ppShader) {
    return m_pD3DDevice->GetVertexShader(ppShader);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetPixelShaderConstantF(UINT StartRegister, const float * pConstantData, UINT Vector4fCount) {
    return m_pD3DDevice->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetVertexShaderConstantF(UINT StartRegister, const float * pConstantData, UINT Vector4fCount) {
    return m_pD3DDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetVertexShaderConstantF(UINT StartRegister, float * pConstantData, UINT Vector4fCount) {
    return m_pD3DDevice->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetVertexShaderConstantI(UINT StartRegister, const int * pConstantData, UINT Vector4iCount) {
    return m_pD3DDevice->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetVertexShaderConstantI(UINT StartRegister, int * pConstantData, UINT Vector4iCount) {
    return m_pD3DDevice->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetVertexShaderConstantB(UINT StartRegister, const BOOL * pConstantData, UINT BoolCount) {
    return m_pD3DDevice->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetVertexShaderConstantB(UINT StartRegister, BOOL * pConstantData, UINT BoolCount) {
    return m_pD3DDevice->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9 * pStreamData, UINT OffsetInBytes, UINT Stride) {
    return m_pD3DDevice->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9 ** ppStreamData, UINT * pOffsetInBytes, UINT * pStride) {
    return m_pD3DDevice->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetStreamSourceFreq(UINT StreamNumber, UINT Setting) {
    return m_pD3DDevice->SetStreamSourceFreq(StreamNumber, Setting);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetStreamSourceFreq(UINT StreamNumber, UINT * pSetting) {
    return m_pD3DDevice->GetStreamSourceFreq(StreamNumber, pSetting);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetIndices(IDirect3DIndexBuffer9 * pIndexData) {
    return m_pD3DDevice->SetIndices(pIndexData);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetIndices(IDirect3DIndexBuffer9 ** ppIndexData) {
    return m_pD3DDevice->GetIndices(ppIndexData);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::CreatePixelShader(const DWORD * pFunction, IDirect3DPixelShader9 ** ppShader) {
    return m_pD3DDevice->CreatePixelShader(pFunction, ppShader);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetPixelShader(IDirect3DPixelShader9 * pShader) {
    return m_pD3DDevice->SetPixelShader(pShader);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetPixelShader(IDirect3DPixelShader9 ** ppShader) {
    return m_pD3DDevice->GetPixelShader(ppShader);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetPixelShaderConstantF(UINT StartRegister, float * pConstantData, UINT Vector4fCount) {
    return m_pD3DDevice->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetPixelShaderConstantI(UINT StartRegister, const int * pConstantData, UINT Vector4iCount) {
    return m_pD3DDevice->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetPixelShaderConstantI(UINT StartRegister, int * pConstantData, UINT Vector4iCount) {
    return m_pD3DDevice->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::SetPixelShaderConstantB(UINT StartRegister, const BOOL * pConstantData, UINT BoolCount) {
    return m_pD3DDevice->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::GetPixelShaderConstantB(UINT StartRegister, BOOL * pConstantData, UINT BoolCount) {
    return m_pD3DDevice->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::DrawRectPatch(UINT Handle, const float * pNumSegs, const D3DRECTPATCH_INFO * pRectPatchInfo) {
    return m_pD3DDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::DrawTriPatch(UINT Handle, const float * pNumSegs, const D3DTRIPATCH_INFO * pTriPatchInfo) {
    return m_pD3DDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::DeletePatch(UINT Handle) {
    return m_pD3DDevice->DeletePatch(Handle);
}

HRESULT STDMETHODCALLTYPE CDirect3DDevice9Proxy::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9 ** ppQuery) {
    return m_pD3DDevice->CreateQuery(Type, ppQuery);
}
