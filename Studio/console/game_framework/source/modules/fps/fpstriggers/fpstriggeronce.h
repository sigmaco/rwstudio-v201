/*****************************************************************************
*
* File :     FPSTriggerOnce.h
*
* Abstract : 
*
* Wait for the Event (m_targetname)
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

#ifndef __FPS_TRIGGER_ONCE_H__
#define __FPS_TRIGGER_ONCE_H__

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
      *  Abstract:Triggers once, then removes itself.  You must set the target to the name of another object in the level that has a matching
      *  "targetname".  
      */
      class FPSTriggerOnce: public CSystemCommands, public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
      public:
         
         RWS_MAKENEWCLASS(FPSTriggerOnce);
         RWS_DECLARE_CLASSID(FPSTriggerOnce);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Single Delay Trigger", "Delays before sending transmit event then deletes itself");
         
         RWS_BEGIN_COMMANDS
            RWS_MESSAGE( CMD_targetname, "Target name", "Receive Event"            , RECEIVE , 0, "ACTN_PLAYERTOUCH")
            RWS_MESSAGE( CMD_target    , "Target"    , "Send Event Triggered"     , TRANSMIT, 0, 0)
            RWS_MESSAGE( CMD_killtarget, "Kill target", "Removes CEventHandlers linked to this event", TRANSMIT, 0, 0)
            RWS_END_COMMANDS;
         
         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);
         
         FPSTriggerOnce(const CAttributePacket&);
         ~FPSTriggerOnce(void);
         void TriggerEvent( void );
         
      protected:
         CEventId m_targetname;   /** < Received to triggers this FPSTriggerOnce */
         CEventId m_target;       /** < Sent when m_targetname received */
         CEventId m_killtarget;   /** < Send to kill all CEventHandlers linked to m_killtarget */
         
         CAtomicPtr m_pAtomic;
      };
      
   }//namespace
}
#endif