/*****************************************************************************
 *
 * File :     CFXCameraSpaceBloomxbox.cpp
 *
 *
 *****************************************************************************
 *
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd. or
 * Canon Inc. to assist in its use or modification.
 *
 * Criterion Software Ltd. will not, under any
 * circumstances, be liable for any lost revenue or other damages arising
 * from the use of this file.
 *
 * Copyright (c) 2000 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/

#include "precomp.h"
#include "rwcore.h"

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif


//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/crplevel.h"
#include "framework/toolkits/math/maths.h"
#include "framework/mainloop/render.h"
#include "framework/core/resourcemanager/cresourcemanager.h"
#include "../cfxcameraspacebloom.h"
#include "cfxcameraspacebloomxbox.h"
#include "bloomvertexshaderdefs.h"


#include "bloomvertexshader.h"
#include "bloomoutput.h"
#include "extracthighlights.h"

namespace RWS
{
   namespace FX
   {                   
      static DWORD
      g_BloomVertexShaderDeclaration[] =
      {
         D3DVSD_STREAM(0),
         D3DVSD_REG(VSD_REG_POS,       D3DVSDT_FLOAT3),  // POSITION  - register v0
         D3DVSD_REG(VSD_REG_COLOR,     D3DVSDT_FLOAT4),  // COLOR     - register v2
         D3DVSD_REG(VSD_REG_TEXCOORDS, D3DVSDT_FLOAT2),  // TEXCOORDS - register v1
         D3DVSD_END()
      };

      typedef struct RenderedVertex
      {
          RwReal      x, y, z;        // The un-transformed position for the vertex
          RwReal      r, g, b , a;    // colour
          RwReal      u;              // Texture coordinate
          RwReal      v;              // Texture coordinate
      }RenderedVertex;

      CFXCameraSpaceBloomXBox::CFXCameraSpaceBloomXBox()
      {
         RWS_FUNCTION("RWS::FX::CFXCameraSpaceBloomXBox::CFXCameraSpaceBloomXBox");

         // get a pointer to the current device
         m_Dev = static_cast<LPDIRECT3DDEVICE8>(RwXboxGetCurrentD3DDevice());

         // create a vertex buffer
         if (m_Dev->CreateVertexBuffer( 4 * sizeof(RenderedVertex), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED,
                                        &m_FullScreenPolyVertexBufferLinear ) != D3D_OK)
         {
            RWS_RETURNVOID();
         }

         if (D3D_OK != D3DDevice_CreateVertexShader(g_BloomVertexShaderDeclaration,
                                                    dwBloomvertexshaderVertexShader,
                                                    &m_VertexShader,
                                                    0))
         {
            RWS_RETURNVOID();
         }

         m_Dev->CreatePixelShader(reinterpret_cast<D3DPIXELSHADERDEF*>(dwExtracthighlightsPixelShader),
                                  &m_ExtractHighLightsPixelShader );

         m_Dev->CreatePixelShader(reinterpret_cast<D3DPIXELSHADERDEF*>(dwBloomoutputPixelShader),
                                  &m_BloomOutputPixelShader );

         // linear
         {
            RenderedVertex* pVertices;

            D3DVertexBuffer_Lock( m_FullScreenPolyVertexBufferLinear, 0, 0, (RwUInt8**)&pVertices, 0 );

            {
               RenderedVertex V = {-0.5f, -0.5f, 2.0f,    1.0f, 1.0f, 1.0f, 1.0f  ,    0.0f, 480.0f};
               pVertices[0] = V;                                                                 
            }                                                                                    
                                                                                              
            {                                                                                    
               RenderedVertex V = { 0.5f, -0.5f, 2.0f,    1.0f, 1.0f, 1.0f, 1.0f  ,  640.0f, 480.0f};
               pVertices[1] = V;                                                                   
            }                                                                                      
                                                                                                
            {                                                                                      
               RenderedVertex V = {-0.5f,  0.5f, 2.0f,    1.0f, 1.0f, 1.0f, 1.0f  ,    0.0f, 0.0f};
               pVertices[2] = V;                                                                   
            }                                                                                      
                                                                                                
            {                                                                                      
               RenderedVertex V = { 0.5f,  0.5f, 2.0f,    1.0f, 1.0f, 1.0f, 1.0f  ,  640.0f, 0.0f};
               pVertices[3] = V;
            }
            D3DVertexBuffer_Unlock( m_FullScreenPolyVertexBufferLinear );
         }

         // FIXME: We need to get the exact resolution of the current backbuffer here
         m_Dev->CreateTexture(640, 480, 1, 0, D3DFMT_LIN_A8R8G8B8, 0, &m_TempTexture);
         m_TempTexture->GetSurfaceLevel(0, &m_TempSurface);

         RWS_RETURNVOID();
      }

      CFXCameraSpaceBloomXBox::~CFXCameraSpaceBloomXBox()
      {
         RWS_FUNCTION("RWS::FX::CFXCameraSpaceBloomXBox::~CFXCameraSpaceBloomXBox");

         // destroy the highlight shader
         if ( m_ExtractHighLightsPixelShader )
         {
            D3DDevice_DeletePixelShader(m_ExtractHighLightsPixelShader);
         }

         // destroy the bloom shader
         if ( m_BloomOutputPixelShader )
         {
            D3DDevice_DeletePixelShader(m_BloomOutputPixelShader);
         }

         // destroy the pixel shader
         if ( m_VertexShader )
         {
            D3DDevice_DeleteVertexShader(m_VertexShader);
         }

         // destroy the vertex buffer
         if ( m_FullScreenPolyVertexBufferLinear )
         {
            m_FullScreenPolyVertexBufferLinear->Release();
         }

         // release the temp surface
         if ( m_TempSurface )
         {
            m_TempSurface->Release();
         }

         // release the temp texture
         if ( m_TempTexture )
         {
            m_TempTexture->Release();
         }

         RWS_RETURNVOID();
      }

      void CFXCameraSpaceBloomXBox::SaveRenderStates(DWORD *SB)
      {
         RWS_FUNCTION("RWS::FX::CFXCameraSpaceBloomXBox::SaveRenderStates");

         HRESULT  Res;

         Res = m_Dev->CreateStateBlock(D3DSBT_ALL, SB);
         RWS_ASSERT(Res == D3D_OK, "Unable to create stateblock");

         RWS_RETURNVOID();
      }

      void CFXCameraSpaceBloomXBox::RestoreRenderStates(DWORD SB)
      {
         RWS_FUNCTION("RWS::FX::CFXCameraSpaceBloomXBox::RestoreRenderStates");

         m_Dev->ApplyStateBlock(SB);

         RWS_RETURNVOID();
      }

      void CFXCameraSpaceBloomXBox::DeleteRenderStates(DWORD SB)
      {
         RWS_FUNCTION("RWS::FX::CFXCameraSpaceBloomXBox::DeleteRenderStates");

         m_Dev->DeleteStateBlock(SB);

         RWS_RETURNVOID();
      }


      void CFXCameraSpaceBloomXBox::ApplyParallelCamera()
      {
         RWS_FUNCTION("RWS::FX::CFXXBOXMotionBlur::ApplyParallelCamera");

         // setup the camera
         D3DXMATRIX matView;
         D3DXMATRIX matWorld;
         D3DXMATRIX matProj;
         D3DXMATRIX matViewProj;
         D3DXMATRIX matWorldViewProj;

         D3DXMatrixIdentity(&matView);
         D3DXMatrixIdentity(&matWorld);
         D3DXMatrixOrthoLH (&matProj, 1.0f, 1.0f, 0.2f, 20.0f);

         m_Dev->SetTransform(D3DTS_VIEW,       &matView);
         m_Dev->SetTransform(D3DTS_WORLD,      &matWorld);
         m_Dev->SetTransform(D3DTS_PROJECTION, &matProj);


         D3DXMatrixMultiply (&matViewProj,      &matView,   &matProj);
         D3DXMatrixMultiply (&matWorldViewProj, &matWorld,  &matViewProj);
         D3DXMatrixTranspose(&matWorldViewProj, &matWorldViewProj);
         m_Dev->SetVertexShaderConstant(VSCONST_REG_TRANSFORM_OFFSET, &matWorldViewProj, VSCONST_REG_TRANSFORM_SIZE);
         
         RWS_RETURNVOID();
      }

      void CFXCameraSpaceBloomXBox::ApplyHighlight(IDirect3DTexture8 *pTexture)
      {
         RWS_FUNCTION("RWS::FX::CFXXBOXMotionBlur::ApplyHighlightRenderStates");

         // setup the render states for alpha blended textured poly
         m_Dev->SetPixelShader( m_ExtractHighLightsPixelShader );
         m_Dev->SetVertexShader( m_VertexShader );

         RwV4d    ShaderConstant;
         ShaderConstant.x = m_Brightness;
         ShaderConstant.y = m_Brightness;
         ShaderConstant.z = m_Brightness;
         ShaderConstant.w = m_Brightness;
         m_Dev->SetPixelShaderConstant(5, &ShaderConstant, 1 );

         for ( int i = 0 ; i < 4 ; i++ )
         {
            m_Dev->SetTextureStageState(i, D3DTSS_ALPHAOP,         D3DTOP_SELECTARG1);
            m_Dev->SetTextureStageState(i, D3DTSS_ALPHAARG1,       D3DTA_DIFFUSE);
                                                                
            m_Dev->SetTextureStageState(i, D3DTSS_COLOROP,         D3DTOP_SELECTARG2);
            m_Dev->SetTextureStageState(i, D3DTSS_COLORARG1,       D3DTA_DIFFUSE);
            m_Dev->SetTextureStageState(i, D3DTSS_COLORARG2,       D3DTA_TEXTURE);
            m_Dev->SetTextureStageState(i, D3DTSS_ADDRESSU,        D3DTADDRESS_CLAMPTOEDGE );
            m_Dev->SetTextureStageState(i, D3DTSS_ADDRESSV,        D3DTADDRESS_CLAMPTOEDGE );

            m_Dev->SetTextureStageState(i, D3DTSS_MAGFILTER,       D3DTEXF_GAUSSIANCUBIC );
            m_Dev->SetTextureStageState(i, D3DTSS_MINFILTER,       D3DTEXF_GAUSSIANCUBIC );
            m_Dev->SetTextureStageState(i, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
            m_Dev->SetTextureStageState(i, D3DTSS_RESULTARG,       D3DTA_CURRENT);
            m_Dev->SetTextureStageState(i, D3DTSS_MIPMAPLODBIAS,   0);
            m_Dev->SetTextureStageState(i, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
            m_Dev->SetTexture(i, pTexture);
         }

         m_Dev->SetRenderState(D3DRS_ZWRITEENABLE,        FALSE);
         m_Dev->SetRenderState(D3DRS_ZFUNC,               D3DCMP_ALWAYS);
                                                          
         m_Dev->SetRenderState(D3DRS_SRCBLEND,            D3DBLEND_SRCALPHA);
         m_Dev->SetRenderState(D3DRS_DESTBLEND,           D3DBLEND_INVSRCALPHA);
         m_Dev->SetRenderState(D3DRS_ALPHABLENDENABLE,    FALSE);
         m_Dev->SetRenderState(D3DRS_ALPHATESTENABLE,     FALSE);
         m_Dev->SetRenderState(D3DRS_CULLMODE,            D3DCULL_NONE);
                                                          
         m_Dev->SetRenderState(D3DRS_FOGCOLOR,            FALSE);
         m_Dev->SetRenderState(D3DRS_AMBIENT,             FALSE);
         m_Dev->SetRenderState(D3DRS_BLENDCOLOR,          0);
         m_Dev->SetRenderState(D3DRS_BLENDOP,             D3DBLENDOP_ADD);
         m_Dev->SetRenderState(D3DRS_SPECULARENABLE,      FALSE);
         m_Dev->SetRenderState(D3DRS_LIGHTING,            FALSE);

         // align all four full screen stages
         {
            RwReal stageOffset[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
            for (int i = 0; i < 4; i++ )
            {

               m_Dev->SetVertexShaderConstant( VSCONST_REG_T0_OFFSET + i * VSCONST_REG_T0_SIZE, stageOffset,
                                               VSCONST_REG_T0_SIZE );
            }
         }

         m_Dev->SetStreamSource(0, m_FullScreenPolyVertexBufferLinear, sizeof(RenderedVertex));
         m_Dev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

         // set the current texture to be NULL, in doing so decrement FrontBuffer Refcount
         m_Dev->SetTexture(0, 0);

         RWS_RETURNVOID();
      }


      void CFXCameraSpaceBloomXBox::ApplyToOutput(IDirect3DTexture8 *pTexture, RwReal UVOffset)
      {
         RWS_FUNCTION("RWS::FX::CFXXBOXMotionBlur::ApplyHighlightRenderStates");

         // setup the render states for alpha blended textured poly
         m_Dev->SetVertexShader( m_VertexShader );

         for ( int i = 0 ; i < 4 ; i++ )
         {
            m_Dev->SetTextureStageState(i, D3DTSS_ALPHAOP,         D3DTOP_SELECTARG1);
            m_Dev->SetTextureStageState(i, D3DTSS_ALPHAARG1,       D3DTA_DIFFUSE);
                                                                
            m_Dev->SetTextureStageState(i, D3DTSS_COLOROP,         D3DTOP_SELECTARG2);
            m_Dev->SetTextureStageState(i, D3DTSS_COLORARG1,       D3DTA_DIFFUSE);
            m_Dev->SetTextureStageState(i, D3DTSS_COLORARG2,       D3DTA_TEXTURE);
            m_Dev->SetTextureStageState(i, D3DTSS_ADDRESSU,        D3DTADDRESS_CLAMPTOEDGE );
            m_Dev->SetTextureStageState(i, D3DTSS_ADDRESSV,        D3DTADDRESS_CLAMPTOEDGE );

            m_Dev->SetTextureStageState(i, D3DTSS_MAGFILTER,       D3DTEXF_GAUSSIANCUBIC );
            m_Dev->SetTextureStageState(i, D3DTSS_MINFILTER,       D3DTEXF_GAUSSIANCUBIC );
            m_Dev->SetTextureStageState(i, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
            m_Dev->SetTextureStageState(i, D3DTSS_RESULTARG,       D3DTA_CURRENT);
            m_Dev->SetTextureStageState(i, D3DTSS_MIPMAPLODBIAS,   0);
            m_Dev->SetTextureStageState(i, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
            m_Dev->SetTexture(i, pTexture);
         }

         m_Dev->SetRenderState(D3DRS_ZWRITEENABLE,        FALSE);
         m_Dev->SetRenderState(D3DRS_ZFUNC,               D3DCMP_ALWAYS);
                                                          
         m_Dev->SetRenderState(D3DRS_SRCBLEND,            D3DBLEND_SRCALPHA);
         m_Dev->SetRenderState(D3DRS_DESTBLEND,           D3DBLEND_ONE);

         m_Dev->SetRenderState(D3DRS_ALPHABLENDENABLE,    TRUE);
         m_Dev->SetRenderState(D3DRS_ALPHATESTENABLE,     FALSE);
         m_Dev->SetRenderState(D3DRS_CULLMODE,            D3DCULL_NONE);
                                                          
         m_Dev->SetRenderState(D3DRS_FOGCOLOR,            FALSE);
         m_Dev->SetRenderState(D3DRS_AMBIENT,             FALSE);
         m_Dev->SetRenderState(D3DRS_BLENDCOLOR,          0);
         m_Dev->SetRenderState(D3DRS_BLENDOP,             D3DBLENDOP_ADD);
         m_Dev->SetRenderState(D3DRS_SPECULARENABLE,      FALSE);
         m_Dev->SetRenderState(D3DRS_LIGHTING,            FALSE);

         // offset the uv coordinates for each texture stage
         // the offsets will form a star pattern.
         {
            {
               RwReal stageOffset[4] = { - UVOffset,  - UVOffset , 0.0f, 0.0f };
               m_Dev->SetVertexShaderConstant(  VSCONST_REG_T0_OFFSET + 0 * VSCONST_REG_T0_SIZE,
                                                stageOffset, VSCONST_REG_T0_SIZE );
            }
            {
               RwReal stageOffset[4] = {   UVOffset,  - UVOffset, 0.0f, 0.0f };
               m_Dev->SetVertexShaderConstant(  VSCONST_REG_T1_OFFSET + 0 * VSCONST_REG_T0_SIZE,
                                                stageOffset, VSCONST_REG_T1_SIZE );
            }
            {
               RwReal stageOffset[4] = {   UVOffset,    UVOffset, 0.0f, 0.0f };
               m_Dev->SetVertexShaderConstant(  VSCONST_REG_T2_OFFSET + 0 * VSCONST_REG_T0_SIZE,
                                                stageOffset, VSCONST_REG_T2_SIZE );
            }
            {
               RwReal stageOffset[4] = { - UVOffset,    UVOffset, 0.0f, 0.0f };
               m_Dev->SetVertexShaderConstant(  VSCONST_REG_T3_OFFSET + 0 * VSCONST_REG_T0_SIZE,
                                                stageOffset, VSCONST_REG_T3_SIZE );
            }
         }

         m_Dev->SetStreamSource(0, m_FullScreenPolyVertexBufferLinear, sizeof(RenderedVertex));
         m_Dev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

         // set the current texture to be NULL, in doing so decrement FrontBuffer Refcount
         m_Dev->SetTexture( 0, 0 );

         RWS_RETURNVOID();
      }


      void CFXCameraSpaceBloomXBox::ProcessCamera(RwCamera *pCamera)
      {
         RWS_FUNCTION("RWS::FX::CFXCameraSpaceBloomXBox::ProcessCamera");


         DWORD                EntryRenderState;
         LPDIRECT3DSURFACE8   DxBackBuffer;

         SaveRenderStates(&EntryRenderState);

         ApplyParallelCamera();

         m_Dev->GetBackBuffer(0, 0, &DxBackBuffer);

         // create the bloom
         IDirect3DSurface8 *Stencil;

         m_Dev->GetDepthStencilSurface(&Stencil);
         m_Dev->SetRenderTarget(m_TempSurface, Stencil);
         ApplyHighlight(reinterpret_cast<IDirect3DTexture8 *>(DxBackBuffer));
         Stencil->Release();

         m_Dev->GetDepthStencilSurface(&Stencil);
         {

            RwV4d    ShaderConstant;

            for ( RwUInt32 i = 0 ; i<m_Separation ; i += m_SeparationStep)
            {
               float u;

               u = static_cast<float>( 1.0f / m_Separation) * i;

               ShaderConstant.x = ( (m_EndColor.red   - m_StartColor.red)   * u ) + m_StartColor.red;
               ShaderConstant.y = ( (m_EndColor.green - m_StartColor.green) * u ) + m_StartColor.green;
               ShaderConstant.z = ( (m_EndColor.blue  - m_StartColor.blue)  * u ) + m_StartColor.blue;
               ShaderConstant.w = ( (m_EndColor.alpha - m_StartColor.alpha) * u ) + m_StartColor.alpha;

               m_Dev->SetPixelShader( m_BloomOutputPixelShader );
               m_Dev->SetPixelShaderConstant(1, &ShaderConstant, 1 );

               // copy back to the screen
               m_Dev->SetRenderTarget(DxBackBuffer, Stencil);
               ApplyToOutput(static_cast<IDirect3DTexture8*>(m_TempTexture), static_cast<float>(i) );
            }
         }

         Stencil->Release();

         DxBackBuffer->Release();
   
         RestoreRenderStates(EntryRenderState);
         DeleteRenderStates(EntryRenderState);

         RWS_RETURNVOID();
      }

   } // FX

} // RWS
