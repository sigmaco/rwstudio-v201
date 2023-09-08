#include "precomp.h"
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

#include "rpmatfx.h"

#include "framework/toolkits/world/clevel.h"
#include "CFXReflectEnvironmentXBox.h"
#include "framework/core/resourcemanager/cresourcemanager.h"

namespace RWS
{                    

   namespace FX
   {

      typedef struct RenderedVertex
      {
          RwReal      x, y, z;        // The un-transformed position for the vertex
          RwReal      u;              // Texture coordinate
          RwReal      v;              // Texture coordinate
      }RenderedVertex;

      /**
      *
      *  Construct a CFXReflectEnvironmentXBox object.
      *
      */
      CFXReflectEnvironmentXBox::CFXReflectEnvironmentXBox()
      {
         RWS_FUNCTION("RWS::FX::CFXReflectEnvironmentXBox::CFXReflectEnvironmentXBox");

         // get a pointer to the current device
         m_Dev = static_cast<LPDIRECT3DDEVICE8>(RwXboxGetCurrentD3DDevice());


         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CFXReflectEnvironmentXBox object.
      *
      */
      CFXReflectEnvironmentXBox::~CFXReflectEnvironmentXBox()
      {
         RWS_FUNCTION("RWS::FX::CFXReflectEnvironmentXBox::~CFXReflectEnvironmentXBox");

         RWS_RETURNVOID();
      }

      /**
      * Copies an area of the back buffer specified by TopLeftUV and BottomRightUV to pRaster.
      *
      * \param TopLeftUV      Coordinates of the top left area of the screen to copy to environment map texture.
      * \param BottomRightUV  Coordinates of the bottom right area of the screen to copy to environment map texture. 
      * \param pRaster        Pointer to the raster for which the area of the screen will be copied.
      * \return               void. 
      *
      */
      void CFXReflectEnvironmentXBox::UpdateTextureFromDisplay(const RwV2d &TopLeftUV, const RwV2d &BottomRightUV, const RwRaster *pRaster)
      {
         RWS_FUNCTION("RWS::FX::CFXReflectEnvironmentXBox::UpdateTextureFromDisplay");
  
         DWORD                EntryRenderState;
         LPDIRECT3DSURFACE8   DxBackBuffer;
         HRESULT              Res;

         const RwReal Width = 1.0f, Height = 1.0f;
         const RwReal ZNear = 1.0f;
         const RwReal ZFar  = 20.0f;

         // ensure RenderWare has finished rendering any commands that have previously been issused.
          _rwXbFlushCacheBuffer();

         // create the vertices used to define the polygon that will be used to copy from the back buffer.
         RwVideoMode VideoMode;

         RwEngineGetVideoModeInfo(&VideoMode, RwEngineGetCurrentVideoMode());
         const RwReal Left   = -0.5f;
         const RwReal Right  =  0.5f;
         const RwReal Top    = -0.5f;
         const RwReal Bottom =  0.5f;
         const RwReal Z      =  ZNear + 1.0f;

         RenderedVertex Vertices[] = {
                                       {Left,   Top,    Z, TopLeftUV.x     * static_cast<RwReal>(VideoMode.width), TopLeftUV.y     * static_cast<RwReal>(VideoMode.height) },
                                       {Right,  Top,    Z, BottomRightUV.x * static_cast<RwReal>(VideoMode.width), TopLeftUV.y     * static_cast<RwReal>(VideoMode.height) },
                                       {Left,   Bottom, Z, TopLeftUV.x     * static_cast<RwReal>(VideoMode.width), BottomRightUV.y * static_cast<RwReal>(VideoMode.height) },
                                       {Right,  Bottom, Z, BottomRightUV.x * static_cast<RwReal>(VideoMode.width), BottomRightUV.y * static_cast<RwReal>(VideoMode.height) },
                                    };

         // get a pointer to the back buffer surface
         m_Dev->GetBackBuffer( 0, 0, &DxBackBuffer );

         // copy the screen
         Res = m_Dev->CreateStateBlock(D3DSBT_ALL, &EntryRenderState);
         RWS_ASSERT(Res == D3D_OK, "Unable to create stateblock");

         // get the destination texture
         RwXboxRasterExtension   *XExt;
         IDirect3DTexture8       *DxDestText;
         IDirect3DSurface8       *DxDestSurface;
         XExt = RwXboxRasterGetExtension( const_cast<RwRaster *>(pRaster) );
         DxDestText = static_cast<IDirect3DTexture8*>(XExt->texture);
         DxDestText->GetSurfaceLevel( 0, &DxDestSurface );

         // set the render target to pRaster
         m_Dev->SetRenderTarget(DxDestSurface, 0);

         // setup the camera
         D3DXMATRIX matView;
         D3DXMATRIX matWorld;
         D3DXMATRIX matProj;

         D3DXMatrixIdentity(&matView);
         D3DXMatrixIdentity(&matWorld);
         D3DXMatrixOrthoLH (&matProj, Width, Height, ZNear, ZFar);

         m_Dev->SetTransform(D3DTS_VIEW,       &matView);
         m_Dev->SetTransform(D3DTS_WORLD,      &matWorld);
         m_Dev->SetTransform(D3DTS_PROJECTION, &matProj);

         // setup the render states for alpha blended textured poly
         m_Dev->SetPixelShader ( 0 );

         m_Dev->SetTextureStageState(0, D3DTSS_ALPHAOP,         D3DTOP_SELECTARG1);
         m_Dev->SetTextureStageState(0, D3DTSS_ALPHAARG1,       D3DTA_DIFFUSE);
                                                          
         m_Dev->SetTextureStageState(0, D3DTSS_COLOROP,         D3DTOP_SELECTARG2);
         m_Dev->SetTextureStageState(0, D3DTSS_COLORARG1,       D3DTA_DIFFUSE);
         m_Dev->SetTextureStageState(0, D3DTSS_COLORARG2,       D3DTA_TEXTURE);
         m_Dev->SetTextureStageState(0, D3DTSS_ADDRESSU,        D3DTADDRESS_CLAMPTOEDGE );
         m_Dev->SetTextureStageState(0, D3DTSS_ADDRESSV,        D3DTADDRESS_CLAMPTOEDGE );

         m_Dev->SetTextureStageState(0, D3DTSS_MAGFILTER,       D3DTEXF_LINEAR );
         m_Dev->SetTextureStageState(0, D3DTSS_MINFILTER,       D3DTEXF_LINEAR );
         m_Dev->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
         m_Dev->SetTextureStageState(0, D3DTSS_RESULTARG,       D3DTA_CURRENT);
         m_Dev->SetTextureStageState(0, D3DTSS_MIPMAPLODBIAS,   0);
         m_Dev->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
         m_Dev->SetTexture(0, reinterpret_cast<IDirect3DTexture8 *>(DxBackBuffer) );
         m_Dev->SetTexture(1, 0 );

         m_Dev->SetRenderState(D3DRS_ZWRITEENABLE,        FALSE);
         m_Dev->SetRenderState(D3DRS_ZFUNC,               D3DCMP_ALWAYS);
                                                       
         m_Dev->SetRenderState(D3DRS_SRCBLEND,            D3DBLEND_ONE);
         m_Dev->SetRenderState(D3DRS_DESTBLEND,           D3DBLEND_ZERO);
         m_Dev->SetRenderState(D3DRS_ALPHABLENDENABLE,    FALSE);
         m_Dev->SetRenderState(D3DRS_ALPHATESTENABLE,     FALSE);
         m_Dev->SetRenderState(D3DRS_CULLMODE,            D3DCULL_NONE);
                                                       
         m_Dev->SetRenderState(D3DRS_FOGCOLOR,            FALSE);
         m_Dev->SetRenderState(D3DRS_FOGENABLE,           FALSE);
         m_Dev->SetRenderState(D3DRS_COLORVERTEX,         FALSE);

         m_Dev->SetRenderState(D3DRS_AMBIENT,             FALSE);
         m_Dev->SetRenderState(D3DRS_BLENDCOLOR,          0);
         m_Dev->SetRenderState(D3DRS_BLENDOP,             D3DBLENDOP_ADD);
         m_Dev->SetRenderState(D3DRS_SPECULARENABLE,      FALSE);
         m_Dev->SetRenderState(D3DRS_LIGHTING,            FALSE);
         D3DCOLOR Col = 0xffffffff;
         m_Dev->SetRenderState(D3DRS_BLENDCOLOR,          Col);
         m_Dev->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALL);

         m_Dev->SetVertexShader(  D3DFVF_XYZ | D3DFVF_TEX1 );

         // copy from the back buffer to pRaster
         m_Dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &Vertices, sizeof(RenderedVertex));

         m_Dev->SetTexture(0, 0 );

         DxDestSurface->Release();

         // apply the old render states
         m_Dev->ApplyStateBlock (EntryRenderState);
         m_Dev->DeleteStateBlock(EntryRenderState);

         DxBackBuffer->Release();

         // restore the old back buffer
         m_Dev->SetRenderTarget(DxBackBuffer, 0);

         RWS_RETURNVOID();
      }

   } // namespace FX

} //namespace RWS

