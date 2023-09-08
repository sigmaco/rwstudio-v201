/*****************************************************************************
*
* File :     render.h
*
* Abstract : Rendering sub-system
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

#ifndef __GAMELOOP_RENDER_H__
#define __GAMELOOP_RENDER_H__

/**
*
*  \ingroup Render
*
*  \page RenderOverview Overview
*
*  This module is responsible for controlling the rendering of each frame.
*
*/

#include "../../core/eventhandler/ceventhandler.h"

namespace RWS
{
   /**
   *  \ingroup SystemEvents
   *  \page RenderEvents Rendering Events
   *  \li iMsgDoRender
   *  Sent each frame to begin rendering, if directors camera is disabled or no
   *  event handler is linked to iMsgDoRenderDirectorsCamera.
   */
   RWS_DEFINE_EVENT(iMsgDoRender, "RwCamera*", 
      "Sent each frame to begin rendering, if directors camera is disabled.");

   /**
   *  \ingroup SystemEvents
   *  \page RenderEvents Rendering Events
   *  \li iMsgDoRenderDirectorsCamera
   *  Sent each frame to begin rendering if directors camera is enabled.
   */
   RWS_DEFINE_EVENT(iMsgDoRenderDirectorsCamera, "RwCamera*",
      "Sent each frame to begin rendering if directors camera is enabled.");
   /**
   *  \ingroup SystemEvents
   *  \page RenderEvents
   *  \li iMsg_SetEnableDirectorsCamera
   *  Sent when the directors camera is enabled.
   */
   RWS_DEFINE_EVENT(iMsg_SetEnableDirectorsCamera, 0,
      "Sent when the directors camera is enabled.");
   /**
   *  \ingroup SystemEvents
   *  \page RenderEvents
   *  \li iMsg_SetDisableDirectorsCamera
   *  Sent when the directors camera is disabled.
   */
   RWS_DEFINE_EVENT(iMsg_SetDisableDirectorsCamera, 0,
      "Sent when the directors camera is disabled.");

   /**
   *  \ingroup SystemEvents
   *  \page RenderEvents Rendering Events
   *  \li iMsgPreShowRaster
   *  Sent each frame just before show raster is called.
   */
   RWS_DEFINE_EVENT(iMsgPreShowRaster, "RwCamera*", 
      "Sent each frame just before show raster is called.");
 
   /**
   *  \ingroup SystemEvents
   *  \page StreamEvents
   *  \li iMsgRequestCurrentCameraMatrix
   *  Sent when the workspace requests the current camera.
   */
   RWS_DEFINE_EVENT(iMsgRequestCurrentCameraMatrix,
      "RwUInt32 InstanceID", "Sent when the workspace requests the current camera.");

   namespace MainLoop
   {
      /**
      *
      *  \ingroup Render
      *
      *  Namespace used to encapsulate functionality for rendering.
      *
      */
      namespace Render
      {
         /**
         *
         *  \ingroup Render
         *
         *  This constant is used to define the freq that the iMsgDoRender event is generated,
         *  i.e. If we are running on an NTSC system TickRate is 60 Hz but on PAL it is
         *  50 Hz.
         *
         *  Usage RWS::MainLoop::Render::Rate
         *
         */
#if defined (VIDEO_MODE_PAL)
         const RwUInt32 Rate = 50; 
#elif defined (VIDEO_MODE_NTSC)
         const RwUInt32 Rate = 60; 
#else
         const RwUInt32 Rate = 60; 
#endif

         void Open(void); // Open Render SubModule

         void Close(void); // Close Render SubModule

         void Poll(void); // Poll Render SubModule

         void SetEnabled(RwBool bEnabled); // Enable/Disable rendering event 

         RwUInt32 GetFrameCount(void);

         void RegisterStreamChunkHandlers(void);

         void UnRegisterStreamChunkHandlers(void);

         /**
         *  Get the pointer to the global camera stored within MainLopp::Render.  
         *
         *  \returns Returns a pointer to the global camera.
         */
         RwCamera *GetCamera(void);

         /**
         *  Set the pointer to the global camera stored within MainLopp::Render.  
         *
         *  \param pCamera Pointer to a RwCamera object, Sets global camera to pCamera
         */
         void SetCamera(RwCamera * const pCamera);
      }
   }
}

#endif
   
