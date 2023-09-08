/*****************************************************************************
*
* File :     MainLoop.h
*
* Abstract : Main game loop.
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

#ifndef __MainLoop_H__
#define __MainLoop_H__

#include "framework/core/eventhandler/ceventhandler.h"

/**
*
*  \ingroup MainLoop
*
*  \page MainLoopOverview Overview
*
*  This module is responsible for defining the order of initialization and shutdown of the
*  game framework sub-systems, and polling each of the sub-systems which forms the processing
*  of the main loop.
*
*/

namespace RWS
{
  /**
   *  \ingroup SystemEvents
   *  \page StreamEvents Stream Events
   *  \li iMsgStopSystem
   *  Sent when RWS::strfunc_StopSystem is received, usually at the start of a 'large' stream.
   */
   RWS_DEFINE_EVENT(iMsgStopSystem,
      0, "Sent when a \'stop system\' message is received.");

   /**
   *  \ingroup SystemEvents
   *  \page StreamEvents
   *  \li iMsgStartSystem
   *  Sent when RWS::strfunc_StartSystem is received, usually at the end of a 'large' stream.
   */
   RWS_DEFINE_EVENT(iMsgStartSystem,
      0, "Sent when a \'start system\' message is received.");

   /**
   *
   * Main Game Loop, this namespace handles initialization and shutdown
   * of all of the major game framework components, this is also the location of 
   * the highest level game loop used to control the order in which each of the
   * game processes are performed.
   *
   */
   namespace MainLoop
   {
      // Open MainLoop and sub-systems
      //
      void Open(RwInt32 ScreenWidth, RwInt32 ScreenHeight, RwInt32 ZBitDepth);

      // Close MainLoop and sub-systems
      //
      void Close(void);

      // Poll MainLoop
      //
      void Poll(void);

      void RegisterStreamChunkHandlers(void);

      void UnRegisterStreamChunkHandlers(void);
   }
}

#endif