/*****************************************************************************
*
* File :     render.cpp
*
* Notes : 
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

//#define RWS_TRACEENABLE
//#define RWS_CALLSTACKENABLE

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "render.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/debugtools/debugtools.h"
#include "framework/core/streamhandler/streamhandler.h"
#include "framework/core/streamhandler/strfunc.h"
#include "framework/toolkits/world/helpers/camerahelper.h"
#include "framework/core/netstream/nettools.h"

#ifdef _WINDOWS
#include "../startup/win32/win.h"
#endif

#include "connectionscreen.h"

namespace RWS
{
   CEventId iMsgDoRender;
   CEventId iMsgDoRenderDirectorsCamera;

   CEventId iMsg_SetEnableDirectorsCamera;
   CEventId iMsg_SetDisableDirectorsCamera;
   CEventId iMsgPreShowRaster;

   CEventId iMsgRequestCurrentCameraMatrix;

   namespace MainLoop
   {
      namespace Render
      {
         namespace 
         {
            RwBool gbEnabled = TRUE;  // Enable/Disable Rendering event

            RwCamera *g_pCamera;
         } // namespace

         /**
         *  Get the pointer to the global camera stored within MainLoop::Render.  
         *
         *  \returns Returns a pointer to the global camera.
         */
         RwCamera *GetCamera(void) {return g_pCamera;}

         /**
         *  Set the pointer to the global camera stored within MainLoop::Render.  
         *
         *  \param pCamera Pointer to a RwCamera object, Sets global camera to pCamera
         */
         void SetCamera(RwCamera * const pCamera) {g_pCamera = pCamera;}
 
         namespace
         {
            /**
            *
            *  Enable the directors camera.
            *
            *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
            *
            *  \param pStream A pointer to the RenderWare Gfx Stream object.
            */
#ifdef RWS_DESIGN
            void EnableDirectorsCamera (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
            {
               RWS_FUNCTION("RWS::NULL::SetEnableDirectorsCamera");
      
               DebugSwitches::SetSwitch(DebugSwitches::m_enable_DirectorsCamera, true);
               SendMsg(iMsg_SetEnableDirectorsCamera);
      
               RWS_RETURNVOID();
            }
#endif   
            /**
            *
            *  Disable the directors camera.
            *
            *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
            *
            *  \param pStream A pointer to the RenderWare Gfx Stream object.
            */
#ifdef RWS_DESIGN
            void DisableDirectorsCamera (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
            {
               RWS_FUNCTION("RWS::NULL::DisableDirectorsCamera");
      
               DebugSwitches::SetSwitch(DebugSwitches::m_enable_DirectorsCamera, false);
               SendMsg(iMsg_SetDisableDirectorsCamera);
      
               RWS_RETURNVOID();
            }
#endif

           /**
            *
            *  Receive new camera transformation matrix from RenderWare Studio Workspace.
            *
            *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
            *
            *  \param pStream A pointer to the RenderWare Gfx Stream object.
            */
#ifdef RWS_DESIGN
            void SetDirectorsCameraMatrix (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
            {
               RWS_FUNCTION("RWS::NULL::SetDirectorsCameraMatrix");
      
               typedef struct taggfCameraMsg {
                  RwMatrix m; /**< Transformation Matrix - Camera Position/Orientation */
                  RwReal fov; /**< Field of view - radians */
               } gfCameraMsg, *pgfCameraMsg;
      
               RWS_ASSERT(MainLoop::Render::GetCamera(), "NewCameraMatrix MainLoop::Render::GetCamera()==0."
                     " Either no cameras in scene or not yet created.");
      
               RwChar *buffer = static_cast<char*>(::RWS_OP_NEW(rChunkHeader.length) );
      
               RwStreamRead(pStream, buffer, rChunkHeader.length);
      
               pgfCameraMsg pCmsg = reinterpret_cast<pgfCameraMsg>(buffer);;
      
               RwFrame *cameraFrame = RwCameraGetFrame (MainLoop::Render::GetCamera());
      
               // Set transformation matrix
               //
               RwMatrix *nmatrix = RwMatrixCreate();

#if defined(DOLPHIN) && defined(__MWERKS__)
               std::memcpy(nmatrix, &pCmsg->m, sizeof(RwMatrix));
#else
               memcpy(nmatrix, &pCmsg->m, sizeof(RwMatrix));
#endif
      
               nmatrix->flags = 0;

               RwMatrixOrthoNormalize (nmatrix, nmatrix);

               RwFrameTransform(cameraFrame, nmatrix, rwCOMBINEREPLACE);

               RwFrameUpdateObjects(cameraFrame);
      
               RwMatrixDestroy(nmatrix);
      
               // Setup aspect ratio
               //
               RwReal ViewWindow = (RwReal)RwTan(pCmsg->fov * 0.5f);

               CameraHelper::SetFieldOfView(ViewWindow, MainLoop::Render::GetCamera());
      
               operator delete (buffer);
      
               RWS_RETURNVOID();
            }
#endif
            /**
            *
            *  Request to send current camera frame to workspace.
            *
            *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
            *
            *  \param pStream A pointer to the RenderWare Gfx Stream object.
            */
#ifdef RWS_DESIGN
            void GetCameraMatrix (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream) 
            {
               RWS_FUNCTION("RWS::NULL::GetCameraMatrix");
      
                  RwFrame *frame;
         
                  CMsg msg (iMsgRequestCurrentCameraMatrix, reinterpret_cast<void*> (0) );
         
                  if (SendMsg(msg))
                  {
                     // Event handlers that respond to iMsgRequestCurrentCameraMatrix call 
                     // SendTransformToWorkspace with the frame of the camera.
                     //
                  }
                  else
                  {
                     frame = RwCameraGetFrame (MainLoop::Render::GetCamera());
            
                  NetTools::SendTransform(frame);
               }
      
               RWS_RETURNVOID();
            }
#endif

         } // namespace

         /**
         *
         *  Register the chunk handlers implemented by MainLoop::Render, \see strfunc_func \see CStreamHandler.
         *
         *  MainLoop::Render implements the following chunk handlers, \ref strfunc_SetDirectorsCameraMatrix,
         *  \ref strfunc_EnableDirectorsCamera, strfunc_DisableDirectorsCamera, strfunc_GetEntityMatrix.
         *
         */
         void RegisterStreamChunkHandlers(void)
         {
            RWS_FUNCTION("RWS::MainLoop::Render::RegisterStreamChunkHandlers");
         
#ifdef RWS_DESIGN
            CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
               strfunc_SetDirectorsCameraMatrix), SetDirectorsCameraMatrix);

            CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
               strfunc_EnableDirectorsCamera), EnableDirectorsCamera);

            CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
               strfunc_DisableDirectorsCamera), DisableDirectorsCamera);

            CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
               strfunc_GetEntityMatrix), GetCameraMatrix);
#endif
            RWS_RETURNVOID();
         }


         /**
         *
         *  Unregister the chunk handlers implemented by MainLoop::Render, \see strfunc_func \see CStreamHandler.
         */
         void UnRegisterStreamChunkHandlers(void)
         {
            RWS_FUNCTION("RWS::MainLoop::Render::UnRegisterStreamChunkHandlers");
            
#ifdef RWS_DESIGN
            CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
               strfunc_SetDirectorsCameraMatrix));

            CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
               strfunc_EnableDirectorsCamera));

            CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
               strfunc_DisableDirectorsCamera));

            CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
               strfunc_GetEntityMatrix));
#endif
            RWS_RETURNVOID();
         }


         /**
         *
         *  \ingroup Render
         *
         *  Initialize the render events, registers rendering related events.
         *
         */
         void Open(void)
         {
            RWS_FUNCTION("RWS::MainLoop::Render::Open");

            CEventHandler::RegisterMsg(iMsgDoRender, iMsgDoRenderStr, "RwCamera*");

            CEventHandler::RegisterMsg(iMsgDoRenderDirectorsCamera, iMsgDoRenderDirectorsCameraStr, "RwCamera*");
            CEventHandler::RegisterMsg(iMsgPreShowRaster, iMsgPreShowRasterStr, "RwCamera*");


#ifdef RWS_DESIGN
            CEventHandler::RegisterMsg(iMsg_SetEnableDirectorsCamera,
               iMsg_SetEnableDirectorsCameraStr, 0);

            CEventHandler::RegisterMsg(iMsg_SetDisableDirectorsCamera,
               iMsg_SetDisableDirectorsCameraStr, 0);

            CEventHandler::RegisterMsg(iMsgRequestCurrentCameraMatrix,
               iMsgRequestCurrentCameraMatrixStr, "RwUInt32 InstanceID");

            ::RWS_NEW ConnectionScreen::CRenderConnectionScreen;
#endif
            RWS_RETURNVOID();
         }

         /**
         *
         *  \ingroup Render
         *
         *  UnInitialize the render events, unregister rendering related events.
         *
         */
         void Close(void)
         {
            RWS_FUNCTION("RWS::MainLoop::Render::Close");

            CEventHandler::UnRegisterMsg(iMsgDoRender);
            CEventHandler::UnRegisterMsg(iMsgDoRenderDirectorsCamera);
            CEventHandler::UnRegisterMsg(iMsgPreShowRaster);

#ifdef RWS_DESIGN
            CEventHandler::UnRegisterMsg(iMsg_SetEnableDirectorsCamera);
            CEventHandler::UnRegisterMsg(iMsg_SetDisableDirectorsCamera);
            CEventHandler::UnRegisterMsg(iMsgRequestCurrentCameraMatrix);
#endif
            RWS_RETURNVOID();
         }
         
         /**
         *  \ingroup Render
         *
         *  Set rendering event flag state.
         *
         *  \param bEnabled Flag if true rendering event is enabled, if false rendering event
         *  is disabled.
         *  
         */
         void SetEnabled(RwBool bEnabled)
         {
            RWS_FUNCTION("RWS::MainLoop::Render::SetEnableRenderingEvent");

            gbEnabled = bEnabled;

            RWS_RETURNVOID();
         }

         namespace 
         {
            RwUInt32 sm_frame_count = 0;
         }

         /**
         *
         *  \ingroup Render
         *
         *  \return Returns the number of frames that have been displayed.
         *
         */
         RwUInt32 GetFrameCount(void) 
         {
            RWS_FUNCTION("RWS::MainLoop::Render::GetFrameCount");

            RWS_RETURN(sm_frame_count);
         }

         /**
         *
         *  \ingroup Render
         *
         *  Triggers, iMsgDoRender or iMsgDoRenderDirectorsCamera event informing 
         *  the system to render the current scene
         *
         */
         void Poll(void)
         {
            RWS_FUNCTION("RWS::MainLoop::Render::Poll");

            if (!gbEnabled) RWS_RETURNVOID();

            sm_frame_count++;

#if defined(RWS_DEBUGTOOLS) && defined(RWS_DESIGN)
            if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_DirectorsCamera) == true) 
            {
               // Directors camera enabled
               //
               CMsg Msg (iMsgDoRenderDirectorsCamera, MainLoop::Render::GetCamera());
               
               if (!SendMsg(Msg))
               {
                  // If nothings listening to iMsgDoRenderDirectorsCamera send
                  // iMsgDoRender anyway.
                  //
                  CMsg Msg (iMsgDoRender, MainLoop::Render::GetCamera());
                  SendMsg(Msg);
               }
            }
            else
            {
               // Directors camera disabled
               //
               CMsg Msg (iMsgDoRender, MainLoop::Render::GetCamera());
               SendMsg(Msg);
            }
#else
            CMsg Msg(iMsgDoRender, MainLoop::Render::GetCamera());
            SendMsg(Msg);
#endif

            CMsg PreFlipMessage(iMsgPreShowRaster, MainLoop::Render::GetCamera());
            SendMsg(PreFlipMessage);



            if (MainLoop::Render::GetCamera())
            {
#if defined(_WINDOWS)
               RwCameraShowRaster(MainLoop::Render::GetCamera(), Win::GetMainWindow(), rwRASTERFLIPDONTWAIT);
#elif defined(SKY)
               RwCameraShowRaster(MainLoop::Render::GetCamera(), 0, rwRASTERFLIPWAITVSYNC);
#elif defined(_XBOX)
               RwCameraShowRaster(MainLoop::Render::GetCamera(), 0, rwRASTERFLIPWAITVSYNC);
#elif defined(DOLPHIN)
               RwCameraShowRaster(MainLoop::Render::GetCamera(), 0, rwRASTERFLIPWAITVSYNC);
#elif
#error         Your platform RwCameraShowRaster goes here...
#endif            
            }

            RWS_RETURNVOID();
         }

      } // namespace Render

   } // namespace MainLoop

} // namespace RWS 
