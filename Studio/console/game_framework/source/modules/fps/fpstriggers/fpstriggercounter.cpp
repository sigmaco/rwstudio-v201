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
#include "fpstriggercounter.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace FPS
   {
      RWS_IMPLEMENT_CLASSID(FPSTriggerCounter);
      RWS_REGISTERCLASS(FPSTriggerCounter);
      
      
      /**
      *
      * \ref FPSTriggerCounter. Constructor for FPSTriggerCounter.
      *
      * \see ~FPSTriggerCounter
      *
      */
      FPSTriggerCounter::FPSTriggerCounter(const CAttributePacket& attr) : 
      InitCEventHandler(&m_pAtomic),
         m_state(State_Waiting),
         m_count(2),
         m_rx_count(0),
         m_delay(0),
         m_delay_count(0)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerCounter::FPSTriggerCounter");
         
         // Can't use initializer ('this' isn't valid) so...
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref~FPSTriggerCounter. Destructor for FPSTriggerCounter
      *
      * \see FPSTriggerCounter
      *
      */           
      FPSTriggerCounter::~FPSTriggerCounter(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerCounter::~FPSTriggerCounter");
         
         UnLinkMsg(iMsgRunningTick);
         
         UnLinkMsg(m_targetname);
         
         UnRegisterMsg(m_targetname);
         UnRegisterMsg(m_target);
         UnRegisterMsg(m_killtarget);
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref HandleEvents(). Handle events.    
      *
      * \param pMsg Standard Message Package.
      *  
      * \ref iMsgRunningTick
      */
      void FPSTriggerCounter::HandleEvents(CMsg &pMsg)
      {            
         RWS_FUNCTION("RWS::FPS::FPSTriggerCounter::HandleEvents");
         
         // Check Current State
         switch (m_state)
         {
         case State_Waiting:         
            if (pMsg.Id == m_targetname)
            {
               TriggerEvent();
            }
            break;
            
         case State_Delayed:                 
            // Wait...for n seconds
            if (pMsg.Id == iMsgRunningTick)
            {
               WaitThenSend();
            }
            break;
            
         }
         
         RWS_RETURNVOID();
         
      }
      
      
      /**
      *
      * \ref TriggerEvent. Trigger an event when X amount of events have been received
      *       
      * \see WaitThenSend
      *
      */   
      void FPSTriggerCounter::TriggerEvent( void )
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerCounter::TriggerEvent");
         
         m_rx_count++;
         
         // Have the required number of events been received            
         if (m_rx_count >= m_count)
         {
            // Has a delay been specified               
            if (m_delay == 0)
            {
               // No, Send m_target event, send event, destroy kill targets and delete
               SendMsg(m_target);
               
               RegisterToDeleteEventHandlers(m_killtarget);
               
               Delete();
            }
            else
            {
               // Yes, Unlink from trigger event, attach to running tick and change to Delayed state                 
               UnLinkMsg(m_targetname);
               LinkMsg(iMsgRunningTick, 0);
               m_state = State_Delayed;
            }
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref WaitThenSend. Wait for X amount of time then send event and killevent.
      *
      * \see TriggerEvent
      *
      */   
      void FPSTriggerCounter::WaitThenSend( void )
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerCounter::WaitThenSend");
         
         m_delay_count++;
         
         // Delay complete ?
         if (m_delay_count >= m_delay)
         {
            // Yes, Send m_target event, send event, destroy kill targets and delete               
            SendMsg(m_target);
            
            RegisterToDeleteEventHandlers(m_killtarget);
            
            Delete();
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref HandleAttributes. Handle attribute updates.
      *
      * \param attr reference to a CAttributePacket.
      *
      */         
      void FPSTriggerCounter::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerCounter::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(FPSTriggerCounter));
         
         while (!attrIt.IsFinished())
         {  
            switch (attrIt->GetCommandId())
            {
            case CMD_targetname:
               {
                  const RwChar *name;
                  attrIt->GetCommandData(&name);
                  
                  UnLinkMsg (m_targetname);
                  UnRegisterMsg(m_targetname);
                  RegisterMsg(m_targetname, name, 0);
                  LinkMsg(m_targetname, 0);
               }
               break;
               
            case CMD_target:
               {
                  const RwChar *name;
                  attrIt->GetCommandData(&name);
                  
                  UnRegisterMsg(m_target);
                  RegisterMsg(m_target, name, 0);
               }
               break;
               
            case CMD_killtarget:
               {
                  const RwChar *name;
                  attrIt->GetCommandData(&name);
                  
                  UnRegisterMsg(m_killtarget);
                  RegisterMsg(m_killtarget, name, 0);
               }
               break;
               
            case CMD_delay:
               {
                  RwReal t;
                  attrIt->GetCommandData(t);
                  m_delay = static_cast<RwUInt32>(t * 60.0f);
               }
               break;
               
            case CMD_count:
               attrIt->GetCommandData(m_count);
               break;
            }
            ++attrIt;
         }
         
         m_pAtomic.HandleSystemCommands(attr);
         
         RWS_RETURNVOID();
      }
   }//namespace
}
