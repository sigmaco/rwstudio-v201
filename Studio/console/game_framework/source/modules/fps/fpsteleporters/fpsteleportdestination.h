/*****************************************************************************
*
* File :     FPSTeleportDestination.h
*
* Abstract : 
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

#ifndef __FPS_TELEPORT_DESTINATION_H__
#define __FPS_TELEPORT_DESTINATION_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/toolkits/world/factory.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace FPS
   {
      /**
      *
      *  \ingroup Mod_FPS
      *
      *   Defines a teleport destination for a trigger_teleport
      *
      *   Parameters 
      *   targetname This is the value that the trigger_teleport will need to target. 
      *
      */
      class FPSTeleportDestination : public CSystemCommands, public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(FPSTeleportDestination);
         RWS_DECLARE_CLASSID(FPSTeleportDestination);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Teleport End Point", "Specifies a teleport destination, see also FPSTeleport");
         
         FPSTeleportDestination(const CAttributePacket& attr) : InitCEventHandler(&m_pAtomic)
         {
            // Can't use initializer ('this' isn't valid) so...
            m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
            
            RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         }
         
         ~FPSTeleportDestination(void);
         
         RWS_BEGIN_COMMANDS
            RWS_MESSAGE( CMD_targetname,    "Target name", "Returns Position", RECEIVE, RwFrame**, 0)                               
         RWS_END_COMMANDS;
         
         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);
         
      protected:
         CEventId m_targetname;
         CAtomicPtr m_pAtomic;
      };
      
      
   }// namespace FPS
}// namespace RWS
#endif