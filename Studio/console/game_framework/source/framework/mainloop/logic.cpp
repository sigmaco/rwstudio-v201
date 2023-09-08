/*****************************************************************************
*
* File :     logic.cpp
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
//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "logic.h"
#include "../../core/macros/debugmacros.h"
#include "../streamhandler/streamhandler.h"
#include "../streamhandler/strfunc.h"
#include "framework/toolkits/time/gftime.h"
#include "framework/toolkits/debugtools/debugtools.h"

namespace RWS
{
   CEventId iMsgSetRunningMode;
   CEventId iMsgSetPausedMode;
   CEventId iMsgSetFrozenMode;

   CEventId iMsgRunningPreTick;
   CEventId iMsgRunningTick;
   CEventId iMsgRunningPostTick;

   CEventId iMsgPausedPreTick;
   CEventId iMsgPausedTick;
   CEventId iMsgPausedPostTick;

   namespace MainLoop
   {
      namespace Logic
      {
         

         namespace
         {
            //////////////////////////////////////////////////////////////////
            //
            // Current frame number (Logic Tick) & frames rendered
            //
            int frame_number = 0;

            //////////////////////////////////////////////////////////////////
            // 
            // Registered message Id's
            //
            CMsg msg_MsgPreTick;
            CMsg msg_MsgTick;
            CMsg msg_MsgPostTick;

            RwBool gbEnabled = TRUE;
         }

         /**
         *
         *  \ingroup Logic
         *
         *  \ref Running, Forces game logic state to running, sets pre-tick event to iMsgRunningPreTick,
         *  tick event to iMsgRunningTick and post-tick to iMsgRunningPostTick.
         *  Also broadcasts an iMsgSetRunningMode event.
         */

         void Running(void)
         {
            RWS_FUNCTION("SetMainLoopRunning");
         
            SendMsg(iMsgSetRunningMode);  // Event running mode selected
         
            msg_MsgPreTick.Id = iMsgRunningPreTick;
            msg_MsgTick.Id = iMsgRunningTick;
            msg_MsgPostTick.Id = iMsgRunningPostTick;
         
            RWS_RETURNVOID();
         }
      
         /**
         *
         *  \ingroup Logic
         *
         *  \ref Paused, Forces game logic state to paused, sets pre-tick event to iMsgPausedPreTick,
         *  tick event to iMsgPausedTick and post-tick to iMsgPausedPostTick.
         *  Also broadcasts an iMsgSetPausedMode event.
         *
         *  \see RWS::MainLoop::Logic::Poll
         */

         void Paused(void)
         {
            RWS_FUNCTION("SetMainLoopPaused");
         
            SendMsg(iMsgSetPausedMode);   // Event pause mode selected
         
            msg_MsgPreTick.Id = iMsgPausedPreTick;
            msg_MsgTick.Id = iMsgPausedTick;
            msg_MsgPostTick.Id = iMsgPausedPostTick;
         
            RWS_RETURNVOID();
         }

        /**
         *
         *  \ingroup Logic
         *
         *  \ref Frozen, Forces game logic state to frozen, set pre-tick, tick and post-tick events to none.
         *  Also broadcasts an iMsgSetFrozenMode event.
         *
         *  \see RWS::MainLoop::Logic::Poll
         */

         void Frozen(void)
         {
            RWS_FUNCTION("SetMainLoopPaused");
         
            SendMsg(iMsgSetFrozenMode);   // Event frozen mode selected
         
            msg_MsgPreTick.Id = ERROR_NOT_A_VALID_MESSAGE_ID;
            msg_MsgTick.Id = ERROR_NOT_A_VALID_MESSAGE_ID;
            msg_MsgPostTick.Id = ERROR_NOT_A_VALID_MESSAGE_ID;
         
            RWS_RETURNVOID();
         }
 
         /**
         *  \ingroup Logic
         *
         *  Enable/Disable logic tick event.
         *
         *  \param benabled Flag if true tick event is enabled, if false tick event
         *  is disabled.
         *  
         */
         void SetEnabled(RwBool benabled)
         {
            gbEnabled = benabled;
         }
         
        /**
         *
         *  \ingroup Logic
         *
         *  \ref Poll, Triggers three events which are typically used to update the game logic of any
         *  behaviors. These events are pre-tick, tick and post-tick. The actually events depend on the
         *  state of the Logic module \see Frozen, Paused, Running.
         *  Also calls the EventSystem to delete any CEventHandlers registered for deletion. 
         *
         */
         
         void Poll(void)
         {
            if (!gbEnabled) return;
            
#if defined(RWS_DEBUGTOOLS) && defined(RWS_DESIGN)
            RwUInt32 start_time = Time::GetTime();
#endif
            // Send Pre-Tick Event
            //
            SendMsg(msg_MsgPreTick);

#if defined(RWS_DEBUGTOOLS) && defined(RWS_DESIGN)
            if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_metrics_printf))
            {
               DebugTools::_Printf("RWS::Logic::Poll msg_MsgPreTick %d\n", Time::GetTime() - start_time);
            }

            start_time = Time::GetTime();
#endif
            // Send current frame number as part of tick event, useful for distributing processing over
            // several frames.
            //
            msg_MsgTick.pData = reinterpret_cast<void*>(frame_number);

            frame_number++;

            // Send Tick Event
            //
            SendMsg(msg_MsgTick);

#if defined(RWS_DEBUGTOOLS) && defined(RWS_DESIGN)
            if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_metrics_printf))
            {
               DebugTools::_Printf("RWS::Logic::Poll msg_MsgTick %d\n", Time::GetTime() - start_time);
            }

            start_time = Time::GetTime();
#endif
            
            // Send Post Tick Event
            //
            SendMsg(msg_MsgPostTick);

#if defined(RWS_DEBUGTOOLS) && defined(RWS_DESIGN)
            if (DebugSwitches::GetSwitch(DebugSwitches::m_enable_metrics_printf))
            {
               DebugTools::_Printf("RWS::Logic::Poll msg_MsgPostTick %d\n", Time::GetTime() - start_time);
            }
#endif
            
            // Do Safe cleanup of deleted entities
            //
            CEventHandler::DeleteEventHandlers(iMsgDeleteEntity);
         }

        /**
         *
         *  \ingroup Logic
         *
         *  \ref Open, Opens the logic module, registers logic events.
         *
         *  \see RWS::iMsgSetRunningMode
         *  \see RWS::iMsgSetPausedMode
         *  \see RWS::iMsgRunningPreTick
         *  \see RWS::iMsgRunningTick
         *  \see RWS::iMsgRunningPostTick
         *  \see RWS::iMsgPausedPreTick
         *  \see RWS::iMsgPausedTick
         *  \see RWS::iMsgPausedPostTick
         *  \see RWS::iMsg_SetEnableDirectorsCamera
         *  \see RWS::iMsg_SetDisableDirectorsCamera
         *
         */

         void Open(void)
         {
            CEventHandler::RegisterMsg (iMsgSetRunningMode, iMsgSetRunningModeStr, 0);
            CEventHandler::RegisterMsg (iMsgSetPausedMode, iMsgSetPausedModeStr, 0);
            CEventHandler::RegisterMsg (iMsgSetFrozenMode, iMsgSetFrozenModeStr, 0);
         
            CEventHandler::RegisterMsg (iMsgRunningPreTick, iMsgRunningPreTickStr, 0);
            CEventHandler::RegisterMsg (iMsgRunningTick, iMsgRunningTickStr, "RwUInt32");
            CEventHandler::RegisterMsg (iMsgRunningPostTick, iMsgRunningPostTickStr, 0);
         
            CEventHandler::RegisterMsg (iMsgPausedPreTick, iMsgPausedPreTickStr, 0);
            CEventHandler::RegisterMsg (iMsgPausedTick, iMsgPausedTickStr, "RwUInt32");
            CEventHandler::RegisterMsg (iMsgPausedPostTick, iMsgPausedPostTickStr, 0);
         }

        /**
         *
         *  \ingroup Logic
         *
         *  Close the logic module, unregister logic events.
         *
         *  \see RWS::iMsgSetRunningMode
         *  \see RWS::iMsgSetPausedMode
         *  \see RWS::iMsgRunningPreTick
         *  \see RWS::iMsgRunningTick
         *  \see RWS::iMsgRunningPostTick
         *  \see RWS::iMsgPausedPreTick
         *  \see RWS::iMsgPausedTick
         *  \see RWS::iMsgPausedPostTick
         *  \see RWS::iMsg_SetEnableDirectorsCamera
         *  \see RWS::iMsg_SetDisableDirectorsCamera
         *
         */
         void Close(void)
         {
            CEventHandler::UnRegisterMsg ( iMsgSetRunningMode );
            CEventHandler::UnRegisterMsg ( iMsgSetPausedMode );
            CEventHandler::UnRegisterMsg ( iMsgSetFrozenMode );

            CEventHandler::UnRegisterMsg ( iMsgRunningPreTick );
            CEventHandler::UnRegisterMsg ( iMsgRunningTick );
            CEventHandler::UnRegisterMsg ( iMsgRunningPostTick );

            CEventHandler::UnRegisterMsg ( iMsgPausedPreTick );
            CEventHandler::UnRegisterMsg ( iMsgPausedTick );
            CEventHandler::UnRegisterMsg ( iMsgPausedPostTick );
         }

         namespace
         {
            /**
            *
            *  Enter frozen mode.
            *
            */
            void SetFrozenMode (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
            {
               RWS_FUNCTION("RWS::NULL::SetFrozenMode");
      
               MainLoop::Logic::Frozen();
      
               RWS_RETURNVOID();
            }
   
            /**
            *
            *  Enter running mode.
            *
            */
            void SetRunningMode (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
            {
               RWS_FUNCTION("RWS::NULL::SetRunningMode");
      
               // FE, extend this to return to the previous state from frozen.
               //
               MainLoop::Logic::Running();
      
               RWS_RETURNVOID();
            }
         }

         /**
         *
         *  Register the chunk handlers implemented by MainLoop::Logic, \see strfunc_func \see CStreamHandler.
         *
         *  MainLoop::Logic implements the following chunk handlers, \ref strfunc_SetFrozenMode,
         *  \ref strfunc_SetRunningMode.
         *
         */
         void RegisterStreamChunkHandlers(void)
         {
            CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
               strfunc_SetFrozenMode), SetFrozenMode);

            CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
               strfunc_SetRunningMode), SetRunningMode);
         }

         /**
         *
         *  Unregister the chunk handlers implemented by MainLoop::Logic, \see strfunc_func \see CStreamHandler.
         */
         void UnRegisterStreamChunkHandlers(void)
         {
            CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_SetFrozenMode));
            CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_SetRunningMode));
         }
      }
   }
}
