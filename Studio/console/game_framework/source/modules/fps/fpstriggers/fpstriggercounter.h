/*****************************************************************************
*
* File :     FPSTriggerCounter.h
*
* Abstract : 
*
* Wait for (m_count) Events (m_targetname)
*
* Wait for (m_delay) ticks 
*
* Fires Event (m_target)
*
* Deletes Anything Linked to (m_killtarget)
*
* Deletes Itself
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

#ifndef __FPS_TRIGGER_COUNTER_H__
#define __FPS_TRIGGER_COUNTER_H__

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
      *  \ingroup Mod_FPS
      *
      * Acts as an intermediary for an action that takes multiple inputs.
      *
      * If nomessage is not set, it will print "1 more. " etc when triggered and "sequence complete" when finished.
      * After the counter has been triggered "count" times (default 2), it will fire all of it's targets and
      * remove itself.
      *
      */
      class FPSTriggerCounter : public CSystemCommands, public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(FPSTriggerCounter);
         RWS_DECLARE_CLASSID(FPSTriggerCounter);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Counter Trigger", "Counts (count) input events before sending trigger event");
         
         
         RWS_BEGIN_COMMANDS            
            RWS_MESSAGE( CMD_targetname,
               "Target name",
               "Receive Event",
               RECEIVE , 0     , 0           )

            RWS_MESSAGE( CMD_target    ,
               "Target",
               "Transmit Event, if num received>count send Transmit",
               TRANSMIT, 0     , 0           )

            RWS_MESSAGE( CMD_killtarget,
               "Kill target",
               "Removes CEventHandlers linked to this event",
               TRANSMIT, 0     , 0           )

            RWS_ATTRIBUTE( CMD_delay   ,
               "Delay",
               "Delay before action is triggered (seconds)",
               SLIDER  , RwReal, RANGE(0,0,2))

            RWS_ATTRIBUTE( CMD_count,
               "Count",
               "This parameter decided how many times the trigger_counter should"
               "be triggered before it triggers it target",
               SLIDER, RwUInt32, RANGE(1,2,3))

            RWS_END_COMMANDS;
         
         FPSTriggerCounter(const CAttributePacket&);
         ~FPSTriggerCounter(void);
         
         virtual void HandleEvents(CMsg &pMsg);  
         virtual void HandleAttributes(const CAttributePacket& attr);
         void TriggerEvent( void );
         void WaitThenSend( void );
         
      protected:
         // Define entity states
         enum State
         {
            State_Waiting,
               State_Delayed
         };
         
         RwUInt32 m_state;
         
         CEventId m_target;      /**< Sent when m_targetname received */
         CEventId m_targetname;  /**< Received to triggers this FPSTriggerOnce */
         CEventId m_killtarget;  /**< Send to kill all CEventHandlers linked to m_killtarget */
         
         RwUInt32 m_count;       /**< Number of rx messages required to trigger */
         RwUInt32 m_rx_count;    /**< Number of rx messages currently received */        
         RwUInt32 m_delay;       /**< Required delay */
         RwUInt32 m_delay_count; /**< Temp, delay counter, starts at 0 when reaches m_delay trigger is fired */        
         CAtomicPtr m_pAtomic;
      };
   }
}
#endif