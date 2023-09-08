/*****************************************************************************
*
* File :     FPSTeleporter.h
*
* Abstract : Teleport the player or monster to an FPSTeleportDestination. 
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

#ifndef __FPS_TELEPORTER_H__
#define __FPS_TELEPORTER_H__

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
      *   When triggered it will teleport the player or monster to an FPSTeleportDestination. 
      *
      *   Parameters 
      *   TARGET     Corresponds to the name of the FPSTeleportDestination 
      *   TARGETNAME Handle that other triggers can use to trigger it.If present, will only teleport when triggered and not when touched. 
      */
      class FPSTeleporter : public CSystemCommands, public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
      public:
         
         RWS_MAKENEWCLASS(FPSTeleporter);
         RWS_DECLARE_CLASSID(FPSTeleporter);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Teleport Start Point", "Specifies a teleport destination, see also FPSTeleportDestination");
         
         FPSTeleporter(const CAttributePacket&);
         ~FPSTeleporter(void);
         
         RWS_BEGIN_COMMANDS
            RWS_MESSAGE( CMD_targetname,    "Target name", "Trigger Teleport"            , RECEIVE , CEventHandler*, "ACTN_PLAYERTOUCH")
            RWS_MESSAGE( CMD_target    ,    "Target"    , "Request Destination Position", TRANSMIT, RwFrame**     , 0)
            RWS_END_COMMANDS;
         
         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);
         
      protected:
         CEventId m_targetname;      /**< Event which triggers the teleport  */
         CEventId m_target;          /**< Event which finds the destination of the teleport */
                                     CEventId m_ACTN_TELEPORT;   /**< Event which tells the object being teleport, that 
                                     it has been teleported and where to go */           
                                     CAtomicPtr m_pAtomic;
      };
      
   }// namespace FPS
}// namespace RWS

#endif