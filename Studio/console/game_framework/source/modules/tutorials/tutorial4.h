/*****************************************************************************
*
* File :       Tutorial4.h
*
* Abstract :   Creating an event.
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

#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"

#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/mainloop/logic.h"

#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace Tutorial
   {
      /**
      *
      *  \ingroup Mod_Tutorials
      *
      *  Simple send message behavior with interface to RenderWare studio.
      *
      *  \see CAttributeHandler
      *  \see CEventHandler
      *  \see CSystemCommands
      *  \see CAtomicPtr
      *
      */
      class CTutorial4 : public CSystemCommands, public CAttributeHandler , public CEventHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(CTutorial4);
         RWS_DECLARE_CLASSID(CTutorial4);
         RWS_CATEGORY("Tutorial");
         RWS_DESCRIPTION("Tutorial4", "Tutorial 4");

         CTutorial4(const CAttributePacket& attr);
         ~CTutorial4(void);

         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);

         // The attribute definition for this behavior allows you to turn on and off the sending message.
         //
         RWS_BEGIN_COMMANDS
            RWS_ATTRIBUTE(CMD_TriggerMessage, "Send Event", "", BOOLEAN, RwUInt32, DEFAULT(0))
         RWS_END_COMMANDS;

      protected:
         CAtomicPtr m_pAtomic;   /**< Behavior's Atomic. */
         CEventId m_trigger;     /**< m_trigger is used to store the clients reference to an event. */ 
      };
   }//namespace Tutorial
}//namespace RWS