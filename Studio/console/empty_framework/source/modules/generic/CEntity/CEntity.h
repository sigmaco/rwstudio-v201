/*****************************************************************************
*
* File :       CEntity.h
*
* Abstract :   Implement very simple behavior provided as a starting point
*              for the empty framework.
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

#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"

#include "framework/core/attributehandler/clinearmemorymanager.h"
#include "framework/core/attributehandler/allocation policies/dynamicallocationpolicy.h"

namespace RWS
{
   namespace Generic
   {
      /*
      *
      *  The empty framework version of CEntity is provided as part of the empty framework
      *  as a skeleton default behavior. 
      *
      *  \see CAttributeHandler
      *  \see CEventHandler
      *  \see CSystemCommands
      *
      */
      class CEntity : 
         public CSystemCommands, public CAttributeHandler , public CEventHandler, public DynamicAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(CEntity);
         RWS_DECLARE_CLASSID(CEntity);
         RWS_CATEGORY("Empty Framework Example");
         RWS_DESCRIPTION("CEntity", "A default behavior provided as an example for the empty framework");

         CEntity(const CAttributePacket& attr);
         ~CEntity(void);

         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);

      protected:
      };
   }//namespace Tutorial
}//namespace RWS