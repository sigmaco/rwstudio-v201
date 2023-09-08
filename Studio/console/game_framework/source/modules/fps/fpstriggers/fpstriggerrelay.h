/*****************************************************************************
*
* File :     FPSTriggerRelay.h
*
* Abstract : A trigger that waits for a specified event ( m_targetname ),
* the behavior then waits for a specified length of time ( m_delay == m_delay_count ) 
* before triggering the event ( m_target ) and deleting any behaviors linked to 
* the event ( m_killtarget ) before resetting.
*
* Notes :
*
*  target The trigger that will be triggered 
*
*  targetname The variable used to trigger the trigger_relay. 
*
*  killtarget Removes brush or monster with target name. Now you see it, now you don't. 
*  sounds Set the sound upon triggering to one of the following: 
*
*
* delay Delay before the target trigger is triggered. 
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
* Copyright (c) 2002 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#ifndef __FPS_TRIGGER_RELAY_H__
#define __FPS_TRIGGER_RELAY_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/mainloop/logic.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace FPS
   {
      /**
      *
      *  \ingroup Mod_FPS
      *
      * Abstract : A trigger that waits for a specified event ( m_targetname ),
      * the behavior then waits for a specified length of time ( m_delay == m_delay_count ) 
      * before triggering the event ( m_target ) and deleting any behaviors linked to 
      * the event ( m_killtarget ) before resetting.
      *
      * Notes :
      *
      *  target The trigger that will be triggered 
      *
      *  targetname The variable used to trigger the trigger_relay. 
      *
      *  killtarget Removes brush or monster with target name. Now you see it, now you don't. 
      *  sounds Set the sound upon triggering to one of the following: 
      *
      *
      * delay Delay before the target trigger is triggered. 
      */
      class FPSTriggerRelay: public CSystemCommands, public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
      public:            
         RWS_MAKENEWCLASS(FPSTriggerRelay);
         RWS_DECLARE_CLASSID(FPSTriggerRelay);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Trigger Relay", "Passes on an event after {delay} seconds");
         
         RWS_BEGIN_COMMANDS
            RWS_MESSAGE  ( CMD_targetname, "Targetname", "Receive Event", RECEIVE , 0, 0)
            RWS_MESSAGE  ( CMD_target    , "Target"    , "Send Event Triggered" ,TRANSMIT, "RwFrame*", 0)       
            RWS_MESSAGE  ( CMD_killtarget, "Kill target", "Removes CEventHandlers linked to this event", TRANSMIT, 0     , 0)       
            RWS_ATTRIBUTE( CMD_delay     , "Delay"     , "Delay {Seconds} before sending event (target, killtarget)", SLIDER, RwReal, RANGE(0, 0, 60))       
         RWS_END_COMMANDS;
         
         virtual void HandleEvents(CMsg &pMsg);
         
         virtual void HandleAttributes(const CAttributePacket& attr);
         
         FPSTriggerRelay(const CAttributePacket&);
         ~FPSTriggerRelay(void);
         void TriggerEvent( void);
         void TriggerDelayed( void );
         
      protected:
         
         // Define entities states
         enum State
         {
            State_Waiting,          /*< Waiting to be triggered */
            State_Delayed           /*< Delay before triggering */
         };
         
         RwUInt32 m_state;        /*< Current state */        
         CEventId m_targetname;   /*< Received to triggers this FPS_trigger_relay */        
         CEventId m_target;       /*< Sent when m_targetname received */
         CEventId m_killtarget;   /*< Used to kill all CEventHandlers linked to m_killtarget */      
         RwUInt32 m_delay;        /*< Delay in ticks */
         RwUInt32 m_delay_count;  /*< Count of number of ticks */
         CAtomicPtr m_pAtomic;    /*< Pointer to 3D representation of this entity*/
      };
   }
}
#endif
