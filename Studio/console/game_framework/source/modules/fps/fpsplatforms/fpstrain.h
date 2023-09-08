/*****************************************************************************
*
* File :     FPSTrain.h
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

#ifndef __FPSTrain_H__
#define __FPSTrain_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"
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
      *  Implementation of a basic moving train.  A train is an object that moves between a set of fixed points.
      *  These points are defined by using the \ref FPSPathNode.  
      *
      *  \see CAttributeHandler, CEventHandler, CSystemCommands
      *
      */
      class FPSTrain : public CSystemCommands, public CAttributeHandler , public CEventHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(FPSTrain);
         RWS_DECLARE_CLASSID(FPSTrain);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Path following Platform", "Alternate platform type, path is specified using multiple FPSPathNodes");
         
         virtual void HandleAttributes(const CAttributePacket& attr);            
         virtual void HandleEvents(CMsg &pMsg);
         
         FPSTrain(const CAttributePacket&);            
         ~FPSTrain(void);
         
         RwBool TriggerTarget(void);
         void Proc_m_pathcorner(CMsg &pMsg);
         
         enum states
         {
            State_Initialize,
               State_Waiting_To_Trigger,
               State_Moving,
               State_Delayed,
               State_Finished
         };
         
         RWS_BEGIN_COMMANDS                
            RWS_MESSAGE( CMD_targetname, "Target name", "Event which will activate the train", RECEIVE, 0, "ACTN_PLAYERTOUCH")
            RWS_MESSAGE( CMD_target,    "Target","The triggername of it's first pathcorner", TRANSMIT, CEventHandler*, 0)
            RWS_ATTRIBUTE( CMD_speed,   "Speed","Movement speed (100 default) units per second", SLIDER, RwUInt32, RANGE(1, 100, 999))                
            RWS_END_COMMANDS;
         
      protected:
         RwMatrix *m_pRwMatrix;          /**< Transformation matrix applied each frame while the entity is moving */            
         CEventId m_targetname;          /**< Activate train event */
         CEventId m_target;              /**< Get path node event */
         static CEventId m_pathcorner;   
         states m_state;                 /**< Current state see enum */
         RwUInt32 m_count;               /**< A counter, used to count up to m_num_frames_to_move and m_num_frames_to_delay */
         RwUInt32 m_num_frames_to_move;  /**< Number of frames to move for */
         RwUInt32 m_num_frames_to_delay; /**< Number of frames to wait when target is reached */
         RwUInt32 m_speed;               /**< Speed of object to move  */
         CAtomicPtr m_pAtomic;           /**< Clump/Atomic/Frame Extension */
      };
   }
}

#endif