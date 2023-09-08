/*****************************************************************************
*
* File :     FPSTriggerOnce.cpp
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
#include "precomp.h"
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>
#include <rpworld.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpworld.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "fpstriggeronce.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace FPS
   {
      RWS_IMPLEMENT_CLASSID(FPSTriggerOnce);
      RWS_REGISTERCLASS(FPSTriggerOnce);
      
      /**
      *
      * \ref FPSTriggerOnce. Constructor for FPSTriggerOnce.
      *
      * \see ~FPSTriggerOnce
      *
      */
      FPSTriggerOnce::FPSTriggerOnce(const CAttributePacket& attr) : InitCEventHandler(&m_pAtomic)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerOnce::FPSTriggerOnce");
         
         // Can't use initializer ('this' isn't valid) so...
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         
         // if no m_targetname is specified then use ACTN_PLAYERTOUCH
         RegisterMsg(m_targetname, "ACTN_PLAYERTOUCH", 0);
         LinkMsg(m_targetname, 0);
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref ~FPSTriggerOnce. Destructor for FPSTriggerOnce.
      *
      * \see FPSTriggerOnce
      *
      */
      FPSTriggerOnce::~FPSTriggerOnce(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerOnce::~FPSTriggerOnce");
         
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
      void FPSTriggerOnce::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerOnce::HandleEvents");
         
         // Receive Event m_targetname
         if (pMsg.Id == m_targetname)
         {
            TriggerEvent();
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref TriggerEvent. Trigger event then kill all attached to event m_killtarget.  
      * Then kill this CEventHandler.
      *
      */
      void FPSTriggerOnce::TriggerEvent( void )
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerOnce::TriggerEvent");
         
         // Send Event m_target
         SendMsg(m_target);
         
         // Kill all attached to Event m_killtarget
         RegisterToDeleteEventHandlers(m_killtarget);
         
         // Delete this CEventHandler
         Delete();
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref HandleAttributes. Handle attribute updates.
      *
      * \param attr reference to a CAttributePacket.
      *
      */     
      void FPSTriggerOnce::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerOnce::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(FPSTriggerOnce));
         
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
            }
            ++attrIt;
         }
         
         m_pAtomic.HandleSystemCommands(attr); // Initialize atomic/clump/frame
         
         RWS_RETURNVOID();
      }
   }//namespace
}