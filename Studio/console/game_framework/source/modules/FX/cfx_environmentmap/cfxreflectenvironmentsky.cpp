/*****************************************************************************
 *
 * File :     cfxreflectenvironment.h
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
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif
#include "rwcore.h"
/*
 * For extern sceGsDBuffDc _rwDMAFlipData...
 */
#include "libgraph.h"

#include "rpmatfx.h"

#include "framework/toolkits/world/clevel.h"
#include "CFXReflectEnvironmentSky.h"
#include "framework/core/resourcemanager/cresourcemanager.h"
#include "framework/mainloop/render.h"

#include "cfxreflectenvironment.h"

namespace RWS
{                    

   namespace FX
   {

      /**
      *
      *  Construct a CFXReflectEnvironmentSky object.
      *
      */
      CFXReflectEnvironmentSky::CFXReflectEnvironmentSky()
      {
         RWS_FUNCTION("RWS::FX::CFXReflectEnvironmentSky::CFXReflectEnvironmentSky");

         m_RasterZ = RwRasterCreate(static_cast<RwUInt32>(ENV_MAP_WIDTH), static_cast<RwUInt32>(ENV_MAP_HEIGHT),
                                    0, rwRASTERTYPEZBUFFER);

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy a CFXReflectEnvironmentSky object.
      *
      */
      CFXReflectEnvironmentSky::~CFXReflectEnvironmentSky()
      {
         RWS_FUNCTION("RWS::FX::CFXReflectEnvironmentSky::~CFXReflectEnvironmentSky");

         RwRasterDestroy( m_RasterZ );

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
      void CFXReflectEnvironmentSky::UpdateTextureFromDisplay(const RwV2d &TopLeftUV, const RwV2d &BottomRightUV,
                                                              const RwRaster *pRaster)
      {
         RWS_FUNCTION("RWS::FX::CFXReflectEnvironmentSky::UpdateTextureFromDisplay");

         RwCamera *pCamera;
         RwRaster *pFrameRaster;

         if ( !(pCamera = RWS::MainLoop::Render::GetCamera() ) )
         {
            RWS_RETURNVOID();
         }

         RWS_ASSERT(!RwCameraGetCurrentCamera(), "UpdateTextureFromDisplay can not be called between \
                                                  RwCameraBeginUpdate() and RwCameraEndUpdate()");
  
         // create the frame raster.
         if (RwCameraBeginUpdate(pCamera))
         {
            RwRect rect = {0, 0, 0, 0};
   
            pFrameRaster = RwRasterCreate(0, 0, 0, rwRASTERTYPECAMERATEXTURE | rwRASTERDONTALLOCATE);
            RWS_ASSERT(pFrameRaster, "Unable to create raster");

            rect.w = RwRasterGetWidth (RwCameraGetRaster( pCamera ));
            rect.h = RwRasterGetHeight(RwCameraGetRaster( pCamera ));

            if ( !RwRasterSubRaster(pFrameRaster, RwCameraGetRaster( pCamera ), &rect) )
            {
               RWS_ASSERT(0, "Unable to create sub raster");
            }
       
            RwReal      RasterWidth, RasterHeight;
            RwReal    UAdjust, VAdjust;

            RasterWidth  = RwRasterGetWidth (pFrameRaster);
            RasterHeight = RwRasterGetHeight(pFrameRaster);

            // calculate the scale values need to adjust to the PS2 coordinate system.
            UAdjust = (1.0f / RasterWidth  ) * rect.w;
            VAdjust = (1.0f / RasterHeight ) * rect.h;

            RwCameraEndUpdate(pCamera);

            // create the camera with pRaster as it's back buffer
            RwCamera *pCam;

            pCam = RwCameraCreate();
            RWS_ASSERT(pCam, "Unable to create camera");

            RwFrame *pFrame = RwFrameCreate();
                   
            RwFrameSetIdentity( pFrame );
   
            RwCameraSetFrame ( pCam, pFrame );

            RwCameraSetRaster  ( pCam, const_cast<RwRaster*>(pRaster) );
            RwCameraSetZRaster ( pCam, m_RasterZ );

            RwCameraSetProjection ( pCam, rwPARALLEL );

            const RwReal ZNear = 0.01f;
            const RwReal ZFar  = 10000.0f;

            RwCameraSetNearClipPlane ( pCam, ZNear );
            RwCameraSetFarClipPlane  ( pCam, ZFar );

            RwV2d  ViewWindow;

            ViewWindow.x = static_cast<RwReal>(RwTan ( (rwPI * ((90.0f) / 180.0f)) * 0.5f));
            ViewWindow.y = ViewWindow.x / ( ENV_MAP_WIDTH / ENV_MAP_HEIGHT);
   
            RwCameraSetViewWindow ( pCam, &ViewWindow );

            // create the quad needed to render the selected back buffer area to pRaster
            const RwV2d Vx[]  =  {  {0.0f, 1.0f}, {1.0f, 0.0f} };
            RwV2d Uvs[]       =  {  {TopLeftUV.x * UAdjust, TopLeftUV.y * VAdjust },
                                    {BottomRightUV.x * UAdjust, BottomRightUV.y * VAdjust } };
            CFXReflectEnvironment::EnvMapRasterRender(pFrameRaster, Vx, Uvs, pCam);

            RwCameraDestroy( pCam );

            // destroy the frame raster
            RwRasterDestroy(pFrameRaster);

            RwFrameDestroy (pFrame);
         }

         RWS_RETURNVOID();
      }

   }

}
