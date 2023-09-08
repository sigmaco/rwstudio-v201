/*****************************************************************************
*
* File :      Tutorial5.cpp
*
* Abstract :  Receiving an event.
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
#include "tutorial5.h"
#include "framework/toolkits/math/maths.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace Tutorial
   {
      RWS_IMPLEMENT_CLASSID(CTutorial5);
      RWS_REGISTERCLASS(CTutorial5);

      /**
      *
      *  Create CTutorial5 object. Gets behaviors atomic by extracting information the behavior needs from the 
      *  attributes used to construct the behavior.  This constructor also registers the event m_trigger with the 
      *  event handler system. Linking to an event informs the system that this behavior's event handler
      *  should be called when SendMsg is called (by another behavior) with the specified event.
      *
      *  The event this tutorial receives is sent from Tutorial 4. This (tutorial 5) behavior both registers
      *  and (because it receives the message) links to the 'trigger' message.  Both Senders and receivers should
      *  register messages, not assume they have been registered somewhere else.
      *
      *  \see SendMsg
      *  \see CTutorial4
      *  \param attr standard attribute packets needed to initialize the object.
      *
      */
      CTutorial5::CTutorial5(const CAttributePacket& attr)
         : InitCEventHandler(&m_pAtomic)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial5::CTutorial5");

         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);

         RWS_ASSERT(m_pAtomic, "Failed to create atomic");

         m_mat = RwMatrixCreate();
         RWS_POST(m_mat);

         RegisterMsg( m_trigger, "trigger", "RwUInt32");
         LinkMsg(m_trigger, "RwUInt32");

         RWS_RETURNVOID();
      }

      /**
      *
      *  Destroy CTutorial5 object. Unlinks / Unregisters m_trigger event and running tick.
      *
      */
      CTutorial5::~CTutorial5(void)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial5::~CTutorial5")

         RWS_PRE(m_mat);
         RwMatrixDestroy(m_mat);
         m_mat = 0;

         UnLinkMsg(iMsgRunningTick);
         UnLinkMsg(m_trigger);
         UnRegisterMsg(m_trigger);

         RWS_RETURNVOID();
      }

      /**
      *
      *  Handle events.  When the m_trigger message is received, the associated data from the tick box
      *  (from CTutorial4, passed as data in the message) is checked. If the check box is checked, 
      *  then the running tick is linked. In that way, on the next running tick message, the box will
      *  rotate.
      *
      *  \see CTutorial4
      *  \param pMsg the standard message packets, which may contain running tick or trigger messages.
      *
      */
      void CTutorial5::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial5::HandleEvents");

         RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic.ptr());

         if (pMsg.Id == iMsgRunningTick)
         {
            RwMatrixSetIdentity(m_mat);
            RwMatrixRotate(m_mat, &YAxis, 0.5f, rwCOMBINEPRECONCAT);
            RwFrameTransform(pFrame, m_mat, rwCOMBINEPRECONCAT);
         }
         else if (pMsg.Id == m_trigger)
         {
            RwUInt32 checked = reinterpret_cast<RwUInt32>(pMsg.pData);

            if (checked == 1)
            {
               if (IsLinked(iMsgRunningTick) == (RwBool)FALSE)
               {
                  LinkMsg(iMsgRunningTick, 0);
               }

               RWS_TRACE("Message received, rotating object");
            }
            else
            {
               if (IsLinked(iMsgRunningTick) == (RwBool)TRUE)
               {
                  UnLinkMsg(iMsgRunningTick);
               }

               RWS_TRACE("Message received, stop rotating object");
            }
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  Handle attribute update. This behavior has no attributes, so this function does
      *  nothing except pass the incoming attribute changes to the base class. This updates
      *  object position, etc.
      *
      *  \param attr standard attribute packets.
      *
      */
      void CTutorial5::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial5::HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);

         m_pAtomic.HandleSystemCommands(attr);

         RWS_RETURNVOID();
      }

   }//namespace Tutorial
}//namespace RWS