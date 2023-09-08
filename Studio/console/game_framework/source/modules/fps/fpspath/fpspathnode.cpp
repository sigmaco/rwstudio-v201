/*****************************************************************************
*
* File :     FPSPathNode.cpp
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
#include "fpspathnode.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace FPS
   {
      RWS_IMPLEMENT_CLASSID(FPSPathNode);
      RWS_REGISTERCLASS(FPSPathNode);
      
      CEventId FPSPathNode::m_pathcorner;
      
      
      /**
      *
      * \ref FPSPathNode. FPSPathNode Constructor.
      *
      * \see ~FPSPathNode.
      *
      */
      FPSPathNode::FPSPathNode(const CAttributePacket& attr) : InitCEventHandler(&m_pAtomic) , m_wait(0)
      {
         RWS_FUNCTION("RWS::FPS::FPSPathNode::FPSPathNode");
         
         // Can't use initializer ('this' isn't valid) so...
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         
         RegisterMsg(m_pathcorner, "ptr_FPSPathNode", "FPSPathNode*");
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref ~FPSPathNode. FPSPathNode Destructor.
      *
      * \see FPSPathNode
      *
      */
      FPSPathNode::~FPSPathNode(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSPathNode::~FPSPathNode");
         
         UnLinkMsg (m_targetname);
         UnRegisterMsg(m_targetname);
         UnRegisterMsg(m_target);
         UnRegisterMsg(m_pathcorner);
         
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
      void FPSPathNode::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSPathNode::HandleEvents");
         
         if (pMsg.Id == m_targetname)
         {
            if (pMsg.pData)
            {
               CEventHandler *pCEventHandler = static_cast<CEventHandler*>(pMsg.pData);
               CMsg reply(m_pathcorner, static_cast<FPSPathNode*>(this));
               SendMsgToEventHandler(reply, pCEventHandler);
            }
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
      void FPSPathNode::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSPathNode::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         // Initialize contained class first
         m_pAtomic.HandleSystemCommands(attr); // Initialize atomic/clump/frame
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(FPSPathNode));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_targetname:
               ReplaceLinkedMsg(m_targetname, attrIt->GetAs_RwChar_ptr(), "CEventHandler*");
               break;
               
            case CMD_target:
               ReplaceRegisteredMsg(m_target, attrIt->GetAs_RwChar_ptr(), "CEventHandler*");
               break;
               
            case CMD_wait:
               {
                  RwReal wait;
                  
                  attrIt->GetCommandData(wait);
                  m_wait = static_cast<RwUInt32>(wait * RWS::MainLoop::Logic::Rate);   // Multiply by frame rate
               }
               break;
            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
   } // namespace FPS
}// namespace RWS