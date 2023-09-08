/*****************************************************************************
*
* File :     FPSPathNode.h
*
* Abstract : 
*
* Path corners are used to define paths, each path node is linked to the next node by way
* of the registered event. On receiving the target event a pointer to the FPSPathNode
* is returned...from this the wait and the name/Id of the next event can be determined.
*
* -> Event PathNode 1 -> Receive
*    FPSPathNode * <- Return
*
* Query FPSPathNode* to determine next path node
*
* -> Event PathNode 2 -> Receive
*    FPSPathNode * <- Return
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

#ifndef __FPS_PATH_NODE_H__
#define __FPS_PATH_NODE_H__

//////////////////////////////////////////////////////////////////
//
// Renderware Studio Includes
//
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/mainloop/logic.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace FPS
   {
      /**
      *
      *  \ingroup Mod_FPS
      *
      *  You can use the FPSPathNode entity to create a linked string of path points for something like a 
      *  FPSTrain to follow. Simply give them a Name (targetname) and Target the next FPSPathNode in the path.*
      *
      *  You can control whether the train stops at this point for a number of seconds (specified as the Wait value) 
      *  and give the train a new speed that will take effect once the train passes over the FPSPathNode.
      *
      */
      class FPSPathNode : public CSystemCommands, public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(FPSPathNode);
         RWS_DECLARE_CLASSID(FPSPathNode);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Path Node", "A path node");
         
         RWS_BEGIN_COMMANDS
            RWS_MESSAGE(   CMD_targetname,    "Target name", "Name used to identify this path corner"    ,   RECEIVE ,    CEventHandler*, 0)
            RWS_MESSAGE(   CMD_target    ,    "Target"    , "Name used to identify the next path corner",   TRANSMIT,    CEventHandler*, 0)                
            RWS_ATTRIBUTE( CMD_wait      ,    "Wait"      , "Delay when reaching the pathcorner seconds",   SLIDER  ,    RwReal        , RANGE(0, 0, 60))                
            RWS_END_COMMANDS;
         
         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);
         
         FPSPathNode(const CAttributePacket&);
         
         ~FPSPathNode(void);
         
         RwUInt32 Get_m_wait(void) 
         {
            return m_wait;
         }
         
         RwFrame *Get_RwFrame(void)
         {
            return RpAtomicGetFrame(m_pAtomic.ptr());
         }
         
         CEventId &Get_m_target(void)
         {
            return m_target;
         }
         
      protected:
         
         RwUInt32 m_wait;                /**< How long the monster/train will delay when reaching the pathcorner. If set to -1 it stops. */            
         CEventId m_targetname;          /**< The parameter that other pathcorners,trains or monsters use to refer to the pathcorner. */
         CEventId m_target;              /**< The triggername of the next FPSPathNode in the chain.  */
         
         static CEventId m_pathcorner;
         
         CAtomicPtr m_pAtomic;
      };
   }
}
#endif