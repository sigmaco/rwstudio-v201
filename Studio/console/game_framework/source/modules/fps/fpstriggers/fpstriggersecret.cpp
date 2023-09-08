/*****************************************************************************
*
* File :     FPSTriggerSecret.cpp
*
* Abstract : Used to count the total number of secret areas found, a secret area
* is defined by an piece of invisible geometry, when the collision occurs with the
* geometry the m_targetname event is triggered, causing m_Secrets_Found to be 
* incremented.
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
#include "fpstriggersecret.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace FPS
   {
      RWS_IMPLEMENT_CLASSID(FPSTriggerSecret);
      RWS_REGISTERCLASS(FPSTriggerSecret);
      
      RwUInt32 FPSTriggerSecret::m_Secrets_Found;
      CEventId FPSTriggerSecret::m_targetname;
      
      
      /**
      *
      * \ref FPSTriggerSecret. Constructor for FPSTriggerSecret.
      *
      * \see ~FPSTriggerSecret.
      *
      */
      FPSTriggerSecret::FPSTriggerSecret(const CAttributePacket& attr) : InitCEventHandler(&m_pAtomic)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerSecret::FPSTriggerSecret");
         
         // Can't use initializer ('this' isn't valid) so...
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         
         m_Secrets_Found = 0;
         
         RegisterMsg(m_targetname, "ACTN_PLAYERTOUCH",0);
         LinkMsg(m_targetname);
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref ~FPSTriggerSecret. Destructor for FPSTriggerSecret.
      *
      * \see FPSTriggerSecret.
      *
      */
      FPSTriggerSecret::~FPSTriggerSecret(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerSecret::~FPSTriggerSecret");
         
         UnLinkMsg(m_targetname);
         UnRegisterMsg(m_targetname);
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref HandleAttributes. Handle attribute updates.
      *
      * \param attr reference to a CAttributePacket.
      *
      */   
      void FPSTriggerSecret::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerSecret::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         // Initialize atomic/clump/frame
         m_pAtomic.HandleSystemCommands(attr);
         
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
      void FPSTriggerSecret::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerSecret::HandleEvents");
         
         // Handle trigger event
         if (pMsg.Id == m_targetname)
         {
            IncreaseSecretsFound();
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref IncreaseSecretsFound. Increase number of secrets found when triggered by an event.
      *
      */
      void FPSTriggerSecret::IncreaseSecretsFound( void )
      {
         RWS_FUNCTION("RWS::FPS::FPSTriggerSecret::IncreaseSecretsFound");
         
         // Increase total number of secrets found
         m_Secrets_Found++;
         
         RWS_TRACE("FPSTriggerSecret::HandleEvents m_Secrets_Found" << m_Secrets_Found);
         
         Delete();
         
         RWS_RETURNVOID();
      }
   }
}

