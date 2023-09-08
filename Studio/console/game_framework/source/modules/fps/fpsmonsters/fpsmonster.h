/*****************************************************************************
*
* File :     FPSMonster.h
*
* Abstract : Generic monster behavior
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
*/

#ifndef __FPS_MONSTER_H__
#define __FPS_MONSTER_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"

#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/toolkits/collision/collide.h"

#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace FPS
   {
      /**
      *
      *  \ingroup Mod_FPS
      *
      * Generic monster behavior.  This behavior serves as a skeleton behavior for creating monsters.  
      * This behavior will follow FPSPathNode points around the world;  and if the player comes into 
      * the view cone of the monster and is within line of site,  the monster will turn and face the 
      * player and go into attack mode.  Currently the monster won't do anything in attack mode,  it is up to
      * the user to add in the animations/logic for the actual attack,  the same goes for the monsters
      * guarding mode.  FPSMonster only provides the basic functionality to get you starting.
      *
      *
      * You cannot just drop FPSMonster into your world;  you will need to create a derived version of this class.
      * See FPSExampleMonster,  This class derives from FPSMonster and sets up some default parameters, like height, width
      * and height from ground.  This lets you create monsters of different types.
      *
      * \see FPSExampleMonster
      */
      class FPSMonster : public CSystemCommands, public CAttributeHandler , public CEventHandler, public LinearAllocationPolicy
      {
      public:
         RWS_DECLARE_CLASSID(FPSMonster);
         
         RWS_BEGIN_COMMANDS
            RWS_MESSAGE  (CMD_target,
            "Target",
            "The triggername of a FPSPathNode /or a teleport ?",
            TRANSMIT,
            CEventHandler*,
            0)
            
            RWS_ATTRIBUTE(CMD_m_turn_factor,
            "Turn rate",
            "1 = instant 0 = never.",
            SLIDER,
            RwReal,
            RANGE(0, 0.1, 1))
            
            RWS_ATTRIBUTE(CMD_m_fwd_speed,
            "Forward movement rate",
            "Forwards velocity 0 stationary 5 fast.",
            SLIDER,
            RwReal,
            RANGE(0, 1, 5))
            
            RWS_ATTRIBUTE(CMD_m_close_to_node,
            "Distance when close to node",
            "Distance used to determine if a monster has reached a specified path node.",
            SLIDER,
            RwReal,
            RANGE(0, 20, 40))
            
            RWS_ATTRIBUTE(CMD_ignorePlayer,
            "Ignore Player",
            "If checked the monster will ignore the player.",
            BOOLEAN,
            RwUInt32,
            DEFAULT(0))
            
            RWS_END_COMMANDS;
         
         
         enum states
         {
            State_Init,
               State_Moving,
               State_Delayed,
               State_Guard,
               State_Attack 
         };
         
         virtual void HandleAttributes(const CAttributePacket& attr);
         virtual void HandleEvents(CMsg &pMsg);
         
         FPSMonster(const CAttributePacket&);
         ~FPSMonster(void);
         
      protected:
         
         RwUInt32 m_state;                       /**< Current state  */            
         RwV3d m_dst_vector;                     /**< Target Destination vector */            
         CEventId m_target;                      /**< The triggername of a pathcorner/or a teleport */

         RwBool m_ignorePlayer;                  /**< Indicates if the monster should ignore the player.
         Default is FALSE. */

         static CEventId m_pathcorner;           /**< A FPSPathNode response to m_target event */            
         static CEventId Event_INQ_POSITION;     /**< Inquire position of player */
         static CEventId Event_INQ_RpAtomic;     /**< Inquire RpAtomic of player */            
         static CEventId m_Msg_Shot;             /**< Event received that tells the monster its been shot */            
         static CEventId m_Msg_MonsterShot;      /**< Event sent that tells others that the monster has been shot */

         CAtomicPtr m_pAtomic;                   /**< Clump/Atomic/Frame Extension */

         RwUInt32 m_count;                       /**< A counter, used to count up to m_num_frames_to_move and m_num_frames_to_delay */
         RwUInt32 m_num_frames_to_delay;         /**< Number of frames to wait when target is reached */
         RwReal m_turn_factor;                   /**< Multiplied by angle difference when tracking to determine turn speed */
         RwReal m_fwd_speed;                     /**< Forward Speed */
         RwReal m_close_to_node;                 /**< Close to node distance test */

         RwReal m_height;                        /**< height above ground */
         RwReal m_height_on_ground;              /**< height use to distinguish between ground and walls */
         RwReal m_width;                         /**< width of character */

         RwBool TriggerTarget(void);               /* Trigger the target event */            
         bool Proc_m_pathcorner(CMsg &pMsg);     /* If the response the trigger event is a path corner */

         // If the response to the trigger is a teleport
         //
         // Proc_teleport

         void Proc_iMsgRunningTick_State_Init(void);
         void Proc_iMsgRunningTick_State_Moving(CMsg &pMsg);
         void Proc_iMsgRunningTick_State_Delayed(CMsg &pMsg);
         void Proc_iMsgRunningTick_State_Guard(CMsg &pMsg);
         void Proc_iMsgRunningTick_State_Attack(void);

         void ApplyCollisionDetection(void);

         // Static function as used as callback for RenderWare func
         //
         static RpCollisionTriangle *CheckLineOfSightCallBack(
            RpIntersection * intersection,
            RpWorldSector * sector,       
            RpCollisionTriangle * collTriangle,
            RwReal distance,
            void *data);

         static RpAtomic *AtomicCollideCallBack(
            RpIntersection * intersection,
            RpWorldSector * sector,       
            RpAtomic * atomic,
            RwReal distance,
            void *data);

         RwBool SearchForPlayer(CMsg &pMsg);
         RwBool CheckLineOfSight(RpAtomic *pDstRpAtomic);

         static RwUInt32 monster_unique_id;
         RwUInt32 m_monster_unique_id;
      };
   } // namespace FPS
}// namespace RWS

#endif