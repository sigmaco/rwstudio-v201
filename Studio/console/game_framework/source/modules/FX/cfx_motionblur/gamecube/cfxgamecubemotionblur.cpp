/*****************************************************************************
 *
 * File :     CFXGameCubeMotionBlur.cpp
 *
 * Abstract : A class specifically for the GameCube that can be used to blend the
 *            previous frame with the current frame to produce various motion
 *            blur effects.
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
#include "cfxgamecubemotionblur.h"

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/math/maths.h"
#include "framework/mainloop/render.h"

namespace RWS
{

   namespace FX 
   {
      /**
      *
      *  Construct a CFXGameCubeMotionBlur object.
      *
      */
      CFXGameCubeMotionBlur::CFXGameCubeMotionBlur()
      {
         RWS_FUNCTION("RWS::FX::CFXGameCubeMotionBlur::CFXGameCubeMotionBlur");
         RwRaster *CameraRaster;
         RwCamera *Camera;

         Camera = MainLoop::Render::GetCamera();

         CameraRaster  = RwCameraGetRaster(Camera);

         // create a raster in system memory used to store the embedded from buffer
         m_pFrameRaster = RwRasterCreate(  CameraRaster->width,
                                          CameraRaster->height,
                                          CameraRaster->depth,
                                          rwRASTERTYPECAMERATEXTURE );

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CFXGameCubeMotionBlur object.
      *
      */
      CFXGameCubeMotionBlur::~CFXGameCubeMotionBlur()
      {
         RWS_FUNCTION("RWS::FX::CFXGameCubeMotionBlur::~CFXGameCubeMotionBlur");

         RwRasterDestroy(m_pFrameRaster);

         RWS_RETURNVOID();
      }

      /**
      *
      *  Sets up the polygon used to perform the motion blur
      * \param pCamera       Pointer to an RwCamera object used to perform the motion blur.
      *
      */
      void CFXGameCubeMotionBlur::CreateBlendPoly(RwCamera *pCamera)
      {
         RWS_FUNCTION("RWS::FX::CFXGameCubeMotionBlur::CreateBlendPoly");

         RwReal         ScreenZ, RecipCameraZ;
         RwV3d          TVert;
         RwReal         HalfWidth, HalfHeight;
         RwRaster       *CameraRaster;
         RwReal         U1, V1, U2, V2;
         RwIm2DVertex   *VertexPtr;

         CameraRaster = RwCameraGetRaster(pCamera);

         ScreenZ      = RwIm2DGetNearScreenZ();
         RecipCameraZ = 1.0f / static_cast<RwReal>(RwCameraGetNearClipPlane(pCamera));

         HalfWidth    = static_cast<RwReal>(RwRasterGetWidth (CameraRaster)) / 2.0f;
         HalfHeight   = static_cast<RwReal>(RwRasterGetHeight(CameraRaster)) / 2.0f;

         CalcTransformationMatrix(m_RotMat, HalfWidth, HalfHeight);
         
         // add half a texel to each texture coordinate
         U1 = (0.25f / HalfWidth) ;
         V1 = (0.25f / HalfHeight);

         U2 = 1.0f + U1;
         V2 = 1.0f + V1;
              
         U1 = 0.0f;
         V1 = 0.0f;

         RwUInt8  BlendValue = static_cast<RwUInt8>(m_BlurAmount * 255.0f);

         // set all the components common to each vertex
         int   i;
         VertexPtr = m_Vertex;
         for (i = 0 ; i < 4 ; i++)
         {
            RwIm2DVertexSetScreenZ     (VertexPtr, ScreenZ );
            RwIm2DVertexSetRecipCameraZ(VertexPtr, RecipCameraZ);
            RwIm2DVertexSetIntRGBA(VertexPtr, 255, 255, 255, BlendValue);
            VertexPtr++;
         }

         // create the rest of the polygon
         // vert 1                         
         VertexPtr = m_Vertex;
         TVert = TransformMotionVert(m_RotMat, -HalfWidth, -HalfHeight, 0.0f);
         RwIm2DVertexSetScreenX     (VertexPtr, TVert.x);
         RwIm2DVertexSetScreenY     (VertexPtr, TVert.y);
         RwIm2DVertexSetU           (VertexPtr, U1, 1.0f);
         RwIm2DVertexSetV           (VertexPtr, V1, 1.0f);
         VertexPtr++;

         // vert 2
         TVert = TransformMotionVert(m_RotMat, HalfWidth, -HalfHeight, 0.0f);
         RwIm2DVertexSetScreenX     (VertexPtr, TVert.x);
         RwIm2DVertexSetScreenY     (VertexPtr, TVert.y);
         RwIm2DVertexSetU           (VertexPtr, U2, 1.0f);
         RwIm2DVertexSetV           (VertexPtr, V1, 1.0f);
         VertexPtr++;

         // vert 3
         TVert = TransformMotionVert(m_RotMat, -HalfWidth, HalfHeight, 0.0f);
         RwIm2DVertexSetScreenX     (VertexPtr, TVert.x);
         RwIm2DVertexSetScreenY     (VertexPtr, TVert.y);
         RwIm2DVertexSetU           (VertexPtr, U1, 1.0f);
         RwIm2DVertexSetV           (VertexPtr, V2, 1.0f);
         VertexPtr++;

         // vert 3
         TVert = TransformMotionVert(m_RotMat, HalfWidth, HalfHeight, 0.0f);
         RwIm2DVertexSetScreenX     (VertexPtr, TVert.x);
         RwIm2DVertexSetScreenY     (VertexPtr, TVert.y);
         RwIm2DVertexSetU           (VertexPtr, V2, 1.0f);
         RwIm2DVertexSetV           (VertexPtr, V2, 1.0f);

         RWS_RETURNVOID();                                  
      }


      /**
      *
      *  Blends the front buffer into the back buffer.
      *  \param pCamera  Pointer to an RwCamera object used to perform the motion blur.
      *
      */
      void CFXGameCubeMotionBlur::BlendFrontBuffer(RwCamera *pCamera)
      {
         RWS_FUNCTION("RWS::FX::CFXGameCubeMotionBlur::BlendFrontBuffer");

         if (RwCameraBeginUpdate(pCamera))
         {
            if (m_UpdateBlendPoly)
            {
               CreateBlendPoly(pCamera);
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

            // copy the embedded frame buffer to the frame raster in system memory
            RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,   (void *)FALSE);

            RwGameCubeCameraTextureFlush(m_pFrameRaster, FALSE); 

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
         }

         RWS_RETURNVOID();
      }


   } // namespace FX
} //namespace RWS
