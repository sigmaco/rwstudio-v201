/*****************************************************************************
 *
 * File :     CFXSkyMotionBlur.cpp
 *
 * Abstract : Sky motion blur implementation
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
#include "cfxskymotionblur.h"

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/math/maths.h"
#include "framework/mainloop/render.h"


extern RwRaster   *SkyFrameRaster;

namespace RWS
{

   namespace FX 
   {
                            
      /**
	   *
	   *	Construct a CFXSkyMotionBlur object.
	   *
	   */
      CFXSkyMotionBlur::CFXSkyMotionBlur() : m_pFrameRaster(0)
      {
         RWS_FUNCTION("RWS::FX::CFXSkyMotionBlur::CFXSkyMotionBlur");

         m_UpdateBlendPoly = TRUE;

         RWS_RETURNVOID();         
      }

      /**
      *
      *  Destroy a CFXSkyMotionBlur object.
      *
      */
      CFXSkyMotionBlur::~CFXSkyMotionBlur()
      {
         RWS_FUNCTION("RWS::FX::CFXSkyMotionBlur::~CFXSkyMotionBlur");

         
         RWS_RETURNVOID();
      }


      /**
      *
      *  Sets up the polygon used to perform the motion blur
      * \param pCamera       Pointer to an RwCamera object used to perform the motion blur.
      *
      */
      void CFXSkyMotionBlur::CreateBlendPoly(RwCamera *pCamera)
      {
         RWS_FUNCTION("RWS::FX::CFXSkyMotionBlur::CreateBlendPoly");

         RwReal         ScreenZ, RecipCameraZ;
         RwV3d          TVert;
         RwReal         HalfWidth, HalfHeight;
         RwRaster       *CameraRaster;
         RwReal         u1, v1, u2, v2;
         RwIm2DVertex   *VertexPtr;
         RwReal         EdgeInwards = 1.0f;

         RwVideoMode VideoMode;

         RwEngineGetVideoModeInfo(&VideoMode, RwEngineGetCurrentVideoMode());

         RwReal ScnHWidth  = VideoMode.width  / 2.0f;
         RwReal ScnHHeight = VideoMode.height / 2.0f;

         CameraRaster = RwCameraGetRaster(pCamera);

         ScreenZ      = RwIm2DGetNearScreenZ();
         RecipCameraZ = 1.0f / RwCameraGetNearClipPlane(pCamera);

         HalfWidth    = RwRasterGetWidth (m_pFrameRaster) / 2.0f;
         HalfHeight   = RwRasterGetHeight(m_pFrameRaster) / 2.0f;

         CalcTransformationMatrix(m_RotMat, ScnHWidth, ScnHHeight);
         
         // adjust for center of texel 0.5 / Raster Width
         u1 = 0.25f / HalfWidth;
         v1 = 0.25f / HalfHeight;

         //  (VideoMode.width / Raster Width) + half texel
         u2 = ((0.5f / HalfWidth)  * (VideoMode.width  - EdgeInwards)) + u1;
         v2 = ((0.5f / HalfHeight) * (VideoMode.height - EdgeInwards)) + v1;

         // set all the components common to each vertex
         RwUInt32 i;
         RwInt32  BlendValue = static_cast<RwInt32>(m_BlurAmount * 255.0f);
         VertexPtr = m_Vertex;
         for (i = 0 ; i < 4 ; i++)
         {
            RwIm2DVertexSetScreenZ     (VertexPtr, ScreenZ );
            RwIm2DVertexSetRecipCameraZ(VertexPtr, RecipCameraZ);
            RwIm2DVertexSetIntRGBA     (VertexPtr, 255, 255, 255, BlendValue);
            VertexPtr++;
         }

         // create the rest of the polygon
         // vert 1                         
         VertexPtr = m_Vertex;
         TVert = TransformMotionVert(m_RotMat, -ScnHWidth, -ScnHHeight, 0.0f);
         RwIm2DVertexSetScreenX     (VertexPtr, TVert.x);
         RwIm2DVertexSetScreenY     (VertexPtr, TVert.y);
         RwIm2DVertexSetU           (VertexPtr, u1, 1.0f);
         RwIm2DVertexSetV           (VertexPtr, v1, 1.0f);
         VertexPtr++;

         // vert 2
         TVert = TransformMotionVert(m_RotMat, ScnHWidth - EdgeInwards, -ScnHHeight, 0.0f);
         RwIm2DVertexSetScreenX     (VertexPtr, TVert.x);
         RwIm2DVertexSetScreenY     (VertexPtr, TVert.y);
         RwIm2DVertexSetU           (VertexPtr, u2, 1.0f);
         RwIm2DVertexSetV           (VertexPtr, v1, 1.0f);
         VertexPtr++;

         // vert 3
         TVert = TransformMotionVert(m_RotMat, -ScnHWidth, ScnHHeight - EdgeInwards, 0.0f);
         RwIm2DVertexSetScreenX     (VertexPtr, TVert.x);
         RwIm2DVertexSetScreenY     (VertexPtr, TVert.y);
         RwIm2DVertexSetU           (VertexPtr, u1, 1.0f);
         RwIm2DVertexSetV           (VertexPtr, v2, 1.0f);
         VertexPtr++;

         // vert 3
         TVert = TransformMotionVert(m_RotMat, ScnHWidth - EdgeInwards, ScnHHeight - EdgeInwards, 0.0f);
         RwIm2DVertexSetScreenX     (VertexPtr, TVert.x);
         RwIm2DVertexSetScreenY     (VertexPtr, TVert.y);
         RwIm2DVertexSetU           (VertexPtr, u2, 1.0f);
         RwIm2DVertexSetV           (VertexPtr, v2, 1.0f);

         RWS_RETURNVOID();                                  
      }

      /**
      *
      *  Blends the front buffer into the back buffer.
      *  \param pCamera    Pointer to RenderWare Graphics RwCamera object.
      *
      */
      void CFXSkyMotionBlur::BlendFrontBuffer(RwCamera *pCamera)
      {
         RWS_FUNCTION("RWS::FX::CFXGameCubeMotionBlur::BlendFrontBuffer");

         RWS_ASSERT((RwEngineGetCurrentVideoMode() & rwVIDEOMODEINTERLACE),
            "Motion blur will only work correctly if the video mode is interlaced");

         if (RwCameraBeginUpdate(pCamera))
         {
            // create the frame raster.
            RwRect rect = {0, 0, 0, 0};
   
            m_pFrameRaster = RwRasterCreate(0, 0, 0, rwRASTERTYPECAMERATEXTURE | rwRASTERDONTALLOCATE);
            RWS_ASSERT(m_pFrameRaster, "Unable to create raster");

            rect.w = RwRasterGetWidth (RwCameraGetRaster( pCamera ));
            rect.h = RwRasterGetHeight(RwCameraGetRaster( pCamera ));

            if (!RwRasterSubRaster(m_pFrameRaster, RwCameraGetRaster( pCamera ), &rect) )
            {
               RWS_ASSERT(0, "Unable to create sub raster");
            }

            // check if we need to create the blend polygon
            if (m_UpdateBlendPoly)
            {
               CreateBlendPoly( pCamera );
               m_UpdateBlendPoly = FALSE;
            }

            // save render states
            RwTexture      *_rwRENDERSTATETEXTURERASTER;
            RwShadeMode     _rwRENDERSTATESHADEMODE;
            RwUInt32        _rwRENDERSTATEVERTEXALPHAENABLE;
            RwUInt32        _rwRENDERSTATEZWRITEENABLE;
            RwUInt32        _rwRENDERSTATEZTESTENABLE;
            RwBlendFunction _rwRENDERSTATESRCBLEND;
            RwBlendFunction _rwRENDERSTATEDESTBLEND;
            RwCullMode      _rwRENDERSTATECULLMODE;
            RwUInt32        _rwTEXTUREADDRESSCLAMP;
            RwUInt32        _rwFILTERNEAREST;
            RwBool          _rwRENDERSTATEFOGENABLE;

            RwRenderStateGet(rwRENDERSTATETEXTURERASTER,       (void *)&_rwRENDERSTATETEXTURERASTER);
            RwRenderStateGet(rwRENDERSTATESHADEMODE,           (void *)&_rwRENDERSTATESHADEMODE);
            RwRenderStateGet(rwRENDERSTATEVERTEXALPHAENABLE,   (void *)&_rwRENDERSTATEVERTEXALPHAENABLE);
            RwRenderStateGet(rwRENDERSTATEZTESTENABLE,         (void *)&_rwRENDERSTATEZWRITEENABLE);
            RwRenderStateGet(rwRENDERSTATEZWRITEENABLE,        (void *)&_rwRENDERSTATEZTESTENABLE);
            RwRenderStateGet(rwRENDERSTATESRCBLEND,            (void *)&_rwRENDERSTATESRCBLEND);
            RwRenderStateGet(rwRENDERSTATEDESTBLEND,           (void *)&_rwRENDERSTATEDESTBLEND);
            RwRenderStateGet(rwRENDERSTATECULLMODE,            (void *)&_rwRENDERSTATECULLMODE);
            RwRenderStateGet(rwRENDERSTATETEXTUREADDRESS,      (void *)&_rwTEXTUREADDRESSCLAMP);
            RwRenderStateGet(rwRENDERSTATETEXTUREFILTER,       (void *)&_rwFILTERNEAREST);
            RwRenderStateGet(rwRENDERSTATEFOGENABLE,           (void *)&_rwRENDERSTATEFOGENABLE); 


            // setup the render states for a textured alpha blended polygon
            RwRenderStateSet(rwRENDERSTATETEXTURERASTER,       m_pFrameRaster);
            RwRenderStateSet(rwRENDERSTATESHADEMODE,           (void *)rwSHADEMODEGOURAUD);
            RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,   (void *)TRUE);
            RwRenderStateSet(rwRENDERSTATEZTESTENABLE,         (void *)FALSE);
            RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,        (void *)FALSE);
            RwRenderStateSet(rwRENDERSTATESRCBLEND,            (void *)rwBLENDSRCALPHA);
            RwRenderStateSet(rwRENDERSTATEDESTBLEND,           (void *)rwBLENDINVSRCALPHA);
            RwRenderStateSet(rwRENDERSTATECULLMODE,            (void *)rwCULLMODECULLNONE);
            RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS,      (void *)rwTEXTUREADDRESSCLAMP);
            RwRenderStateSet(rwRENDERSTATETEXTUREFILTER,       (void *)rwFILTERNEAREST);
            RwRenderStateSet(rwRENDERSTATEFOGENABLE,           (void *)FALSE);
                                
            RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, m_Vertex, 4);

            // restore the render states
            RwRenderStateSet(rwRENDERSTATETEXTURERASTER,       (void *)_rwRENDERSTATETEXTURERASTER);
            RwRenderStateSet(rwRENDERSTATESHADEMODE,           (void *)_rwRENDERSTATESHADEMODE);
            RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,   (void *)_rwRENDERSTATEVERTEXALPHAENABLE);
            RwRenderStateSet(rwRENDERSTATEZTESTENABLE,         (void *)_rwRENDERSTATEZWRITEENABLE);
            RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,        (void *)_rwRENDERSTATEZTESTENABLE);
            RwRenderStateSet(rwRENDERSTATESRCBLEND,            (void *)_rwRENDERSTATESRCBLEND);
            RwRenderStateSet(rwRENDERSTATEDESTBLEND,           (void *)_rwRENDERSTATEDESTBLEND);
            RwRenderStateSet(rwRENDERSTATECULLMODE,            (void *)_rwRENDERSTATECULLMODE);
            RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS,      (void *)_rwTEXTUREADDRESSCLAMP);
            RwRenderStateSet(rwRENDERSTATETEXTUREFILTER,       (void *)_rwFILTERNEAREST);
            RwRenderStateSet(rwRENDERSTATEFOGENABLE,           (void *)_rwRENDERSTATEFOGENABLE); 

            RwCameraEndUpdate(pCamera);

            // destroy the frame raster
            RwRasterDestroy(m_pFrameRaster);
         }

         RWS_RETURNVOID();
      }

   } // namespace FX
} //namespace RWS
