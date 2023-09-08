/*****************************************************************************
*
* File :     FPSTimer.h
*
* Abstract : FPSTimer waits time + random time before triggering...
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

#ifndef __FPS_TIMER_H__
#define __FPS_TIMER_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace FPS
   {
      /**
      *
      *  \ingroup Mod_FPS
      *
      *  Timer, waits (time + random) time before triggering...
      *
      */
      class FPSTimer: public CSystemCommands, public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
      public:
         
         RWS_MAKENEWCLASS(FPSTimer);
         RWS_DECLARE_CLASSID(FPSTimer);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Timer Trigger", "Waits (Wait + Random) seconds before triggering and resetting the timer.");
         
         RWS_BEGIN_COMMANDS
            RWS_MESSAGE(   CMD_target     ,    "Target"     , "Send Event Triggered"                           , TRANSMIT,   "RwFrame*",  0)                
            RWS_MESSAGE(   CMD_killtarget ,    "Kill target", "Removes CEventHandlers linked to this event"                      , TRANSMIT,   0,  0)                
            RWS_ATTRIBUTE( CMD_wait       ,    "Wait"       , "Delay {Seconds} before triggering"              , SLIDER  ,   RwReal, RANGE(0,60,99))                
            RWS_ATTRIBUTE( CMD_random     ,    "Random"     , "Specify a random value to add to the wait time.", SLIDER  ,   RwReal, RANGE(0,0,99))                
            RWS_END_COMMANDS;
         
         virtual void HandleEvents(CMsg &pMsg);           
         virtual void HandleAttributes(const CAttributePacket& attr);
         
         FPSTimer(const CAttributePacket&);
         ~FPSTimer(void);
         
      protected:
         CEventId m_target;           /**< Send Event  */
         CEventId m_killtarget;       /**< Send to kill all CEventHandlers linked to m_killtarget */
         
         RwInt32 m_wait_counter;      /**< Counts up to m_wait_num_frames before triggering */
         RwInt32 m_wait_num_frames;   /**< Counter limit */
         RwInt32 m_random;            /**< Random Time */
         
         CAtomicPtr m_pAtomic;
      };
      
   }// namespace FPS
}// namespace RWS
#endif
