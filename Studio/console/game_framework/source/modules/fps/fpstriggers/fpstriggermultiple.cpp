/*****************************************************************************
*
* File :     FPSTriggerMultiple.h
*
* Abstract : 
*
* (1) Wait for the event (m_targetname)
*
* waits for (delay) seconds
*
* sends (m_target)
* deletes (m_killtarget) anything linked to this event
*
* waits for (wait) seconds
*
* goto (1)
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
#include "fpstriggermultiple.h"
#include "framework/toolkits/world/factory.h"
#include "framework/mainloop/logic.h"

namespace RWS
{
   namespace FPS
   {
      RWS_IMPLEMENT_CLASSID(FPSTriggerMultiple);
      RWS_REGISTERCLASS(FPSTriggerMultiple);
      
      /**
      *
      * \ref FPSTriggerMultiple. Constructor for FPSTriggerMultiple.
      *
      * \see ~FPSTriggerMultiple
      *
      */
      FPSTriggerMultiple::FPSTriggerMultiple(const CAttributePacket& attr) :
      InitCEventHandler(&m_pAtomic),
         m_state(State_Waiting),
         m_delay(0),
         m_delay_count(0),
         m_wait(0)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerMultiple::FPSTriggerMultiple");
         
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         
         // if no m_targetname is specified then use ACTN_PLAYERTOUCH   
         RegisterMsg(m_targetname, "ACTN_PLAYERTOUCH", 0);
         LinkMsg(m_targetname, 0);
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref FPSTriggerMultiple. Destructor for FPSTriggerMultiple.
      *
      * \see ~FPSTriggerMultiple.
      *
      */
      FPSTriggerMultiple::~FPSTriggerMultiple(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerMultiple::~FPSTriggerMultiple");
         
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
      void FPSTriggerMultiple::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerMultiple::HandleEvents");
         
         // Check the current state of the trigger    
         switch (m_state)
         {
         case State_Waiting:               
            if (pMsg.Id == m_targetname)
            {
               WaitThenSend();
            }
            break;
            
         case State_Delayed:
            if (pMsg.Id == iMsgRunningTick)
            {
               Delayed();
            }
            break;
            
         case State_Waiting_To_Reset:
            if (pMsg.Id == iMsgRunningTick)
            {
               WaitToReset( );
            }
            break;
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref WaitThenSend. If wait specified,  link to running tick and set flags to wait.  if no delay specified
      * send target event and kill event.
      *
      * \see WaitThenSend, Delayed, WaitToReset
      *
      */
      void FPSTriggerMultiple::WaitThenSend( void )
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerMultiple::WaitThenSend");
         
         // Has a delay been specified
         if (m_delay == 0)
         {
            // no, has a wait before reset been specified
            if (m_wait != 0)
            {
               // Link to running tick and wait for m_wait ticks                            
               LinkMsg(iMsgRunningTick, 0);
               
               m_delay_count = 0;
               m_state = State_Waiting_To_Reset; 
            }
            
            // Send event after reset as it could be used to re-trigger this entity
            // No Delay required, send trigger, send kill target
            SendMsg(m_target);
            
            RegisterToDeleteEventHandlers(m_killtarget);
         }
         else
         {
            // Link to running tick and wait m_delay ticks before firing trigger event
            LinkMsg(iMsgRunningTick, 0);
            
            m_delay_count = 0;
            m_state = State_Delayed;
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref Delayed. If delay specified, wait then send target event and kill event.
      *
      * \see WaitThenSend, Delayed, WaitToReset        
      *
      */
      void FPSTriggerMultiple::Delayed( void )
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerMultiple::Delayed");
         
         m_delay_count++;
         
         // Wait m_delay ticks 
         if (m_delay_count >= m_delay)
         {
            // Has a wait before reset been specified
            if (m_wait != 0)
            {
               // Yes
               m_delay_count = 0;
               m_state = State_Waiting_To_Reset; 
            }
            else
            {
               UnLinkMsg(iMsgRunningTick);
               m_state = State_Waiting;
            }
            
            // Send event after reset as it could be used to re-trigger this entity
            SendMsg(m_target);
            
            RegisterToDeleteEventHandlers(m_killtarget);
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref WaitToReset. Wait then reset.
      *
      * \see WaitThenSend, Delayed, WaitToReset
      *
      */
      void FPSTriggerMultiple::WaitToReset( void )
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerMultiple::WaitToReset");
         
         m_delay_count++;
         
         // wait m_wait ticks
         if (m_delay_count >= m_wait)
         {
            UnLinkMsg(iMsgRunningTick);
            m_state = State_Waiting;
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
      void FPSTriggerMultiple::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerMultiple::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(FPSTriggerMultiple));
         
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
               
            case CMD_wait:
               {
                  RwReal t;
                  attrIt->GetCommandData(t);
                  m_wait = static_cast<RwUInt32>(t * 60.0f);
               }
               break;
            }
            ++attrIt;
         }
         
         m_pAtomic.HandleSystemCommands(attr); // Initialize atomic/clump/frame
         
         RWS_RETURNVOID();
      }
   }//namespace
}