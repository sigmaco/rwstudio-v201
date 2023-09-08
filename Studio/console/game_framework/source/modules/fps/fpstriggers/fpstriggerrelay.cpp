/*****************************************************************************
*
* File :     FPSTriggerRelay.cpp
*
* Notes : 
*
* Receives m_targetname
*
* Delays for m_delay ticks
*
* Sends m_target
*
* Deletes anything attached to m_killtarget
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
#include "fpstriggerrelay.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace FPS
   {
      RWS_IMPLEMENT_CLASSID(FPSTriggerRelay);
      RWS_REGISTERCLASS(FPSTriggerRelay);
      
      /**
      *
      * \ref FPSTriggerRelay. Constructor for FPSTriggerRelay.
      *
      * \see ~FPSTriggerRelay.
      *
      */
      FPSTriggerRelay::FPSTriggerRelay(const CAttributePacket& attr) : InitCEventHandler(&m_pAtomic), m_state(State_Waiting)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerRelay::FPSTriggerRelay");
         
         // Can't use initializer ('this' isn't valid) so...
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref ~FPSTriggerRelay. Destructor for FPSTriggerRelay.
      *
      * \see FPSTriggerRelay
      *
      */
      FPSTriggerRelay::~FPSTriggerRelay(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerRelay::~FPSTriggerRelay");
         
         UnLinkMsg(m_targetname);
         UnLinkMsg(iMsgRunningTick);
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
      void FPSTriggerRelay::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerRelay::HandleEvents");
         
         switch (m_state)
         {
         case State_Waiting:
            if (pMsg.Id == m_targetname)
            {
               TriggerEvent( );
            }
            break;
            
         case State_Delayed:
            if (pMsg.Id == iMsgRunningTick)
            {
               TriggerDelayed( );
            }
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref TriggerEvent. Trigger Target event and kill target.
      *
      * \see TriggerEvent, TriggerDelayed
      *
      */
      void FPSTriggerRelay::TriggerEvent( void )
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerRelay::TriggerEvent");
         
         if (m_delay == 0)
         {
            CMsg msg(m_target, RpAtomicGetFrame(m_pAtomic.ptr()));
            
            // Send Event m_target
            SendMsg(msg);
            
            // Kill all attached to Event m_killtarget
            RegisterToDeleteEventHandlers(m_killtarget);
         }
         else
         {
            LinkMsg(iMsgRunningTick);
            
            m_delay_count = 0;
            m_state = State_Delayed;
         }
         
         RWS_RETURNVOID();
      }
      
      
      /*
      *
      * \ref TriggerDelayed. Trigger event after delay.
      *
      * \see TriggerEvent, TriggerDelayed
      *
      */
      void FPSTriggerRelay::TriggerDelayed( void )
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerRelay::TriggerDelayed");
         
         m_delay_count++;
         
         if (m_delay_count >= m_delay)
         {
            CMsg msg(m_target, RpAtomicGetFrame(m_pAtomic.ptr()));
            
            // Send Event m_target
            SendMsg(msg);
            
            // Kill all attached to Event m_killtarget
            RegisterToDeleteEventHandlers(m_killtarget);
            
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
      void FPSTriggerRelay::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerRelay::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         // Initialize atomic/clump/frame
         m_pAtomic.HandleSystemCommands(attr); 
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(FPSTriggerRelay));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_targetname:
               ReplaceLinkedMsg(m_targetname, attrIt->GetAs_RwChar_ptr(), 0);
               break;
               
            case CMD_target:
               ReplaceRegisteredMsg(m_target, attrIt->GetAs_RwChar_ptr(), "RwFrame*");
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
                  
                  m_delay = static_cast<RwUInt32>(t * RWS::MainLoop::Logic::Rate);
               }
               break;
            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
   }
}