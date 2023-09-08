/*****************************************************************************
*
* File :     CameraHelper.cpp
*
* Abstract : Helper functions for use with RwCamera
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

//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "camerahelper.h"
#include "framehelper.h"
#include "framework/core/macros/debugmacros.h"
#include "../clevel.h"
#include "framework/mainloop/render.h"

namespace RWS
{
   namespace CameraHelper
   {
      namespace
      {
         const RwReal DefaultCameraViewAngle = rwPI * ((60.0f) / 180.0f);  // In Radians
         const RwReal DefaultFarClipPlane = 5000.0f;
         const RwReal DefaultNearClipPlane = 1.0f;
         const RwCameraProjection DefaultCameraProjection = rwPERSPECTIVE;
      }

      /**
      *
      *  \ingroup CameraHelper
      *
      *  This is used to safely destroy an RwCamera, usually called by either the
      *  destructor of CCameraPtr or by the function overload RwBool RwCameraDestroy( RWS::CCameraPtr &rCCameraPtr ).
      *
      * \li Detaches any child frames from the frame of the RwCamera.
      * \li Deletes the frame.
      * \li Removes the RwCamera from the world.
      * \li Deletes the camera.
      *
      *  \param pCamera Pointer to an RwCamera object.
      *
      *  \returns Returns TRUE if successful. 
      */
      RwBool Destroy(RwCamera *pCamera)
      {
         RWS_FUNCTION("RWS::CCameraPtr::Destroy")

         RWS_PRE(pCamera);

         RwFrame* pFrame = RwCameraGetFrame(pCamera);
         if (pFrame)
         {
            // Clean up any frame hierarchy attached to the atomic (the assumption is
            // that any child frames are managed by other objects, so it is sufficient
            // for us to simply detach them and let them 'float off')
            //
            RwFrameForAllChildren(pFrame, FrameHelper::RemoveChildFrame, 0);

            // If this frame is itself attached to another, detach it
            //
            if (RwFrameGetParent(pFrame)) RwFrameRemoveChild(pFrame);

            RwCameraSetFrame(pCamera, 0);
            FrameHelper::FrameDestroy(pFrame);
         }
   
         // Remove camera from world
         RpWorld* pWorld = RwCameraGetWorld (pCamera);
         if (pWorld) RpWorldRemoveCamera(pWorld, pCamera);

         // Release Color-Buffer if required
         //
         if (RwCameraGetRaster(pCamera))
         {
            RwRaster* pRaster = RwCameraGetRaster(pCamera);

            if (pRaster)
            {
               RWS_TRACE("rwRASTERTYPECAMERA Destroy [CBuf]: "<<pRaster);

               RwRasterDestroy(pRaster);

               RwCameraSetRaster(pCamera, 0);
            }
         }

         // Release Z-Buffer if required
         //
         if (RwCameraGetZRaster(pCamera))
         {
               RwRaster *pRaster = RwCameraGetZRaster(pCamera);

               if (pRaster)
               {
                  RWS_TRACE("rwRASTERTYPECAMERA Destroy [ZBuf]: "<<pRaster);

                  RwRasterDestroy(pRaster);

                  RwCameraSetZRaster(pCamera, 0);
               }
         }

         // Finally, RenderWare destroy camera
         RwBool ret = RwCameraDestroy(pCamera);

         RWS_RETURN(ret);
      }

      /**
      *
      *  \ingroup CameraHelper
      *
      *  Create a camera with its own raster, based on the specified width, height and bit depth.
      *
      *  \param width Required screen display width.
      *
      *  \param height Required screen display height.
      *
      *  \param depth Required screen bit depth.
      *
      */
      RwCamera *CreateMainRasterCamera(RwUInt32 Width, RwUInt32 Height, RwUInt32 ZDepth)
      {
         RWS_FUNCTION("RWS::CameraHelper::RwCameraCreateDefault");

         RWS_TRACE("Width "<<Width);
         RWS_TRACE("Height "<<Height);
         RWS_TRACE("ZDepth "<<ZDepth);

         RwCamera *_camera = RwCameraCreate();
   
         RWS_POST(_camera);

         RwRaster *_raster = RwRasterCreate(Width, Height, 0, rwRASTERTYPECAMERA);

         RWS_TRACE("rwRASTERTYPECAMERA Create [CBuf]: "<<_raster);

         RwCameraSetRaster (_camera, _raster);

         RWS_POST(RwCameraGetRaster(_camera));

         RwRaster *_rasterZ = RwRasterCreate(Width, Height, ZDepth, rwRASTERTYPEZBUFFER);

         RWS_TRACE("rwRASTERTYPECAMERA Create [ZBuf]: "<<_rasterZ);

         RwCameraSetZRaster (_camera, _rasterZ);
   
         RWS_POST(RwCameraGetZRaster(_camera));

         RwFrame *_frame = RwFrameCreate();

         RWS_POST(_frame);

         //-----------------------------------------------------
         //
         RwFrameSetIdentity(_frame);
   
         RwCameraSetFrame (_camera, _frame);
   
         RwCameraSetProjection (_camera, DefaultCameraProjection);

         //-----------------------------------------------------
         //
         RwCameraSetNearClipPlane (_camera, DefaultNearClipPlane);
         RwCameraSetFarClipPlane (_camera, DefaultFarClipPlane);

         //-----------------------------------------------------
         //
         RwReal _aspect;
      
         if (Height != 0) 
         {
            _aspect = (RwReal)Width / (RwReal)Height;
   
            RwV2d  _viewwindow;

            _viewwindow.x = static_cast<RwReal>(RwTan (DefaultCameraViewAngle * 0.5f));
            _viewwindow.y = _viewwindow.x / _aspect;
   
            RwCameraSetViewWindow (_camera, &_viewwindow);
         }

         RWS_RETURN(_camera);
      }

      /**
      *
      * \ingroup CameraHelper
      *
      * Create a camera, this camera will use a sub-raster of the main camera.
      *
      * \param rRect Reference to a RwRect object, this is used to define the size of the sub-raster
      * in pixels.
      *
      */
      RwCamera *CreateSubRasterCamera(RwRect &rRect)
      {
         RWS_FUNCTION("RWS::CameraHelper::CreateSubRasterCamera");

         RwUInt32 width = rRect.w;
         RwUInt32 height = rRect.h;

         RwCamera *main_camera = MainLoop::Render::GetCamera();

         RWS_PRE(main_camera);

         RwCamera *_camera = CreateMainRasterCamera(0, 0, 0);

         RWS_POST(_camera);

         RwRasterSubRaster(RwCameraGetRaster(_camera), RwCameraGetRaster(main_camera), &rRect);
         RwRasterSubRaster(RwCameraGetZRaster(_camera), RwCameraGetZRaster(main_camera), &rRect);

         //-----------------------------------------------------
         //
         RwReal _aspect = (RwReal)width / (RwReal)height;
   
         RwV2d  _viewwindow;

         _viewwindow.x = static_cast<RwReal>(RwTan (DefaultCameraViewAngle * 0.5f));
         _viewwindow.y = _viewwindow.x / _aspect;
   
         RwCameraSetViewWindow (_camera, &_viewwindow);

         RWS_RETURN(_camera);
      }

     /**
      *
      * \ingroup CameraHelper
      *
      * Set the field of view of the specified camera.
      *
      *  \param rViewWindow The tan() of half view angle, calculate using RwTan(RWDEG2RAD(view_angle * 0.5f));
      *  \param pCamera A pointer to RwCamera object.
      *
      */
      void SetFieldOfView(RwReal rViewWindow, RwCamera *pCamera)
      {
         RWS_FUNCTION("RWS::CameraHelper::SetFieldOfView");

         RwVideoMode videoMode;
         RwEngineGetVideoModeInfo(&videoMode, RwEngineGetCurrentVideoMode());
      
         RwRect rect;
         RwV2d vw;
      
         rect.x = 0;
         rect.y = 0;
         rect.w = RwRasterGetWidth(RwCameraGetRaster(pCamera));
         rect.h = RwRasterGetHeight(RwCameraGetRaster(pCamera));
      
         if (videoMode.flags & rwVIDEOMODEFFINTERLACE) rect.h *= 2;
      
         vw.x = rViewWindow;
         vw.y = (rect.h * rViewWindow) / rect.w;
      
         RwCameraSetViewWindow(pCamera, &vw);

         RWS_RETURNVOID();
      }

      /**
      *
      * Handles system commands, CMD_LoadMatrix for an RwCamera
      *
      * \param pCamera A pointer to a RenderWare Gfx RwCamera object.
      *
      * \param rAttr A reference to a CAttributePacket.
      *
      */
      void HandleSystemCommands(RwCamera &pCamera, const CAttributePacket& rAttr)
      {
         RWS_FUNCTION("RWS::CameraHelper::HandleSystemCommands");

         CAttributeCommandIterator attrIt(rAttr, RWS_CLASSID_OF(CSystemCommands));
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               case CSystemCommands::CMD_LoadMatrix:
               {
                  RwFrame* pFrame = RwCameraGetFrame(&pCamera);
                  if (pFrame) CSystemCommands::UpdateFrame(*pFrame, *attrIt);
               }
               break;
            }
            ++attrIt;
         }

         RWS_RETURNVOID();
      }
   }
}