/*****************************************************************************
*
* File :     FPSTriggerMultiple.h
*
* Abstract : 
*
* (1) Wait for the event (m_targetname)
*
* waits for (delay) seconds
*
* sends (m_target)
* deletes (m_killtarget) anything linked to this event
*
* waits for (wait) seconds
*
* goto (1)
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
* Copyright (c) 2002 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#ifndef __FPS_TRIGGER_MULTIPLE_H__
#define __FPS_TRIGGER_MULTIPLE_H__

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
      *. 
      *  \ingroup Mod_FPS
      *
      * Abstract: Waits for an event then, delays n seconds,  then sends a target event and the 
      * kill event so that anything linked to the kill event is destroyed. Then after another n second 
      * wait, goes back to the beginning and waits for another event.
      *
      */
      class FPSTriggerMultiple: public CEventHandler, public CAttributeHandler, public CSystemCommands, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(FPSTriggerMultiple);
         RWS_DECLARE_CLASSID(FPSTriggerMultiple);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Multiple Delay Trigger", "Delays before sending transmit event");
         
         RWS_BEGIN_COMMANDS
            RWS_MESSAGE  ( CMD_targetname, "Target name", "Receive Event"            , RECEIVE , 0, "ACTN_PLAYERTOUCH")
            RWS_MESSAGE  ( CMD_target    , "Target"    , "Send Event"               , TRANSMIT, 0, 0)
            RWS_MESSAGE  ( CMD_killtarget, "Kill target", "Removes CEventHandlers linked to this event", TRANSMIT, 0, 0)
            RWS_ATTRIBUTE( CMD_delay     , "Delay"     , "Delay in seconds before sending event (target, killtarget)",   SLIDER, RwReal, RANGE(0, 0, 60))
            RWS_ATTRIBUTE( CMD_wait      , "Wait"      , "Delay in seconds before resetting", SLIDER, RwReal, RANGE(-1, 0, 99))
            RWS_END_COMMANDS;
         
         FPSTriggerMultiple(const CAttributePacket&);
         ~FPSTriggerMultiple(void);
         
         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);
         void WaitThenSend( void );
         void Delayed( void );
         void WaitToReset( void );
         
      protected:
         // Define entity states
         enum State
         {
            State_Waiting,
               State_Delayed,
               State_Waiting_To_Reset
         };
         
         RwUInt32 m_state;
         
         CEventId m_target;      /**< Sent when m_targetname received */
         CEventId m_targetname;  /**< Received to triggers this FPSTriggerOnce */
         CEventId m_killtarget;  /**< Send to kill all CEventHandlers linked to m_killtarget */
         
         RwUInt32 m_delay;       /**< Required delay */
         RwUInt32 m_delay_count; /**< Temp, delay counter, starts at 0 when reaches m_delay trigger is fired */
         RwUInt32 m_wait;        /**< Required wait, re-uses m_delay_count */
         
         CAtomicPtr m_pAtomic;
      };
   }
}
#endif