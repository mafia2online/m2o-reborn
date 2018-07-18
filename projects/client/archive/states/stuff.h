IDirect3DVertexBuffer9*	m_vb;
IDirect3DTexture9 *g_texture = NULL;
DWORD				m_dwRenderTextureBlock;

#define OURVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE)
struct thisVertex {
    float x, y, z;
    float rhw;
    DWORD color;
    float tu, tv;
};




void  foobar_SetRenderTextureStateBlock()
{
    mod.graphics.device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    mod.graphics.device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    mod.graphics.device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    mod.graphics.device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    mod.graphics.device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    mod.graphics.device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    mod.graphics.device->SetRenderState(D3DRS_ALPHAREF, 0x08);
    mod.graphics.device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    mod.graphics.device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    mod.graphics.device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    mod.graphics.device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
    mod.graphics.device->SetRenderState(D3DRS_CLIPPING, TRUE);
    mod.graphics.device->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
    mod.graphics.device->SetRenderState(D3DRS_VERTEXBLEND, FALSE);
    mod.graphics.device->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
    mod.graphics.device->SetRenderState(D3DRS_FOGENABLE, FALSE);

    //--- 
    mod.graphics.device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    mod.graphics.device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    mod.graphics.device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    //------
    mod.graphics.device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    mod.graphics.device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

    mod.graphics.device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    mod.graphics.device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
    mod.graphics.device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    mod.graphics.device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

    mod.graphics.device->SetRenderState(D3DRS_WRAP0, 0);

    // now save it

}

// Render texture with precalculated position
void foobar_RenderTexture(int x, int y, float z, int w, int h, LPDIRECT3DTEXTURE9 texture, unsigned char alpha)
{
    IDirect3DStateBlock9* pStateBlock = NULL;
    mod.graphics.device->CreateStateBlock(D3DSBT_ALL, &pStateBlock);

    foobar_SetRenderTextureStateBlock();


    mod.graphics.device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    mod.graphics.device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    mod.graphics.device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);   //Ignored
    mod.graphics.device->SetTexture(0, texture);

    // x, y, z, rhw, color
    DWORD color = (alpha << 24);
    thisVertex g_square_vertices[] = {
        { (float)x, (float)y, z, 1.0f,color, 0.0f, 0.0f },
        { (float)(x + w), (float)y, z, 1.0f, color, 1.0f, 0.0f },
        { (float)x, (float)(y + h), z, 1.0f, color, 0.0f, 1.0f },
        { (float)(x + w), (float)(y + h), z, 1.0f, color, 1.0f, 1.0f }
    };

    unsigned char*	buffer;
    m_vb->Lock(0, 0, (void **)&buffer, 0);
    memcpy(buffer, g_square_vertices, sizeof(g_square_vertices));
    m_vb->Unlock();


    mod.graphics.device->SetPixelShader(NULL);
    mod.graphics.device->SetFVF(OURVERTEX);

    mod.graphics.device->SetStreamSource(0, m_vb, 0, sizeof(thisVertex));

    //Now we're drawing a Triangle Strip, 4 vertices to draw 2 triangles.
    mod.graphics.device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

    // SetTexture NULL is important, it makes texture availables Release() function
    /*m_DirectDevice->SetTexture(0,NULL);
    /m_DirectDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
    m_DirectDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
    */
        
    // in the end just reset the previous states
    //mod.graphics.device->ApplyStateBlock(m_dwRenderTextureBlockDeposit);
    pStateBlock->Apply();
    pStateBlock->Release();

}

void foobar_FillARGB(int x, int y, float z, int w, int h, D3DCOLOR color)
{
    mod.graphics.device->SetRenderState(D3DRS_ZENABLE, FALSE);
    mod.graphics.device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    mod.graphics.device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

    IDirect3DStateBlock9* pStateBlock = NULL;
    mod.graphics.device->CreateStateBlock(D3DSBT_ALL, &pStateBlock);
    // fuck it

    mod.graphics.device->SetTexture(0, NULL);

    mod.graphics.device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    mod.graphics.device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    mod.graphics.device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    mod.graphics.device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    mod.graphics.device->SetRenderState(D3DRS_ALPHAREF, 0x08);
    mod.graphics.device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    mod.graphics.device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    mod.graphics.device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    mod.graphics.device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
    mod.graphics.device->SetRenderState(D3DRS_CLIPPING, TRUE);
    mod.graphics.device->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
    mod.graphics.device->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
    mod.graphics.device->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
    mod.graphics.device->SetRenderState(D3DRS_FOGENABLE, FALSE);
    mod.graphics.device->SetRenderState(D3DRS_COLORWRITEENABLE,
        D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
        D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
    mod.graphics.device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    mod.graphics.device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    mod.graphics.device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    mod.graphics.device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    mod.graphics.device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    mod.graphics.device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    mod.graphics.device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    mod.graphics.device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
    mod.graphics.device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    mod.graphics.device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    //mod.graphics.device->SetRenderState(D3DRS_EDGEANTIALIAS, TRUE);

    thisVertex g_square_vertices[] = {
        { (float)x, (float)(y + h), z, 0.0f, color , 0.0f, 0.0f }, // x, y, z, rhw, color
    { (float)x, (float)y, z, 0.0f, color, 0.0f, 0.0f },
    { (float)(x + w), (float)(y + h), z, 0.0f, color, 0.0f, 0.0f },
    { (float)(x + w), (float)y, z, 0.0f, color, 0.0f, 0.0f }
    };

    unsigned char*  buffer;
    m_vb->Lock(0, 0, (void **)&buffer, 0);
    memcpy(buffer, g_square_vertices, sizeof(g_square_vertices));
    m_vb->Unlock();


    mod.graphics.device->SetPixelShader(NULL);
    mod.graphics.device->SetFVF(OURVERTEX);

    mod.graphics.device->SetStreamSource(0, m_vb, 0, sizeof(thisVertex));


    //Now we're drawing a Triangle Strip, 4 vertices to draw 2 triangles.
    mod.graphics.device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

    pStateBlock->Apply();
    pStateBlock->Release();

}



void init_stuff() {
    // Vertex buffer is used during quad rendering
    HRESULT hr = mod.graphics.device->CreateVertexBuffer(6 * sizeof(thisVertex), //Size of memory to be allocated
                                                                              //Number of vertices * size of a vertex
        D3DUSAGE_WRITEONLY,  //We never need to read from it so
                             //we specify write only, it's faster
        OURVERTEX,  //Our custom vertex specifier (coordinates & a colour)
        D3DPOOL_MANAGED,     //Tell DirectX to manage the memory of this resource
        &m_vb, NULL);              //Pointer to our Vertex Buffer, after this call
    /*
    hr = D3DXCreateTextureFromFileEx(
        mod.graphics.device,   //Direct3D Device
        "DH.png",       //File Name
        100,
        100,
        1,
        D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT,
        D3DX_FILTER_LINEAR,
        D3DX_FILTER_LINEAR,
        0x1000ff00,
        NULL,
        NULL,
        &g_texture);    //Texture handle
    */

    hr = D3DXCreateTexture(mod.graphics.device, 1024, 1024, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, (IDirect3DTexture9**)&g_texture);


        // hr = D3DXCreateTextureFromFile(mod.graphics.device,   //Direct3D Device

        //     "DH.png",       //File Name

        //     &g_texture);    //Texture handle

    if (FAILED(hr)) {
        mod_log("Error loading texture\n");
    }
}

static bool a_initialized = false;

void foobar() {
    if (!a_initialized) {
        a_initialized = true;
        init_stuff();
    }

    vec3_t screen;
    vec3_t swin = vec3f(-421.75f, 479.31f, 0.1f);
    graphics_world_to_screen(&screen, swin);
    // mod_assert(g_texture);
    //foobar_FillARGB(screen.x, screen.y, screen.z, 100, 100, 0xffffffff);

    vec3_t screen2;
    graphics_world_to_screen(&screen2, swin);
    //foobar_FillARGB(screen2.x, screen2.y, 0.5f, 100, 100, 0xffccffcc);

    // foobar_RenderTexture(screen2.x, screen2.y, screen2.z, 100, 100, g_texture, 100);

    zpl_mutex_lock(&mod.mutexes.cef);
    if (g_texture) {
        // foobar_RenderTexture(0, 0, 0, 1024, 1024, g_texture, 255);
    }
    zpl_mutex_unlock(&mod.mutexes.cef);

    // mod_log("drawing a texture %x at: %f\n", g_texture, screen.z);

    //gfont->RestoreDeviceObjects();
    //gfont->DrawTextScaled(screen.x, screen.y, 0.0, 2.0, 2.0, 0xccffffcc, "hgello weolsd");
}

#define MOD_STUFF_STATE { nullptr, nullptr, nullptr, foobar }
