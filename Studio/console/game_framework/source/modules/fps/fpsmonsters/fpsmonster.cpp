/*****************************************************************************
*
* File :     FPSMonster.cpp
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
*****************************************************************************/
#include "precomp.h"
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Module Includes
//
#include "fpsmonster.h"
#include "../fpspath/fpspathnode.h"

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/toolkits/world\clevel.h"
#include "framework/toolkits/world/factory.h"
#include "framework/mainloop/logic.h"
#include "framework/toolkits/math/maths.h"
#include "modules/generic/cdebugtools/cdebugtools.h"

namespace RWS
{
   namespace FPS
   {       
      RWS_IMPLEMENT_CLASSID(FPSMonster);
      
      // View cone angle of monsters
      const RwReal view_cone_angle = static_cast<RwReal>(RwCos(RWDEG2RAD(45.0f)));
      
      // Inquire the position of the player
      CEventId FPSMonster::Event_INQ_POSITION;
      
      // Inquire the RpAtomic of the player
      CEventId FPSMonster::Event_INQ_RpAtomic;
      
      // Event/Message received indicates that the monster has been shot by the player
      CEventId FPSMonster::m_Msg_Shot;
      
      // Event/Message sent to indicate that the monster has been shot, can then be used
      // to link to a particle effect and generate gibs.
      CEventId FPSMonster::m_Msg_MonsterShot;
      
      // Unique number assigned to the monster, useful for distributing
      // CPU intensive tasks over many frames.
      RwUInt32 FPSMonster::monster_unique_id = 0;
      
      CEventId FPSMonster::m_pathcorner;
      
      
      /**
      *
      * \ref FPSMonster. Constructor for FPSMonster.
      *
      * \see ~FPSMonster.
      *
      */
      FPSMonster::FPSMonster(const CAttributePacket& attr) : 
      InitCEventHandler(&m_pAtomic),
         m_state(State_Init), 
         m_turn_factor(0.1f),
         m_fwd_speed(1.0f),
         m_close_to_node(20.0f*20.0f)
      {
         RWS_FUNCTION("RWS::FPS::FPSMonster::FPSMonster");

         // Can't use initializer ('this' isn't valid) so...
         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);
         
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
         
         // Assign unique Id to this monster, see declaration comment
         monster_unique_id ++;
         
         m_monster_unique_id = monster_unique_id;
         
         // Link to running tick
         LinkMsg(iMsgRunningTick, 0);
         
         // Inquire the position of the player
         RegisterMsg(Event_INQ_POSITION, "Event_INQ_PLYR_POSITION", "return RwMatrix*");
         
         // Inquire the RpAtomic of the player
         RegisterMsg(Event_INQ_RpAtomic, "Event_INQ_PLYR_RpAtomic", "return RpAtomic*");
         
         // Receive new path corner
         RegisterMsg(m_pathcorner, "ptr_FPSPathNode", "FPSPathNode*");
         LinkMsg(m_pathcorner);
         
         RegisterMsg(m_Msg_Shot, "ACTN_PLAYERSHOOT", "CEventHandler*");
         RegisterMsg(m_Msg_MonsterShot, "ACTN_MONSTERSHOT", "RwFrame*");

         RWS_RETURNVOID();
      }
      
      
      /**
      *
      *
      * \ref ~FPSMonster. Destructor for FPSMonster.
      *
      * \see FPSMonster
      *
      */
      FPSMonster::~FPSMonster(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSMonster::~FPSMonster");

         UnLinkMsg( iMsgRunningTick );
         
         UnRegisterMsg( m_target );
         UnRegisterMsg( Event_INQ_POSITION );
         UnRegisterMsg( Event_INQ_RpAtomic );
         
         UnLinkMsg( m_pathcorner );
         UnRegisterMsg( m_pathcorner );
         
         UnRegisterMsg(m_Msg_Shot);
         UnRegisterMsg(m_Msg_MonsterShot);

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
      void FPSMonster::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSMonster::HandleEvents");

         if (pMsg.Id == iMsgRunningTick)
         {
            switch (m_state)
            {
            case State_Init:
               Proc_iMsgRunningTick_State_Init();
               break;
               
            case State_Moving:
               Proc_iMsgRunningTick_State_Moving(pMsg);
               break;
               
            case State_Delayed:
               Proc_iMsgRunningTick_State_Delayed(pMsg);
               break;
               
            case State_Guard:
               Proc_iMsgRunningTick_State_Guard(pMsg);
               break;
               
            case State_Attack:
               Proc_iMsgRunningTick_State_Attack();
               break;
            }
         }
         else if (pMsg.Id == m_pathcorner)
         {
            Proc_m_pathcorner(pMsg);
         }
         else if (pMsg.Id == m_Msg_Shot)
         {
            // Transmit event that the monster has been shot
            CMsg IveBeenShot(m_Msg_MonsterShot, RpAtomicGetFrame(m_pAtomic.ptr()) );
            
            SendMsg(IveBeenShot);
            
            Delete();
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
      void FPSMonster::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSMonster::HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);
         
         m_pAtomic.HandleSystemCommands(attr);
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(FPSMonster));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_target:
               ReplaceRegisteredMsg(m_target, attrIt->GetAs_RwChar_ptr(), "CEventHandler*");
               
               // Reset the monster, in case this is a path node to follow
               m_state = State_Init;
               break;
               
            case CMD_m_turn_factor:
               attrIt->GetCommandData(m_turn_factor);
               break;
               
            case CMD_m_fwd_speed:
               attrIt->GetCommandData(m_fwd_speed);
               break;
               
            case CMD_m_close_to_node:
               attrIt->GetCommandData(m_close_to_node);
               m_close_to_node = m_close_to_node*m_close_to_node;
               break;

            case CMD_ignorePlayer:
               
               // Set ignore flag passed on value sent in.

               m_ignorePlayer = (attrIt->GetAs_RwUInt32() != 0);
               break;
            }
            ++attrIt;
         }

         RWS_RETURNVOID();
      }
      
      
      /**
      *
      *
      * \ref Trigger Target. the m_target event could be linked to a path corner or a teleport
      *  we need to wait for a response in order to determine which it is.
      *
      */
      RwBool FPSMonster::TriggerTarget(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSMonster::TriggerTarget");
         
         // the m_target event could be linked to a path corner or a teleport
         // we need to wait for a response in order to determine which it is.
         CMsg CMsg(m_target, static_cast<CEventHandler*>(this));
         
         // If any response return true
         
         RWS_RETURN(SendMsg(CMsg));
      }
      
      
      /**
      *
      *
      * \ref GetNextDestination. get the next destination node.
      *
      */
      bool FPSMonster::Proc_m_pathcorner(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSMonster::GetNextDestination");
         
         FPSPathNode *pFPSPathNode = static_cast<FPSPathNode*>(pMsg.pData);
         
         if (pFPSPathNode) // Got Response
         {
            // Set m_target to the next path_corner
            UnRegisterMsg(m_target);
            m_target = pFPSPathNode->Get_m_target();
            
            // Get delay time
            m_num_frames_to_delay = pFPSPathNode->Get_m_wait();
            
            // Need to store a copy of the destination point
            RwFrame *pDstRwFrame = pFPSPathNode->Get_RwFrame();
            
            if (pDstRwFrame)
            {
               RwMatrix *pDstMatrix = RwFrameGetLTM(pDstRwFrame);
               
               if (pDstMatrix)
               {
                  m_dst_vector = pDstMatrix->pos;
               }
            }
            
            m_state = State_Moving;
            
            RWS_RETURN(true);
         }
         
         RWS_RETURN(false);
      }
      
      
      /**
      *
      *
      * \ref Proc_iMsgRunningTick_State_Init.  Initial state for FPSMonster.  
      * The monster will stay in this state until a target is found for it.
      *       
      * \see Proc_iMsgRunningTick_State_Init 
      * \see Proc_iMsgRunningTick_State_Moving 
      * \see Proc_iMsgRunningTick_State_Delayed
      * \see Proc_iMsgRunningTick_State_Guard
      * \see Proc_iMsgRunningTick_State_Attack
      *
      */
      void FPSMonster::Proc_iMsgRunningTick_State_Init(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSMonster::Proc_iMsgRunningTick_State_Init");

         // Apply collision detection, if the monster is standing on a lift then it will be linked
         // to the frame of the lift and move with it.
         ApplyCollisionDetection();
         
         // Check if the monster has a m_target set, if it has then need
         // to find the position to move to...
         if (m_target != ERROR_NOT_A_VALID_MESSAGE_ID)
         {
            TriggerTarget();
            
            // Note this may fail if the target hasn't
            // been setup, i.e. there is no FPSPathNode
            // which will respond to the m_target event.
            //
            // For now keep sending the event until we find one.
            //
            // Otherwise m_state will be set to State_Moving and the monster
            // needs to turn and move towards the destination point.
         }
         else
         {
            m_state = State_Guard;
         }

         RWS_RETURNVOID();
      }
      
      
      /**
      *
      *
      * \ref ApplyCollisionDetection.  Apply collision detection between the monster and the world.
      *
      */
      void FPSMonster::ApplyCollisionDetection(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSMonster::ApplyCollisionDetection");

         // Check that we have a entity/frame
         //
         RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic.ptr());
         if (pFrame)
         {
            // Do collision Detection
            RwV3d E;
            
            RwBool m_onground = false;  
            
            E.x = m_width;
            E.y = m_height;
            E.z = m_width;
            
            CCollide::CEllipsoid::CalcCollision(this, 0, pFrame, 0, m_height_on_ground, &m_onground, &E);
         }

         RWS_RETURNVOID();
      }
      
      
      /**
      *
      *
      * \ref Monster Moving. If the monster cannot see a play in site,  then move to next target node.
      *
      * \param pMsg Standard Message Package.
      *
      * \see Proc_iMsgRunningTick_State_Init 
      * \see Proc_iMsgRunningTick_State_Moving 
      * \see Proc_iMsgRunningTick_State_Delayed
      * \see Proc_iMsgRunningTick_State_Guard
      * \see Proc_iMsgRunningTick_State_Attack
      *
      */
      void FPSMonster::Proc_iMsgRunningTick_State_Moving(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSMonster::Proc_iMsgRunningTick_State_Moving");

         if (!m_ignorePlayer && SearchForPlayer(pMsg))
         {
         }
         else
         {
            // Check that we have a entity/frame
            RwFrame *pSrcRwFrame = RpAtomicGetFrame(m_pAtomic.ptr());
            if (pSrcRwFrame)
            {
               // Get the matrix from the frame
               RwMatrix *pSrcMatrix = RwFrameGetLTM(pSrcRwFrame);
               
               // Need to calculate the current bearing of the character...
               RwReal my_bearing = RWRAD2DEG ( static_cast<RwReal>(RwATan2 (pSrcMatrix->right.x, pSrcMatrix->at.x)) );
               RwV2d v;
               
               v.x = m_dst_vector.x - pSrcMatrix->pos.x;
               v.y = m_dst_vector.z - pSrcMatrix->pos.z;
               
               RwReal dst_bearing = RWRAD2DEG ( static_cast<RwReal>(RwATan2 (v.y, v.x)) );
               
               RwReal angdiff =  DegAngDiff(dst_bearing, my_bearing) ;
               
               // Smooth filter heading, i.e. if diff is 10 degree's turn 5 degrees
               //
               angdiff *= m_turn_factor;
               
               RwFrameRotate (pSrcRwFrame, &YAxis, angdiff, rwCOMBINEPRECONCAT);
               
               // Move forwards
               //
               RwV3d v_forward = {0, -10.0f, m_fwd_speed}; // Fwd and gravity
               
               RwFrameTranslate (pSrcRwFrame, &v_forward, rwCOMBINEPRECONCAT);
               
               ApplyCollisionDetection();
               
               // Determine distance from waypoint
               RwV3d diff;
               
               RwV3dSub(&diff, &m_dst_vector, &pSrcMatrix->pos);
               
               diff.y = 0;
               
               RwReal distance = ( RwV3dDotProductMacro(&diff, &diff) );
               
               // Quick guess at a value for distance squared
               if (distance < (m_close_to_node))
               {
                  // Do we need to delay ?
                  if (m_num_frames_to_delay != 0)
                  {
                     m_count = 0;
                     m_state = State_Delayed;
                  }
                  else
                  {
                     // Find next path_corner if any, otherwise go into guard mode
                     if (!TriggerTarget())
                     {
                        m_state = State_Guard;
                     }
                  }
               }
            }
         }

         RWS_RETURNVOID();
      }
      
      
      /**
      *
      *
      * \ref Proc_iMsgRunningTick_State_Delayed. If player not in view then make monster wait at node.  
      * The amount to wait is taken from the path node.
      *
      * \param pMsg Standard Message Package.
      *
      * \see Proc_iMsgRunningTick_State_Init 
      * \see Proc_iMsgRunningTick_State_Moving 
      * \see Proc_iMsgRunningTick_State_Delayed
      * \see Proc_iMsgRunningTick_State_Guard
      * \see Proc_iMsgRunningTick_State_Attack
      *
      */
      void FPSMonster::Proc_iMsgRunningTick_State_Delayed(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSMonster::Proc_iMsgRunningTick_State_Delayed");

         if (!m_ignorePlayer && SearchForPlayer(pMsg))
         {
         }
         else
         {
            m_count++;
            
            if (m_count >= m_num_frames_to_delay)
            {
               // Find next path_corner if any, otherwise go into guard mode
               if (!TriggerTarget())
               {
                  m_state = State_Guard;
               }
            }
         }

         RWS_RETURNVOID();
      }
      
      class CollisionData
      {
      public:
         
         CollisionData(RpAtomic *_pSrcRpAtomic, RpAtomic *_pDstRpAtomic) : 
            clearlineofsight(true), pSrcRpAtomic(_pSrcRpAtomic), pDstRpAtomic(_pDstRpAtomic)
         {
         }
         
         RwBool clearlineofsight;
         
         RpAtomic *pSrcRpAtomic; // This monster
         RpAtomic *pDstRpAtomic; // The target
      };
      
      RpCollisionTriangle *FPSMonster::CheckLineOfSightCallBack(
         RpIntersection * intersection,
         RpWorldSector * sector,       
         RpCollisionTriangle * collTriangle,
         RwReal distance,
         void *data)
      {
         RWS_FUNCTION("RWS::FPS::FPSMonster::CheckLineOfSightCallBack");

         CollisionData *pCollisionData = reinterpret_cast<CollisionData*>(data);
         
         pCollisionData->clearlineofsight = false;
         
         // First collision that occurs exit, as this is a line of sight test by returning 0
         //
         RWS_RETURN( 0 );
      }
      
      
      /**
      *
      *
      * \ref Atomic collide callback. Check if the collision is with either the monster or the player,
      *
      */
      RpAtomic *FPSMonster::AtomicCollideCallBack(
         RpIntersection * intersection,
         RpWorldSector * sector,       
         RpAtomic * atomic,
         RwReal distance,
         void *data)
      {
         RWS_FUNCTION("RWS::FPS::FPSMonster::AtomicCollideCallBack");

         // Check if the collision is with either the monster or the player,
         // if it is continue checking for collisions...
         CollisionData *pCollisionData = reinterpret_cast<CollisionData*>(data);
         
         if (atomic == pCollisionData->pSrcRpAtomic)
         {
            RWS_RETURN(atomic);
         }
         if (atomic == pCollisionData->pDstRpAtomic)
         {
            RWS_RETURN(atomic);
         }
         
         pCollisionData->clearlineofsight = false;
         
         // First collision that occurs exit, as this is a line of sight test by returning 0
         RWS_RETURN( 0 );
      }
      
      
      /**
      *
      *
      * \ref CheckLineOfSight. Check monsters line of site for the player.
      *
      * \param pDstRpAtomic  Atomic for what we are testing line of site with.
      *
      * \return TRUE if a clear line of sight exists i.e. no collision occurred
      *
      * \see SearchForPlayer
      *
      */
      RwBool FPSMonster::CheckLineOfSight(RpAtomic *pDstRpAtomic)
      {
         RWS_FUNCTION("RWS::FPS::FPSMonster::CheckLineOfSight");

         RwFrame* pDstFrame = RpAtomicGetFrame(pDstRpAtomic);
         RwFrame* pSrcFrame = RpAtomicGetFrame(m_pAtomic.ptr());
         if (pDstFrame && pSrcFrame)
         {
            RwMatrix *pSrcRwMatrix = RwFrameGetLTM(pSrcFrame);
            RwMatrix *pDstRwMatrix = RwFrameGetLTM(pDstFrame);
            
            RpIntersection is;
            
            is.type = rpINTERSECTLINE;
            
            is.t.line.start = pSrcRwMatrix->pos;
            is.t.line.end = pDstRwMatrix->pos;
            
            CollisionData cdata(m_pAtomic.ptr(), pDstRpAtomic);
            
            // Collide with atomics first
            RpWorldForAllAtomicIntersections(CLevel::GetOpaqueWorld(), &is, AtomicCollideCallBack, &cdata);
            
            // If the line of sight test failed don't bother checking the world
            if (cdata.clearlineofsight)
            {
               // Fill in nc with nearest collision
               RpCollisionWorldForAllIntersections(CLevel::GetOpaqueWorld(), &is, CheckLineOfSightCallBack, &cdata);
            }
            
            // return true if a clear line of sight exists i.e. no collision occurred
            RWS_RETURN( cdata.clearlineofsight );
         }
         
         
         RWS_RETURN( false );
      }
      
      
      /**
      *     
      * \ref Search for Player.  See if player is in view cone,  if so perform a line of site test.
      *
      * \param pMsg Standard Message Package.
      *
      * \return TRUE if player in view and line of sight.
      *
      * \see SearchForPlayer
      *
      */
      RwBool FPSMonster::SearchForPlayer(CMsg &pMsg)
      {                       
         RWS_FUNCTION("RWS::FPS::FPSMonster::SearchForPlayer");

         // iMsgRunningTick passes the current frame number as its parameter
         RwUInt32 frame_number = reinterpret_cast<RwUInt32>(pMsg.pData);
         
         // Distribute line of sight tests over n frames i.e. each monster should
         // check once ever n frames for a line of sight
         if ( (frame_number&0x1f) == (m_monster_unique_id&0x1f) )
         {
            // Need location of the player, and the atomic of the player, if the ray test
            // hits the world then the test fails
            RwFrame* pMonsterFrame = RpAtomicGetFrame(m_pAtomic.ptr());
            if (pMonsterFrame)
            {
               // Need to check if the monster can see the player
               CMsg InqMsg(Event_INQ_RpAtomic);
               
               SendMsg(InqMsg); 
               
               // Did anything reply ?
               if (InqMsg.pData)
               {
                  RpAtomic *pAtomic = reinterpret_cast<RpAtomic*>(InqMsg.pData);
                  
                  // Need to check the direction of the character relative to the player                       
                  RwV2d v1;
                  
                  RwMatrix* pMonsterMat = RwFrameGetMatrix(pMonsterFrame);
                  v1.x = pMonsterMat->right.x;
                  v1.y = pMonsterMat->at.x;
                  
                  RwV2d v2;
                  
                  // This depends on the direction that has been chosen as forwards for the geometry
                  // in this case the positive Z axis has been chosen...
                  RwFrame* pPlayerFrame = RpAtomicGetFrame(pAtomic);
                  RWS_ASSERT(pPlayerFrame,
                        "Player object which was found does not have a frame to use for positioning.");
                  RwMatrix* pPlayerMat = RwFrameGetMatrix(pPlayerFrame);
                  v2.y = pPlayerMat->pos.x - pMonsterMat->pos.x;
                  v2.x = pPlayerMat->pos.z - pMonsterMat->pos.z;
                  
                  // Normalize the two vectors and take the dot product to obtain the angle difference
                  // between the two vectors.                        
                  RwV2dNormalize(&v1, &v1);
                  RwV2dNormalize(&v2, &v2);
                  
                  // |A||B|cos O = a.b
                  RwReal CosO = RwV2dDotProduct(&v1, &v2);
                  
                  // Check if the CosO is greater than our const view_cone_angle if it is then the 
                  // angular difference between the two angles is less than that specified by view_cone_angle.
                  if (CosO>view_cone_angle) 
                  {
                     if (CheckLineOfSight(pAtomic))
                     {
                        m_state = State_Attack;
                        
                        RWS_RETURN( true );
                     }
                  }
               }
            }
         }
         RWS_RETURN( false );
      }
      
      
      /**
      *
      *
      * \ref Proc_iMsgRunningTick_State_Guard.  If player not in sight,  then guard.  
      * The user needs to provide code to handle animation and logic for the guarding state.
      *
      * \param pMsg Standard Message Package.
      *
      * \see Proc_iMsgRunningTick_State_Init 
      * \see Proc_iMsgRunningTick_State_Moving 
      * \see Proc_iMsgRunningTick_State_Delayed
      * \see Proc_iMsgRunningTick_State_Guard
      * \see Proc_iMsgRunningTick_State_Attack
      *
      */
      void FPSMonster::Proc_iMsgRunningTick_State_Guard(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSMonster::Proc_iMsgRunningTick_State_Guard");

         if (!m_ignorePlayer && SearchForPlayer(pMsg))
         {
         }
         else
         {
            // Add code to animate monster while guarding here
         }

         RWS_RETURNVOID();
      }
      
      
      /**
      *
      *
      * \ref Proc_iMsgRunningTick_State_Attack. If the monster has the player in site it goes into the attack state,  
      * which will make the monster turn towards the player.
      *        
      * \param pMsg Standard Message Package.
      *
      * \see Proc_iMsgRunningTick_State_Init 
      * \see Proc_iMsgRunningTick_State_Moving 
      * \see Proc_iMsgRunningTick_State_Delayed
      * \see Proc_iMsgRunningTick_State_Guard
      * \see Proc_iMsgRunningTick_State_Attack
      *
      */
      void FPSMonster::Proc_iMsgRunningTick_State_Attack(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSMonster::Proc_iMsgRunningTick_State_Attack");

         // Need to check if the monster can see the player
         CMsg InqMsg(Event_INQ_POSITION);
         
         SendMsg(InqMsg); 
         
         // Did anything reply ?
         if (InqMsg.pData)
         {
            RwFrame* pMonsterFrame = RpAtomicGetFrame(m_pAtomic.ptr());
            if (pMonsterFrame)
            {
               RwMatrix *pSrcMatrix = RwFrameGetLTM(pMonsterFrame);
               RwMatrix *pDstmatrix = static_cast<RwMatrix*>(InqMsg.pData);
               
               // Need to calculate the current bearing of the character...
               RwReal my_bearing = RWRAD2DEG ( static_cast<RwReal>(RwATan2 (pSrcMatrix->right.x, pSrcMatrix->at.x)) );
               
               RwV2d v;
               
               // Determine bearing of character to target i.e. the player
               v.x = pDstmatrix->pos.x - pSrcMatrix->pos.x;
               v.y = pDstmatrix->pos.z - pSrcMatrix->pos.z;
               
               RwReal dst_bearing = RWRAD2DEG ( static_cast<RwReal>( RwATan2 (v.y, v.x) ));
               
               // Determine the angular difference between the character and the target
               RwReal angdiff =  DegAngDiff(dst_bearing, my_bearing) ;
               
               angdiff *= 0.1f;
               
               RwFrameRotate (pMonsterFrame, &YAxis, angdiff, rwCOMBINEPRECONCAT);
            }
         }

         RWS_RETURNVOID();
      }
   }// namespace FPS
}// namespace RWS
