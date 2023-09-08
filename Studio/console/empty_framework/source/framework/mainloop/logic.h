/*****************************************************************************
*
* File :     logic.h
*
* Abstract : Game logic sub-system
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

#ifndef __GAMELOOP_LOGIC_H__
#define __GAMELOOP_LOGIC_H__

/**
*
*  \ingroup Logic
*
*  \page LogicOverview Overview
*
*  This module is responsible for updating the game logic each logical frame.
*
*/

#include "../../core/eventhandler/ceventhandler.h"

namespace RWS
{
   /**
   *  \ingroup SystemEvents
   *  \page LogicEvents Logic Events
   *  \li iMsgSetRunningMode
   *  Sent when gameloop is set into running mode, game is running.
   */
   RWS_DEFINE_EVENT(iMsgSetRunningMode, 0, "Sent when logic sub-system is set into running mode.");
   /**
   *  \ingroup SystemEvents
   *  \page LogicEvents
   *  \li iMsgSetPausedMode
   *  Sent when gameloop is set into paused mode, game is paused.
   */
   RWS_DEFINE_EVENT(iMsgSetPausedMode, 0, "Sent when logic sub-system is set into paused mode.");
   /**
   *  \ingroup SystemEvents
   *  \page LogicEvents
   *  \li iMsgSetFrozenMode
   *  Sent when gameloop is set into frozen mode, game logic is frozen.
   */
   RWS_DEFINE_EVENT(iMsgSetFrozenMode, 0, "Sent when logic sub-system is set into frozen mode."); 
   /**
   *  \ingroup SystemEvents
   *  \page LogicEvents
   *  \li iMsgRunningPreTick
   *  Sent just before each MsgRunningTick.
   */
   RWS_DEFINE_EVENT(iMsgRunningPreTick, 0, "Sent just before each MsgRunningTick.");
   /**
   *  \ingroup SystemEvents
   *  \page LogicEvents
   *  \li iMsgRunningTick
   *  Sent each logic loop while the game is unpaused, sends current frame number as parameter.
   */
   RWS_DEFINE_EVENT(iMsgRunningTick, "RwUInt32", "Sent each logic loop while the game is unpaused,"
      " sends current frame number as parameter.");
   /**
   *  \ingroup SystemEvents
   *  \page LogicEvents
   *  \li iMsgRunningPostTick
   *  Sent just after each MsgRunningTick.
   */
   RWS_DEFINE_EVENT(iMsgRunningPostTick, 0, "Sent just after each MsgRunningTick.");
   /**
   *  \ingroup SystemEvents
   *  \page LogicEvents
   *  \li iMsgPausedPreTick
   *  Sent just before each MsgPausedTick.
   */
   RWS_DEFINE_EVENT(iMsgPausedPreTick, 0, "Sent just before each MsgPausedTick.");
   /**
   *  \ingroup SystemEvents
   *  \page LogicEvents
   *  \li iMsgPausedTick
   *  Sent each logic loop while the game is paused, sends current frame number as parameter.
   */
   RWS_DEFINE_EVENT(iMsgPausedTick, "RwUInt32", "Sent each logic loop while the game"
      " is paused, sends current frame number as parameter.");
   /**
   *  \ingroup SystemEvents
   *  \page LogicEvents
   *  \li iMsgPausedPostTick
   *  Sent just after each MsgPausedTick.
   */
   RWS_DEFINE_EVENT(iMsgPausedPostTick, 0, "Sent just after each MsgPausedTick.");

   namespace MainLoop
   {
    /**
      *
      *  \ingroup Logic
      *
      *  Namespace used to encapsulate functionality for the update of game logic.
      *
      */
      namespace Logic
      {
          /**
          *
          *  \ingroup Logic
          *
          *  This constant is used to define the freq that the iMsgRunningTick event is generated,
          *  i.e. If we are running on an NTSC system TickRate is 60 Hz but on PAL it is
          *  50 Hz.
          *
          *  Usage RWS::MainLoop::Logic::Rate
          *
          */
#if defined (VIDEO_MODE_PAL)
         const RwUInt32 Rate = 50; 
#elif defined (VIDEO_MODE_NTSC)
         const RwUInt32 Rate = 60; 
#else
         const RwUInt32 Rate = 60; 
#endif
         void Running(void); // Set Logic sub-system into Running mode.

         void Paused(void);  // Set Logic sub-system into Paused mode.

         void Frozen(void);  // Set Logic sub-system into Frozen mode.

         void Poll(void);    // Poll Logic sub-system.

         void Open(void);    // Open Logic sub-system.

         void Close(void);   // Close Logic sub-system.
         
         void SetEnabled(RwBool benabled); // Enable/Disable rendering event 

         void RegisterStreamChunkHandlers(void);

         void UnRegisterStreamChunkHandlers(void);
      }
   }
}

#endif
