/*****************************************************************************
*
* File :      Tutorial6.cpp
*
* Abstract :  Sending an event.
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
#include "tutorial6.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace Tutorial
   {
      RWS_IMPLEMENT_CLASSID(CTutorial6);
      RWS_REGISTERCLASS(CTutorial6);

      /**
      *
      *  Create CTutorial6 object. Gets behaviors atomic by extracting information the behavior needs from the 
	   *  attributes used to construct the behavior. Also creates matrix for rotating the object. It does not
      *  register or link to any messages at this point. This is because the names of these messages are not
      *  yet known.
      *
      *  \see HandleAttributes
      *  \param attr the standard attribute packets used to initialize.
      *
      */
      CTutorial6::CTutorial6(const CAttributePacket& attr)
         : InitCEventHandler(&m_pAtomic)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial6::CTutorial6");

         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);

         RWS_ASSERT(m_pAtomic, "Failed to create atomic");

         m_mat = RwMatrixCreate();

         RWS_POST(m_mat);

         RWS_RETURNVOID();
      }
   
      /**
      *
      *  Destroy CTutorial6 object. Unlink and unregister messages named and the running tick (system) messages.
      *  Note that the running tick message is not unregistered, as it is a system message.
      *
      */
      CTutorial6::~CTutorial6(void)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial6::~CTutorial6");

         RWS_PRE(m_mat);
         RwMatrixDestroy(m_mat);
         m_mat = 0;

         UnLinkMsg(m_incoming);
         UnLinkMsg(m_outgoing);
         UnLinkMsg(iMsgRunningTick);

         UnRegisterMsg(m_incoming);
         UnRegisterMsg(m_outgoing);

         RWS_RETURNVOID();
      }
  
      /**
      *
      *  Handle events.  When the 'incoming' message is received, the data is checked to find the status of the 
      *  sender's checkbox. If it is checked, then the running tick is linked to. If not, any link to the running 
      *  tick is removed. When linked the next system tick will cause the object to start rotating.
      *
      *  \param pMsg the standard message packet which may contain either a running tick or 'incoming' message.
      *
      */
      void CTutorial6::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial6::HandleEvents");

         if (pMsg.Id == iMsgRunningTick)
         {
            RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic.ptr());

            RwMatrixSetIdentity(m_mat);
            RwMatrixRotate(m_mat, &YAxis, 0.5f, rwCOMBINEPRECONCAT);
            RwFrameTransform(pFrame, m_mat, rwCOMBINEPRECONCAT);
         }
         else if (pMsg.Id == m_incoming)
         {
            // When the 'incoming' message is received the link to the iMsgRunningTick system message made
            // depending on the state of the incoming data. If there is no data then state is toggled. The
            // toggle is needed as when a message is 'test fired' the data parameter will be zero (NULL).
            RwUInt32 var;
            
            if (pMsg.pData)
            {
               var = *static_cast<RwUInt32*>(pMsg.pData);
            }
            else
            {
               var = !IsLinked(iMsgRunningTick);
            }

            // Change the state...
            if (var == 1)
            {
               if (IsLinked(iMsgRunningTick) == (RwBool)FALSE)
               {
                  LinkMsg(iMsgRunningTick, 0);
               }
            }
            else
            {
               if (IsLinked(iMsgRunningTick) == (RwBool)TRUE)
               {
                  UnLinkMsg(iMsgRunningTick);
               }
            }
         }

         RWS_RETURNVOID();
      }
  
      /**
      *
      * Handle attributes.  This function retrieves the state of the
      * check box from RenderWare Studio and sends the outgoing message 
      * to the event handler system. From there it will be received by any
      * behaviors which have linked to it. The function also receives 
      * attribute packets updating the names of the incoming and outgoing
      * messages.
      *
      * \param attr the standard attribute packets, some of which contain
      * message name changes or checkbox state changes.
      *
      */
      void CTutorial6::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial6::HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);

         // Handle system commands, e.g. if object moved from within RenderWare Studio, then
         // move them on console.
         m_pAtomic.HandleSystemCommands(attr);

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CTutorial6));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_targetname:

               // Receive message from RenderWare Studio to get the target name for the message.
               // the following steps can be shortened to...
               //
               // ReplaceLinkedMsg(m_incoming, attrIt->GetAs_char_ptr(), "RwUInt32*");
               //
               // ...but for clarity the individual steps have been shown.
               //
               UnLinkMsg(m_incoming);
               UnRegisterMsg(m_incoming);
               RegisterMsg(m_incoming, attrIt->GetAs_RwChar_ptr(), "RwUInt32*");
               LinkMsg(m_incoming, "RwUInt32*");
               break;

            case CMD_transname:

               // Transmission message from RenderWare Studio to get the target name for the message.
               // the following steps can be shortened to...
               //
               // ReplaceLinkedMsg(m_outgoing, attrIt->GetAs_char_ptr(), "RwUInt32*");
               //
               // ...but for clarity the individual steps have been shown.
               //
               UnLinkMsg(m_outgoing);
               UnRegisterMsg(m_outgoing);
               RegisterMsg(m_outgoing, attrIt->GetAs_RwChar_ptr(), "RwUInt32*");
               LinkMsg(m_outgoing, "RwUInt32*");
               break;

            case CMD_TriggerMessage:

               //  Receive message from RenderWare studio to send message "m_outgoing".
               RwUInt32 temp = attrIt->GetAs_RwUInt32();
               CMsg message(m_outgoing, static_cast<void*>(&temp));
               SendMsg(message);
               break;
            }

            ++attrIt;
         }

         RWS_RETURNVOID();
      }

   }//namespace Tutorial
}//namespace RWS
