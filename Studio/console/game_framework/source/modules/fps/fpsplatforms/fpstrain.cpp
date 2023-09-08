/*****************************************************************************
*
* File :     FPSTrain.cpp
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
#include "fpstrain.h"
#include "modules/fps/fpspath/fpspathnode.h"
#include "framework/mainloop/logic.h"
#include "framework/mainloop/mainloop.h"
#include "framework/toolkits/debugtools/debugtools.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace FPS
   {
      RWS_IMPLEMENT_CLASSID(FPSTrain);
      RWS_REGISTERCLASS(FPSTrain);
      
      CEventId FPSTrain::m_pathcorner;
      
      /**
      *
      * \ref HandleAttributes().  Handle Attributes for this behavior
      *
      * \param attr Standard Attribute Package       
      *
      */
      void FPSTrain::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSTrain::HandleAttributes");
         
         CAttributeHandler::HandleAttributes(attr);
         
         // Initialize contained class first
         m_pAtomic.HandleSystemCommands(attr);
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(FPSTrain));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_targetname:
               ReplaceLinkedMsg(m_targetname, attrIt->GetAs_RwChar_ptr(), 0);
               break;
            case CMD_target:
               ReplaceRegisteredMsg(m_target, attrIt->GetAs_RwChar_ptr(), "CEventHandler*");
               m_state = State_Initialize;
               break;
            case CMD_speed:
               attrIt->GetCommandData(m_speed);
               break;
            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref TriggerTarget(). The m_target event could be linked to a path corner 
      * or a teleport we need to wait for a response in order to determine which it is.
      *
      * \return RwBool
      *
      */
      RwBool FPSTrain::TriggerTarget(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSTrain::TriggerTarget");
         
         CMsg CMsg(m_target, static_cast<CEventHandler*>(this));
         
         // If any response return true
         //
         RWS_RETURN(SendMsg(CMsg));
      }
      
      
      /**
      *
      * \ref Proc_m_pathcorner().  Based on the source and destination positions of the train,  work 
      * out the direction vector and how long it will take to get there based on the frame rate.
      *
      * \param CMsg &pMsg, standard message packet.
      *
      *
      */
      void FPSTrain::Proc_m_pathcorner(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSTrain::Proc_m_pathcorner");
         
         FPSPathNode *pFPSPathNode = static_cast<FPSPathNode*>(pMsg.pData);
         
         if (pFPSPathNode) // Got Response
         {
            // Set m_target to the next path_corner
            //
            UnRegisterMsg(m_target);
            m_target = pFPSPathNode->Get_m_target();
            
            // Get delay time
            //
            m_num_frames_to_delay = pFPSPathNode->Get_m_wait();
            
            // Need to determine the translation required
            //
            RwFrame *pDstRwFrame = pFPSPathNode->Get_RwFrame();
            
            if (pDstRwFrame)
            {
               RwFrame *pSrcRwFrame = RpAtomicGetFrame(m_pAtomic.ptr());
               if (pSrcRwFrame)
               {
                  RwMatrix *pSrcMatrix = RwFrameGetLTM(pSrcRwFrame);
                  RwMatrix *pDstMatrix = RwFrameGetLTM(pDstRwFrame);
                  
                  if (pSrcMatrix && pDstMatrix)
                  {
                     RwV3d delta;
                     RwReal distance;
                     
                     RwV3dSub(&delta, &pDstMatrix->pos, &pSrcMatrix->pos);
                     
                     // Determine how far the object will move
                     
                     rwSqrt(&distance, (RwV3dDotProduct(&delta, &delta)));
                     
                     // Determine the number of frames to transform for, and the delta to apply
                     //
                     RwReal num_frame_to_move = RWS::MainLoop::Logic::Rate * (distance / static_cast<RwReal>(m_speed));
                     
                     RwV3dScale(&delta, &delta, (1.0f / num_frame_to_move));
                     
                     m_num_frames_to_move = static_cast<RwUInt32>(num_frame_to_move);
                     
                     // Generate the transformation matrix
                     //
                     RwMatrixTranslate(m_pRwMatrix, &delta, rwCOMBINEREPLACE);
                     
                     LinkMsg(iMsgRunningTick);
                     
                     m_count = 0;
                     
                     m_state = State_Moving;
                  }
               }
            }
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref HandleEvents(). Handle events       
      *
      * \param msg Standard Message Package
      *  
      * \ref iMsgRunningTick
      */
      void FPSTrain::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSTrain::HandleEvents");
         
         switch (m_state)
         {
            // On Startup move to first node
            //
         case State_Initialize:
            if (pMsg.Id == iMsgStartSystem)
            {
               // Unlink from iMsgStartSystem since creating a new entity resends the message
               // to every entity and we only want to do this once
               UnLinkMsg(iMsgStartSystem);

               if (!TriggerTarget())
               {
                  m_state = State_Finished;
               }
            }
            else if (pMsg.Id == m_pathcorner)
            {
               FPSPathNode *pFPSPathNode = static_cast<FPSPathNode*>(pMsg.pData);
               
               if (pFPSPathNode)
               {
                  RwFrame *pStartAtRwFrame = pFPSPathNode->Get_RwFrame();
                  
                  RwFrameTransform(RpAtomicGetFrame(m_pAtomic.ptr()), RwFrameGetLTM(pStartAtRwFrame), rwCOMBINEREPLACE);
                  
                  // Set m_target to the next path_corner
                  //
                  UnRegisterMsg(m_target);
                  m_target = pFPSPathNode->Get_m_target();
                  
                  // Get delay time
                  //
                  m_num_frames_to_delay = pFPSPathNode->Get_m_wait();
                  
                  m_state = State_Waiting_To_Trigger;
               }
            }
            break;
            // Wait for trigger event before proceeding to next node.
            //
         case State_Waiting_To_Trigger:
            if (pMsg.Id == m_targetname)
            {
               if (!TriggerTarget())
               {
                  m_state = State_Finished;
               }
            }
            else if (pMsg.Id == m_pathcorner)
            {
               Proc_m_pathcorner(pMsg);
            }
            break;
            // Moving between path nodes
            //
         case State_Moving:
            if (pMsg.Id == iMsgRunningTick)
            {
               RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic.ptr());
               if (pFrame)
               {
                  RwFrameTransform(pFrame, m_pRwMatrix, rwCOMBINEPOSTCONCAT);
               }
               
               m_count ++;
               
               if (m_count >= m_num_frames_to_move)
               {
                  if (m_num_frames_to_delay == 0)
                  {
                     UnLinkMsg(iMsgRunningTick);
                     
                     if (!TriggerTarget())
                     {
                        // No response => no more path nodes.
                        //
                        m_state = State_Finished;
                     }
                  }
                  else
                  {
                     m_count = 0;
                     m_state = State_Delayed;
                  }
               }
            }
            else if (pMsg.Id == m_pathcorner)
            {
               Proc_m_pathcorner(pMsg);
            }
            break;
            // Waiting at path node
            //
         case State_Delayed:
            if (pMsg.Id == iMsgRunningTick)
            {
               m_count ++;
               
               if (m_count >= m_num_frames_to_delay)
               {
                  UnLinkMsg(iMsgRunningTick);
                  
                  if (!TriggerTarget())
                  {
                     m_state = State_Finished;
                  }
               }
            }
            else if (pMsg.Id == m_pathcorner)
            {
               Proc_m_pathcorner(pMsg);
            }
            break;
            // No More path nodes.
            //
         case State_Finished:
            break;
         }
           
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      *
      * \ref FPSTrain(). FPSTrain Constructor
      *
      * \see ~FPSTrain
      */
      FPSTrain::FPSTrain(const CAttributePacket& attr) : 
      InitCEventHandler(&m_pAtomic),
         m_state(State_Initialize),
         m_num_frames_to_delay(0),
         m_speed(100)
      {
         RWS_FUNCTION("RWS::FPS::FPSTrain::FPSTrain");
         
         // Can't use initializer ('this' isn't valid) so...
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         
         m_pRwMatrix = RwMatrixCreate();
         
         RegisterMsg(m_targetname, "ACTN_PLAYERTOUCH", 0);
         LinkMsg(m_targetname);
         
         // Receive new path corner
         //
         RegisterMsg(m_pathcorner, "ptr_FPSPathNode", "FPSPathNode*");
         LinkMsg(m_pathcorner);
         
         LinkMsg(iMsgStartSystem);
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref ~FPSTrain().  Destructor for FPSTrain
      *
      * \see FPSTrain
      *
      */
      FPSTrain::~FPSTrain(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSTrain::~FPSTrain");
         
         RwMatrixDestroy(m_pRwMatrix);
         
         UnLinkMsg(iMsgRunningTick);
         UnLinkMsg(m_targetname);
         UnRegisterMsg(m_targetname);
         UnRegisterMsg(m_target);
         
         UnLinkMsg(m_pathcorner);
         UnRegisterMsg(m_pathcorner);
         
         UnLinkMsg(iMsgStartSystem);
         
         RWS_RETURNVOID();
      }
   }
}
