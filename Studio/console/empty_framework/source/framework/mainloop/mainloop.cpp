/*****************************************************************************
*
* File :     MainLoop.cpp
*
* Abstract : Main Game Loop, this module handles initialization and shutdown
* of all of the major game framework components, this is also the location of 
* the highest level game loop used to control the order in which each of the
* main processes are performed
*
* Makes use of 
*
* logic.[h,cpp]    -> Game/Behavior logic
* network.[h,cpp]  -> Network communications
* render.[h,cpp]   -> Scene Rendering
* resource.[h,cpp] -> Resource loading
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
#include "mainloop.h"
#include "logic.h"

#ifdef RWS_DESIGN
#include "network.h"
#endif

#include "render.h"

#include "framework/toolkits/debugtools/debugtools.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/streamhandler/streamhandler.h"
#include "framework/core/resourcemanager/cresourcemanager.h"
#include "framework/core/memoryhandler/memoryprofile.h"
#include "framework/core/functionprofiler/profile.h"

#include "framework/startup/startup.h"

#include "framework/toolkits/time/gftime.h"
#include "framework/toolkits/world/smartptrs/ccameraptr.h"
#include "framework/toolkits/world/helpers/worldhelper.h"
#include "framework/toolkits/world/helpers/camerahelper.h"

#include "framework/core/attributehandler/clinearmemorymanager.h"

#ifdef RWS_EVENTVISUALIZATION
#include "framework/toolkits/eventvisualization/eventvisualization.h"
#endif

//////////////////////////////////////////////////////////////////
//
//
//
namespace RWS
{
   CEventId iMsgStartSystem;
   CEventId iMsgStopSystem;

   namespace MainLoop
   {
      namespace 
      {
#ifdef RWS_DEBUGTOOLS
         DebugTools::DisplayDebugTools *g_pDisplayDebugTools = 0;
#endif
         
#ifdef RWS_EVENTVISUALIZATION
         CEventVisualizer *g_pCEventVisualizer = 0;
#endif

         RwBool g_sysStartedFlag = FALSE;
      }

      
      namespace
      {
         void NewEntityCreatedCB()
         {
            RWS_FUNCTION("RWS::MainLoop::NULL::NewEntityCreatedCB");

            // If system is running, send the 'iMsgStartSystem' message again so that the newly created
            // object gets it (any other objects will get it again too, if still connected, but they
            // should disconnect after receiving it the first time - so shouldn't be a problem).
            if (g_sysStartedFlag)
            {
               SendMsg(iMsgStartSystem);
            }

            RWS_RETURNVOID();
         }

         /**
         *
         *  Send iMsgStartSystem event, enable logic, and render.
         *
         *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
         *
         *  \param pStream A pointer to the RenderWare Gfx Stream object.
         */
         void StartSystem (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
         {
            RWS_FUNCTION("RWS::MainLoop::NULL::StartSystem");
            
            SendMsg(iMsgStartSystem);

            g_sysStartedFlag = TRUE;
            
            MainLoop::Render::SetEnabled (TRUE);
            MainLoop::Logic::SetEnabled (TRUE);
            
            RWS_RETURNVOID();
         }
         
         /**
         *
         *  Send iMsgStopSystem event, disable logic, and render.
         *
         *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
         *
         *  \param pStream A pointer to the RenderWare Gfx Stream object.
         */
         void StopSystem (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
         {
            RWS_FUNCTION("RWS::MainLoop::NULL::StopSystem");
            
            SendMsg(iMsgStopSystem);
            g_sysStartedFlag = FALSE;
            
            MainLoop::Render::SetEnabled (FALSE);
            MainLoop::Logic::SetEnabled (FALSE);
            
            RWS_RETURNVOID();
         }

         /**
         *
         *  Reset, purges system of event handlers, attribute handlers, and assets.
         *
         *  \note This function will not remove global assets or entities.
         *
         *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
         *
         *  \param pStream A pointer to the RenderWare Gfx Stream object.
         */
         void Reset (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
         {
            RWS_FUNCTION("RWS::MainLoop::NULL::Reset");
      
            // Remove 'all' event handlers
            //
            CEventHandler::DeleteEventHandlers(iMsgDeleteEventHandler);
      
            // Remove 'all' attribute handler
            //
            CAttributeHandler::Purge();
      
            // Remove all assets from the system
            //
            CResourceManager::Purge();
      
            RWS_RETURNVOID();
         }

         /**
         *
         *  Initialize the game. This clears out any resources or attribute
         *  handlers that may still be around.
         *
         */
         void Init (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
         {
            RWS_FUNCTION("RWS::MainLoop::NULL::Init");

            // Remove all attribute handlers.
            //
            CAttributeHandler::PurgeAll();

            // Remove all event handlers
            //
            CEventHandler::DeleteEventHandlers(iMsgDeleteEventHandler);

            // Remove all resources.
            //
            CResourceManager::PurgeAll();

            RWS_RETURNVOID();
         }
      }
      
      /**
      *
      *  Register the chunk handlers implemented by MainLoop, \see strfunc_func \see CStreamHandler.
      *
      *  MainLoop implements the following chunk handlers, \ref strfunc_StartSystem,
      *  \ref strfunc_StopSystem, \ref strfunc_Reset.
      *
      */
      void RegisterStreamChunkHandlers(void)
      {
         RWS_FUNCTION("RWS::MainLoop::RegisterStreamChunkHandlers");

         CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
            strfunc_StartSystem), StartSystem);
         
         CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
            strfunc_StopSystem), StopSystem);

         CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
            strfunc_Reset), Reset);

         CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
            strfunc_Initialize), Init);

         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Unregister the chunk handlers implemented by MainLoop::Logic, \see strfunc_func \see CStreamHandler.
      */
      void UnRegisterStreamChunkHandlers(void)
      {
         RWS_FUNCTION("RWS::MainLoop::UnRegisterStreamChunkHandlers");

         CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_StartSystem));
         CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_StopSystem));
         CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_Reset));
         CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_Initialize));

         RWS_RETURNVOID();
      }

      /**
      *
      *  \ingroup MainLoop
      *
      *  \ref Open Opens the Main Loop module, intended as once only initialization
      *  of game framework, opens each of the game framework sub-system CEventHandler,
      *  CAttributeHandler, Stream, MainLoop::Logic, MainLoop::Render and
      *  MainLoop::Network.
      *
      *  \see RWS::CEventHandler::Open
      *  \see RWS::CAttributeHandler::Open
      *  \see RWS::Stream::Open
      *  \see RWS::MainLoop::Logic::Open
      *  \see RWS::MainLoop::Network::Open
      *  \see RWS::MainLoop::Render::Open
      */
      void Open(RwInt32 ScreenWidth, RwInt32 ScreenHeight, RwInt32 ZBitDepth)
      {
         RWS_FUNCTION("RWS::MainLoop::Open");
         
         RWS::MemoryProfile::Open(Time::GetTime, TRUE);
         
         // Create a default world/camera and set as current
         //
         MainLoop::Render::SetCamera( CameraHelper::CreateMainRasterCamera( ScreenWidth, ScreenHeight, ZBitDepth) );
         
         // Open the event handler module
         //
         CEventHandler::Open ();
         
         CEventHandler::RegisterStreamChunkHandlers();
         
         CLinearMemoryManager::RegisterStreamChunkHandlers();

         // Open the attribute handler module
         //
         CAttributeHandler::Open (NewEntityCreatedCB);
         
         CAttributeHandler::RegisterStreamChunkHandlers();

         CStreamHandler::RegisterStreamChunkHandlers();
         
         CResourceManager::Open();

         CResourceManager::RegisterStreamChunkHandlers();
         
         // Initialize logic sub-module and set to running
         //
         Logic::Open();
         
         Logic::RegisterStreamChunkHandlers();
         
         Logic::Running();

        // Open Rendering sub-module
         //
         Render::Open();

         Render::RegisterStreamChunkHandlers();         

#ifdef RWS_DEBUGTOOLS
         // If we are using DebugTools we need an eventhandler namely DisplayDebugTools in
         // order to render the debug info at the correct time, i.e. when the event 
         // iMsgPostRpWorldRender occurs.
         //
         g_pDisplayDebugTools = ::RWS_NEW DebugTools::DisplayDebugTools;
         DebugSwitches::SetSwitch(DebugSwitches::m_enable_printf, true);
#endif
         
#ifdef RWS_EVENTVISUALIZATION
         g_pCEventVisualizer = ::RWS_NEW CEventVisualizer;
#endif

         Render::Poll();
         Render::Poll();

#ifdef RWS_DESIGN
         // Open Network Communications sub-module
         //
         Network::Open();

         Network::RegisterStreamChunkHandlers();
#endif
         
         RegisterStreamChunkHandlers();
         
         CEventHandler::RegisterMsg(iMsgStartSystem, iMsgStartSystemStr, 0);
         CEventHandler::RegisterMsg(iMsgStopSystem, iMsgStopSystemStr, 0);

         RWS_RETURNVOID();
      }
      
      /**
      *
      *  \ingroup MainLoop
      *
      *  \ref Close Closes the Main Loop module, intended as once only shutdown of game framework,
      *  closes each of the game framework sub-system CEventHandler, CAttributeHandler, 
      *  Stream, MainLoop::Logic, MainLoop::Render and MainLoop::Network.
      *
      *  \see RWS::CEventHandler::Close
      *  \see RWS::CAttributeHandler::Close
      *  \see RWS::Stream::Close
      *  \see Logic::Close
      *  \see Network::Close
      *  \see Render::Close
      *
      */
      void Close(void)
      {
         RWS_FUNCTION("RWS::MainLoop::Close");

         CEventHandler::UnRegisterMsg(iMsgStartSystem);
         CEventHandler::UnRegisterMsg(iMsgStopSystem);

#ifdef RWS_EVENTVISUALIZATION
         delete g_pCEventVisualizer;
#endif
         
#ifdef RWS_DEBUGTOOLS
         delete g_pDisplayDebugTools;
#endif
         // Delete all entities, linked to iMsgDeleteEventHandler
         //
         CEventHandler::DeleteEventHandlers (iMsgDeleteEventHandler); 
         
         // Some entities are AttributeHandlers Only... need to remove these using the following
         // call as SendMsgDeleteEventHandlerId will not remove them 
         //
         CAttributeHandler::PurgeAll();
         
         CStreamHandler::UnRegisterStreamChunkHandlers();
         
#ifdef RWS_DESIGN
         Network::UnRegisterStreamChunkHandlers();

         // Close Network Communications sub-module
         //
         Network::Close();
#endif
         
         // Close Logic sub-module
         //
         Logic::UnRegisterStreamChunkHandlers();

         Logic::Close();
                  
         // Close Rendering sub-module
         //
         Render::UnRegisterStreamChunkHandlers();

         Render::Close();
         
         CAttributeHandler::UnRegisterStreamChunkHandlers();
         
         // Close the attribute system
         //
         CAttributeHandler::Close ();
         
         // Remove an resources left in the resource manager.
         //
         CResourceManager::PurgeAll(); // Purge any remaining resources
         
         CResourceManager::UnRegisterStreamChunkHandlers();

         CResourceManager::Close();

         CEventHandler::UnRegisterStreamChunkHandlers();
         
         CLinearMemoryManager::DestroyLinearMemoryManager();
         
         // Close the event system
         //
         CEventHandler::Close ();
         
         // Destroy the default camera
         //
         CameraHelper::Destroy(MainLoop::Render::GetCamera());

         MainLoop::Render::SetCamera(0);

         UnRegisterStreamChunkHandlers();

         RWS::MemoryProfile::Close();
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *  \ingroup MainLoop
      *
      *  Polls the Main Loop, polls each of the game framework
      *  sub-systems Network::Poll, Logic::Poll, Render::Poll.
      *
      *  \see Network::Poll()
      *  \see Logic::Poll()
      *  \see Render::Poll()
      *
      */
      void Poll(void)
      {
         RWS_FUNCTION("RWS::MainLoop::Poll");
         
#ifdef RWS_DESIGN
         // Poll network communications
         //
         Network::Poll();
#endif
         // Update game logic
         //
         Logic::Poll();
         
         // Render scene
         //
         Render::Poll();
         
         // Send Metrics data back to workspace
         //
         RWS_SEND_TRACE_METRICS(60);

         RWS::MemoryProfile::Poll();

         RWS::FunctionProfile::CProfileManager::SendProfileDataToWorkspace();
         RWS_RETURNVOID();
      }
   }// namespace MainLoop
}// namespace RWS
