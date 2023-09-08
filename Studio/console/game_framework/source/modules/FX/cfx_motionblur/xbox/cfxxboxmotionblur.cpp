/*****************************************************************************
 *
 * File :     CFXXBOXMotionBlur.cpp
 *
 * Abstract : XBox motion blur implementation
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
#endif


//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cfxxboxmotionblur.h"

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/math/maths.h"
#include "framework/mainloop/render.h"
#include "framework/core/resourcemanager/cresourcemanager.h"


namespace RWS
{

   namespace FX 
   {

      /**
      *
      *  Construct a CFXXBOXMotionBlur object.
      *
      */
      CFXXBOXMotionBlur::CFXXBOXMotionBlur()
      {
         RWS_FUNCTION("RWS::FX::CFXXBOXMotionBlur::CFXXBOXMotionBlur");

         // get a pointer to the current device
         m_Dev = static_cast<LPDIRECT3DDEVICE8>(RwXboxGetCurrentD3DDevice());

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CFXXBOXMotionBlur object.
      *
      */
      CFXXBOXMotionBlur::~CFXXBOXMotionBlur()
      {
         RWS_FUNCTION("RWS::FX::CFXXBOXMotionBlur::~CFXXBOXMotionBlur");


         RWS_RETURNVOID();
      }

      /**
      *
      *  Creates a state block containing the current state of the GPU.  
      *
      * \param SB Pointer to a DWORD used to store the state block handle
      *  
      */
      void CFXXBOXMotionBlur::CreateSavedRenderState(DWORD *SB)
      {
         RWS_FUNCTION("RWS::FX::CFXXBOXMotionBlur::CreateSavedRenderState");

         HRESULT  Res;

         Res = m_Dev->CreateStateBlock(D3DSBT_ALL, SB);
         RWS_ASSERT(Res == D3D_OK, "Unable to create stateblock");

         RWS_RETURNVOID();
      }

      /**
      *
      *  Applies a state block. 
      *
      * \param SB Handle of the state block to apply
      *  
      */
      void CFXXBOXMotionBlur::RestoreRenderState(DWORD SB)
      {
         RWS_FUNCTION("RWS::FX::CFXXBOXMotionBlur::RestoreRenderState");

         m_Dev->ApplyStateBlock(SB);

         RWS_RETURNVOID();
      }

      /**
      *
      *  Deletes a state block.  
      *
      * \param SB Handle of state block to delete
      *  
      */
      void CFXXBOXMotionBlur::DeleteSavedRenderState(DWORD SB)
      {
         RWS_FUNCTION("RWS::FX::CFXXBOXMotionBlur::DeleteSavedRenderState");

         m_Dev->DeleteStateBlock(SB);

         RWS_RETURNVOID();
      }

      /**
      *  Sets up the polygon used to perform the motion blur
      * \param pCamera  Pointer to an RwCamera object used to perform the motion blur.
      *
      */
      void CFXXBOXMotionBlur::CreateBlendPoly(RwCamera *pCamera)
      {
         RWS_FUNCTION("RWS::FX::CFXXBOXMotionBlur::CreateBlendPoly");

         // create transformation matrix used to transform the blend poly
         t_FullScreenVert *pVertices = m_Verts;
         RwV3d             Angle     = {0.0f, 0.0f, 1.0f};
         RwV3d             Pos;
         const RwReal      Top       = -1.0f;
         const RwReal      Bottom    =  1.0f;
         const RwReal      Left      = -1.0f;
         const RwReal      Right     =  1.0f;
         const RwReal      ZDist     =  0.3f;  
         const RwReal      HalfTexel =  0.5f;  


         // get the width and height of the current render target, we need to do this as the render target
         //is linear we need to specify uvs in texels 
         float Width  = static_cast<RwReal>(RwRasterGetWidth (RwCameraGetRaster(pCamera)));
         float Height = static_cast<RwReal>(RwRasterGetHeight(RwCameraGetRaster(pCamera)));


         Pos.x = m_OffsetX;
         Pos.y = m_OffsetY;
         Pos.z = 0.0f;
         RwMatrixTranslate(m_RotMat, &Pos, rwCOMBINEREPLACE); 

         RwMatrixRotate(m_RotMat, &Angle, m_Angle, rwCOMBINEPOSTCONCAT); 

         RwV3d    Scale;
         Scale.x = (   m_ScaleX) * m_Scale;
         Scale.y = ( - m_ScaleY) * m_Scale;
         Scale.z = 1.0f;
         RwMatrixScale (m_RotMat, &Scale, rwCOMBINEPOSTCONCAT); 

         // build up the blend poly vertices
         // top left
         pVertices->Pos = TransformMotionVert ( m_RotMat, Left,  Top,    ZDist );
         pVertices->u   = HalfTexel;
         pVertices->v   = HalfTexel;
         pVertices++;
                                                       
         // top right                                         
         pVertices->Pos = TransformMotionVert ( m_RotMat, Right, Top,    ZDist );
         pVertices->u   = Width + HalfTexel;
         pVertices->v   = HalfTexel;
         pVertices++;
                                                       
         // bottom left                                        
         pVertices->Pos = TransformMotionVert ( m_RotMat, Left,  Bottom, ZDist );
         pVertices->u   = HalfTexel;
         pVertices->v   = Height + HalfTexel;
         pVertices++;
                                                       
         // bottom right                                        
         pVertices->Pos = TransformMotionVert ( m_RotMat, Right, Bottom, ZDist );
         pVertices->u   = Width  + HalfTexel;
         pVertices->v   = Height + HalfTexel;

         RWS_RETURNVOID();
      }

      /**
      *  Applies the renderstates used for the motion blur effect.
      *
      */
      void CFXXBOXMotionBlur::ApplyMotionBlurRenderStates()
      {
         RWS_FUNCTION("RWS::FX::CFXXBOXMotionBlur::ApplyMotionBlurRenderStates");

         // setup the render states for alpha blended textured poly
         m_Dev->SetPixelShader(0);
         m_Dev->SetVertexShader(  D3DFVF_XYZ | D3DFVF_TEX1 );

         m_Dev->SetTextureStageState  (0, D3DTSS_ALPHAOP,         D3DTOP_SELECTARG1);
         m_Dev->SetTextureStageState  (0, D3DTSS_ALPHAARG1,       D3DTA_TFACTOR);
                                                                  
         m_Dev->SetTextureStageState  (0, D3DTSS_COLOROP,         D3DTOP_SELECTARG2);
         m_Dev->SetTextureStageState  (0, D3DTSS_COLORARG1,       D3DTA_DIFFUSE);
         m_Dev->SetTextureStageState  (0, D3DTSS_COLORARG2,       D3DTA_TEXTURE);
         m_Dev->SetTextureStageState  (0, D3DTSS_ADDRESSU,        D3DTADDRESS_CLAMPTOEDGE );
         m_Dev->SetTextureStageState  (0, D3DTSS_ADDRESSV,        D3DTADDRESS_CLAMPTOEDGE );

         m_Dev->SetTextureStageState  (0, D3DTSS_MAGFILTER,       D3DTEXF_POINT );
         m_Dev->SetTextureStageState  (0, D3DTSS_MINFILTER,       D3DTEXF_POINT );
         m_Dev->SetTextureStageState  (0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
         m_Dev->SetTextureStageState  (0, D3DTSS_RESULTARG,       D3DTA_CURRENT);
         m_Dev->SetTextureStageState  (0, D3DTSS_MIPMAPLODBIAS,   0);
         m_Dev->SetTextureStageState  (0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
         m_Dev->SetTextureStageState  (1, D3DTSS_COLOROP,         D3DTOP_DISABLE);

         m_Dev->SetRenderState        (D3DRS_ZWRITEENABLE,        FALSE);
         m_Dev->SetRenderState        (D3DRS_ZFUNC,               D3DCMP_ALWAYS);
                                                                  
         m_Dev->SetRenderState        (D3DRS_SRCBLEND,            D3DBLEND_SRCALPHA);
         m_Dev->SetRenderState        (D3DRS_DESTBLEND,           D3DBLEND_INVSRCALPHA);
         m_Dev->SetRenderState        (D3DRS_ALPHABLENDENABLE,    TRUE);
         m_Dev->SetRenderState        (D3DRS_ALPHATESTENABLE,     FALSE);
         m_Dev->SetRenderState        (D3DRS_CULLMODE,            D3DCULL_NONE);
                                                                  
         m_Dev->SetRenderState        (D3DRS_FOGCOLOR,            FALSE);
         m_Dev->SetRenderState        (D3DRS_AMBIENT,             FALSE);
         m_Dev->SetRenderState        (D3DRS_BLENDCOLOR,          0);
         m_Dev->SetRenderState        (D3DRS_BLENDOP,             D3DBLENDOP_ADD);
         m_Dev->SetRenderState        (D3DRS_SPECULARENABLE,      FALSE);
         m_Dev->SetRenderState        (D3DRS_LIGHTING,            FALSE);
         m_Dev->SetTexture            (0, 0);
         m_Dev->SetTexture            (1, 0);


         // setup the camera
         D3DXMATRIX     MatView;
         D3DXMATRIX     MatProj;
         const RwReal   NearZ =  0.2f;
         const RwReal   FarZ  = 20.0f;
         const RwReal   Width =  2.0f;
         const RwReal   Height =  2.0f;

         D3DXMatrixIdentity(&MatView);
         D3DXMatrixOrthoLH (&MatProj, Width, Height, NearZ, FarZ);

         m_Dev->SetTransform(D3DTS_VIEW,       &MatView);
         m_Dev->SetTransform(D3DTS_WORLD,      &MatView);
         m_Dev->SetTransform(D3DTS_PROJECTION, &MatProj);
         RWS_RETURNVOID();
      }

      /**
      *  Blends the front buffer into the back buffer.
      *
      */
      void CFXXBOXMotionBlur::BlendFrontBuffer(RwCamera *pCamera)
      {
         RWS_FUNCTION("RWS::FX::CFXXBOXMotionBlur::BlendFrontBuffer");
         DWORD                SavedRenderState;
         LPDIRECT3DSURFACE8   FrontBuffer;

         // save the state of the GPU so we can put render states back as we found them
         CreateSavedRenderState(&SavedRenderState);

         if (m_UpdateBlendPoly)
         {
            CreateBlendPoly(pCamera);
            m_UpdateBlendPoly = FALSE;
         }
  
         m_Dev->GetBackBuffer(-1, 0, &FrontBuffer);

         // setup the render states for the alpha blended polygon
         ApplyMotionBlurRenderStates();


         // set the current texture to the front buffer
         m_Dev->SetTexture(0, (IDirect3DBaseTexture8*)FrontBuffer);
         m_Dev->SetRenderState (D3DRS_TEXTUREFACTOR, D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, m_BlurAmount) );
         m_Dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &m_Verts, sizeof(t_FullScreenVert));

         // set the current texture to be NULL, in doing so decrement FrontBuffer Refcount
         m_Dev->SetTexture(0, 0);


         // restore the RenderWare states
         RestoreRenderState(SavedRenderState);

         // delete the saved state block
         DeleteSavedRenderState(SavedRenderState);

         FrontBuffer->Release();

         RWS_RETURNVOID();
      }

   } // namespace FX
} //namespace RWS
