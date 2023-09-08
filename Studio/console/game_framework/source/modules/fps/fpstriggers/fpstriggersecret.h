/*****************************************************************************
*
* File :     FPSTriggerSecret.h
*
* Abstract : Used to count the total number of secret areas found, a secret area
* is defined by a piece of invisible geometry, when the collision occurs with the
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

#ifndef __FPS_TRIGGER_SECRET_H__
#define __FPS_TRIGGER_SECRET_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace FPS
   {
      /**
      *
      *  \ingroup Mod_FPS
      *
      * Used to count the total number of secret areas found, a secret area
      * is defined by a piece of invisible geometry, when the collision occurs with the
      * geometry the m_targetname event is triggered, causing m_Secrets_Found to be 
      * incremented.
      *
      */
      class FPSTriggerSecret: public CSystemCommands, public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(FPSTriggerSecret);
         RWS_DECLARE_CLASSID(FPSTriggerSecret);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Secret Area Trigger", "Triggered By Player Touch Signifying Entry To The Secret Area");
         
         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);
         
         FPSTriggerSecret(const CAttributePacket&);
         ~FPSTriggerSecret(void);
         void IncreaseSecretsFound( void );
         
         static RwUInt32 m_Secrets_Found;   /**< Total number of secrets found */
         
      protected:          
      static CEventId m_targetname;      /**< Received to trigger FPSTriggerSecret, this member variable is static as all
                                              FPSTriggerSecret are triggered by the same event i.e. ACTN_PLAYERTOUCH */
      
      CAtomicPtr m_pAtomic;              /**< Pointer to 3D representation of this entity */
      };
   }
}
#endif
