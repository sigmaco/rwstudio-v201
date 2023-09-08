/****************************************************************************
*
* File :      CConductor.cpp
*
* Abstract :  Sequence controller behaviour.
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

#include "precomp.h"

// Left to its own devices, the MW linker will dead strip the registrar ctors, so...

#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off     // Turn off exceptions due to bug in MW Dwarf
#endif                     // Generator for PS2, present in CW 3.04

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//

#include "framework/core/resourcemanager/cresourcemanager.h"
#include "framework/toolkits/sequencer/seqctrldataresource.h"
#include "framework/toolkits/sequencer/cseqctrldata.h"
#include "framework/mainloop/logic.h"
#include "cconductor.h"

namespace RWS
{
   namespace SEQUENCE
   {
      /**
      *
      * CConductor class functions.
      *
      */

      RWS_IMPLEMENT_CLASSID(CConductor);
      RWS_REGISTERCLASS(CConductor);

      /**
      *
      * Constructor - just initialize.
      *
      */

      CConductor::CConductor(const CAttributePacket &attr) :
         InitCEventHandler(0),
         m_seqProcessor(RWS_MEMORY_TYPE_RWS, 0, SEQ_FLAG_BUFFER_WARNINGS)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CConductor::CConductor");

         // Link to running/frozen mode change messages.

#ifdef RWS_DESIGN

         LinkMsg(iMsgSetRunningMode);
         LinkMsg(iMsgSetFrozenMode);
#endif

         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - free up objects.
      *
      */

      CConductor::~CConductor(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CConductor::~CConductor");

#ifdef RWS_DESIGN

         UnLinkMsg(iMsgSetRunningMode);
         UnLinkMsg(iMsgSetFrozenMode);
#endif
         UnLinkMsg(m_startMsg);
         UnLinkMsg(iMsgRunningTick);
         UnLinkMsg(m_waitMsg);

         UnRegisterMsg(m_startMsg);
         UnRegisterMsg(m_waitMsg);

         RWS_RETURNVOID();
      }

      /**
      *
      * Standard RWS event handler.
      *
      */

      void CConductor::HandleEvents(CMsg &msg)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CConductor::HandleEvents");

         // Start the sequence running...

         if (msg.Id == m_startMsg && !m_seqProcessor.IsRunning())
         {
            // Begin the sequence...

            LinkMsg(iMsgRunningTick, 0);
            m_seqProcessor.Start();
         }

         // Update the sequence (if running)...

         if (msg.Id == iMsgRunningTick && m_seqProcessor.IsRunning())
         {
            CSeqProcessor::StepState state;
            const char *pEventName;

            // Step sequence...

            m_seqProcessor.StepPrepare();

            do
            {
               state = m_seqProcessor.Step(pEventName);

               if (state == CSeqProcessor::SS_SEND_EVENT)
               {
                  CEventId eventID;

                  // Register and link to message. Assume format is unspecified.

                  RegisterMsg(eventID, pEventName, 0);
                  LinkMsg(eventID);

                  // Send it - NOTE this will block, anything called should take MINIMUM time to
                  // complete - otherwise is may effect the playback of the sequence. Also, the
                  // data pointer will ALWAYS be ZERO.

                  SendMsg(eventID);

                  // Unlink and unregister from this message...

                  UnLinkMsg(eventID);
                  UnRegisterMsg(eventID);
               }
               else if (state == CSeqProcessor::SS_WAIT_ON_EVENT)
               {
                  // Wait for event. Link to event to wait for (unlinks at same time)...

                  ReplaceLinkedMsg(m_waitMsg, pEventName, 0);
               }
            }
            while (state < CSeqProcessor::SS_KEEP_CALLING);

            // Sequence ended ?

            if (state == CSeqProcessor::SS_ENDED)
            {
               m_seqProcessor.Stop();
               m_seqProcessor.FreeBuffers();
               UnLinkMsg(iMsgRunningTick);
            }
         }

         // Waited for event arrived ?

         if (msg.Id == m_waitMsg)
         {
            m_seqProcessor.ClearWaitEvent();
         }

#ifdef RWS_DESIGN

         // Workspace going into running mode...

         if (msg.Id == iMsgSetRunningMode)
         {
            m_seqProcessor.Freeze(FALSE);
         }

         // Workspace going into frozen mode...

         if (msg.Id == iMsgSetFrozenMode)
         {
            m_seqProcessor.Freeze();
         }
#endif         // RWS_DESIGN

         RWS_RETURNVOID();
      }

      /**
      *
      * Intercepts CSystemCommands commands, searching for 'attach resource'
      * messages.
      *
      * \param attr Standard attribute data packet.
      *
      */

      void CConductor::HandleSystemCommands(const CAttributePacket &attr)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CConductor::HandleSystemCommands");

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CSystemCommands));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CSystemCommands::CMD_AttachResource:
               {
                  const RWSGUID * pResourceId;
                  attrIt->GetCommandData(&pResourceId);
                  AddResource(pResourceId);
               }
               break;
            }
            ++attrIt;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * A sequence instruction block resource has been added, code to request it
      * from the resource handler.
      *
      * \param pResourceID  Pointer to determine which resource to add.
      *
      */

      void CConductor::AddResource(const RWSGUID *pResourceID)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CConductor::AddResource");

         // Look up the object in the global store.

         const RwChar *ResourceType;
         const RwChar *ResourceName;
         const void *pObject = RWS::CResourceManager::FindById (pResourceID, 
            &ResourceType, 0, &ResourceName);

         if (pObject)
         {
            // Setup new data pointer.

            if (*static_cast<RwInt32 *>(const_cast<void *>(pObject)) == -1)
            {
               // No new data, free all objects in the sequence.

               m_seqProcessor.FreeAll();
               RWS_DEBUGSTREAM_ERR("No sequence data within resource." << std::endl);
            }
            else
            {
               if (!rwstrcmp (SEQUENCE_DATA_RESOURCE_STRING, ResourceType))
               {
                  const CSeqCtrlData *pSeqCtrlData = static_cast<const CSeqCtrlData *>(pObject);

                  m_seqProcessor.Init(*pSeqCtrlData);
                  m_seqProcessor.DebugDisp(ResourceName, pResourceID);
               }
            }
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Standard RWS attribute handler.
      *
      */

      void CConductor::HandleAttributes(const CAttributePacket &attr)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CConductor::HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);
         HandleSystemCommands(attr);

         // Handle changes to control selections.

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CConductor));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {

            // Message linkage...

            case MSG_START:

               // Unlink and unregister old one - doesn't matter if there isn't an old one...

               UnLinkMsg(m_startMsg);
               UnRegisterMsg(m_startMsg);

               // Link to new message...

               RegisterMsg(m_startMsg, attrIt->GetAs_RwChar_ptr(), "RwUInt32*");
               LinkMsg(m_startMsg, "RwUInt32*");
               break;

            case CMD_LOOP_SEQ:
               {
                  RwBool flag = !(attrIt->GetAs_RwUInt32() == 0);

                  m_seqProcessor.Loop(flag);
               }
               break;

            case CMD_SEQ_SPEED:

               m_seqProcessor.SetRelativeSpeed(attrIt->GetAs_RwReal());
               break;
            }
            ++attrIt;
         }

         RWS_RETURNVOID();
      }

   }  // SEQUENCE
}     // RWS
