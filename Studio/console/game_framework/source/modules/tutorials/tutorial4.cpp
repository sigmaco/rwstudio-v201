/*****************************************************************************
*
* File :      Tutorial4.cpp
*
* Abstract :  Creating an event.
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
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "tutorial4.h"
#include "framework/toolkits/math/maths.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace Tutorial
   {
      RWS_IMPLEMENT_CLASSID(CTutorial4);
      RWS_REGISTERCLASS(CTutorial4);

      /**
      *
      *  Create CTutorial4 object. Gets behaviors atomic by extracting information the behavior needs from the 
      *  attributes used to construct the behavior.  This constructor also registers the event m_trigger with
      *  the event handler system.
      *
      *  This 'trigger' message is received by Tutorial 5 as a control message.
      *
      *  NOTE: The code registers the message to ensure it is setup (all senders & receivers should register any
      *  message they use, unless it's a system message). It does not, however, link to the message. This is
      *  because it transmits it. Only receivers of messages need to link to them.
      *
      *  \param attr standard attribute packets used to initialize.
      *  \see CTutorial5
      *
      */
      CTutorial4::CTutorial4(const CAttributePacket& attr)
         : InitCEventHandler(&m_pAtomic)
      {

         RWS_FUNCTION("RWS::Tutorial::CTutorial4::CTutorial4");

         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);

         RWS_ASSERT(m_pAtomic, "Failed to create atomic");

         RegisterMsg(m_trigger, "trigger", "RwUInt32");

         RWS_RETURNVOID();
      }
   
      /**
      *
      *  Destroy CTutorial4 object.   UnRegister m_trigger event.
      *
      */
      CTutorial4::~CTutorial4(void)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial4::~CTutorial4");

         UnRegisterMsg(m_trigger);

         RWS_RETURNVOID();
      }

      /**
      *
      *  Handle events - no events to process this time, so does nothing. However, it's a virtual function
      *  and so an implementation must exist.
      *
      *  \param pMsg
      *
      */
      void CTutorial4::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial4::HandleEvents");

         RWS_RETURNVOID();
      }

      /**
      *
      * Handle attributes.  This function retrieves the state of the
	   * check box from RenderWare Studio and sends the message m_trigger 
	   * to the event handler system. The state of the check box is transmitted
      * as data within the message. This is needed by tutorial 5.
      *
      * \param attr the standard attribute packets from RenderWare Studio.
      * \see CTutorial5
      *
      */
      void CTutorial4::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial4::HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);

         m_pAtomic.HandleSystemCommands(attr);

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CTutorial4));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_TriggerMessage:

               //  Receive message from RenderWare studio to send message m_trigger

               CMsg message(m_trigger, reinterpret_cast<void*>(attrIt->GetAs_RwUInt32()));
               SendMsg(message);
            break;
            }

            ++attrIt;
         }

         RWS_RETURNVOID();
      }

   }//namespace Tutorial
}//namespace RWS