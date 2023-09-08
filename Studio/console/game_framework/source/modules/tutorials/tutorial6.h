/*****************************************************************************
*
* File :       Tutorial6.h
*
* Abstract :   Sending an event.
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
      *  Implementation of a class that can both send and Receive messages. The messages can be named by the user.
      *
      *  \see CAttributeHandler
      *  \see CEventHandler
      *  \see CSystemCommands
      *  \see CAtomicPtr
      *
      */
      class CTutorial6 : public CSystemCommands, public CAttributeHandler , public CEventHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(CTutorial6);
         RWS_DECLARE_CLASSID(CTutorial6);
         RWS_CATEGORY("Tutorial");
         RWS_DESCRIPTION("Tutorial6", "Tutorial 6");
         
         CTutorial6(const CAttributePacket& attr);
         ~CTutorial6(void);
         
         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);
         
         /*
          *  The attribute definition for this behavior allows you to turn on and off the sending message with
          *  a boolean check box.  Also  CMD_targetname lets you specify a receive event unique message string.
          */
         RWS_BEGIN_COMMANDS
            RWS_MESSAGE(CMD_targetname, "Receive Event", "", RECEIVE, "RwUInt32*", 0)
            RWS_MESSAGE(CMD_transname, "Transmit Event", "", TRANSMIT, "RwUInt32*", 0)
            RWS_ATTRIBUTE(CMD_TriggerMessage, "Send Event", "", BOOLEAN, RwUInt32, DEFAULT(0))
            RWS_END_COMMANDS;
      protected:
         
         CAtomicPtr m_pAtomic;      /**< Behavior's Atomic. */
         CEventId m_incoming,       /**< Used to store the clients reference to the receiving event. */ 
               m_outgoing;          /**< Used to store the clients reference to the transmitting event. */ 
         RwMatrix *m_mat;           /**< Matrix used for rotating object. */
      };

   }//namespace Tutorial
}//namespace RWS

