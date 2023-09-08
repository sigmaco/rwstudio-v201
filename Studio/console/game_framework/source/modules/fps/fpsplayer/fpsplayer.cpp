/*****************************************************************************
*
* File :     FPSPlayer.cpp
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
* Copyright (c) 2002 Criterion Software Ltd.
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
#include "fpsplayer.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/debugtools/debugtools.h"
#include "framework/core/netstream/nettools.h"
#include "framework/mainloop/logic.h"
#include "framework/mainloop/render.h"
#include "framework/toolkits/world/factory.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/world/smartptrs/cclumpptr.h"
#include "framework/toolkits/world/helpers/camerahelper.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/collision/collide.h"
#include "modules/generic/cdebugtools/cdebugtools.h"

namespace RWS
{
   namespace FPS
   {
      
      RWS_IMPLEMENT_CLASSID(FPSPlayer);
      RWS_REGISTERCLASS(FPSPlayer);
      
      /**
      *
      * \ref HandleEvents(). Handle events.  Controls the state of a player,  
      *  e.g. running, jumping etc.  Also checks messages for pickups,  monsters etc.
      *
      * \param pMsg Standard Message Package
      *  
      * \ref iMsgRunningTick
      */
      void FPSPlayer::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::HandleEvents");
         
         if (pMsg.Id == iMsgRunningTick)
         {
            // Update Player Logic
            //
            switch (m_state)
            {
            case State_OnGround:
               State_OnGround_Proc_MsgRunningTick();
               break;
            case State_Jump:
               State_Jump_Proc_MsgRunningTick();
               break;
            case State_Falling:
               State_Falling_Proc_MsgRunningTick();
               break;
            case State_Landing:
               State_Landing_Proc_MsgRunningTick();
               break;
            case State_Last_State:
               break;
            }
            
            // Check if firing weapon ? check frame time and only fire every 4 frames
            //
            RwUInt32 frame = reinterpret_cast<RwUInt32>(pMsg.pData);
            
            if ((frame & 0x3) == 0)
            {
               CMsg InqFire(m_Msg_Inq_Fire_Action);
               
               if (SendMsg(InqFire))
               {
                  RwReal input = *reinterpret_cast<RwReal*>(&InqFire.pData);
                  
                  if (input > 0.5f)
                  {
                     Proc_FireWeapon();
                  }
               }
            }

            if (m_Tx_Pos.Get_registered_count() > 0)
            {
               // Send player position as event.
               //
               RwFrame *pFrame =  RpAtomicGetFrame(m_pAtomic.ptr());

               CMsg msg(m_Tx_Pos, pFrame);

               SendMsg(msg);
            }
         }
         if (pMsg.Id == m_Render_In && pMsg.pData && m_Render_Out.Get_registered_count())
         {
            const RwMatrix *matrix = RwFrameGetLTM(RpAtomicGetFrame(m_pAtomic.ptr()));
            RwCamera *pOutCam;
            RwFrame *pFrame;

            // Copy camera data into local copy, so original attributes are kept, but
            // original is not overwritten in any way...

            pOutCam = RwCameraClone(static_cast<RwCamera *>(pMsg.pData));
            RWS_ASSERT(pOutCam, "Failed to clone incoming camera");

            // Copy frame into temporary local one and attach (as will update matrix below).

            pFrame = RwFrameCreate();
            RwCameraSetFrame(pOutCam, pFrame);

            // Set the matrix inside this camera...

            RwFrame *cameraFrame = RwCameraGetFrame(pOutCam);
            RwFrameTransform(cameraFrame, matrix, rwCOMBINEREPLACE);
            
            // Generate viewing matrix

            RwMatrixRotate(m_ViewingMatrix, &XAxis, m_pitch, rwCOMBINEREPLACE);
            RwFrameTransform(cameraFrame, m_ViewingMatrix, rwCOMBINEPRECONCAT);
            
            // Offset camera from objects center

            RwV3d V3d = {0, m_cameraheight, 0};
            RwFrameTranslate(cameraFrame, &V3d, rwCOMBINEPOSTCONCAT);

            RwMatrix *cameraMatrix = RwFrameGetLTM(cameraFrame);
            RwFrameTransform(m_cameraFrame, cameraMatrix, rwCOMBINEREPLACE);

            CameraHelper::SetFieldOfView(m_ViewWindow, pOutCam);
            // Then send the modified camera onward...

            CMsg msg(m_Render_Out, pOutCam);
            SendMsg(msg);

            // Tidy up, destroy the camera - DOES NOT destroy frame (local) or z-buffer, etc, as these
            // are shared (via the clone) with the incoming camera.

            RwCameraSetFrame(pOutCam, 0);
            RwFrameDestroy(pFrame);
            RwCameraDestroy(pOutCam);
         }

         // Check if player has collected anything
         //
         if (pMsg.Id == m_Event_FPS_Example_Pickup)
         {
            RWS_TRACE("FPSPlayer::HandleEvents FPS_Example_Pickup Collected");
            DeletePickup(pMsg);
         }
         // Check if player has been teleported
         //
         if (pMsg.Id == m_Event_ACTN_TELEPORT)
         {
            RwFrame *pDestFrame = static_cast<RwFrame*>(pMsg.pData);
            
            RWS_TRACE("FPSPlayer::HandleEvents m_Event_ACTN_TELEPORT");
            
            RwFrame *pframe = RpAtomicGetFrame(m_pAtomic.ptr());
            if (pframe)
            {
               // Remove link if any and move player to new world position.
               if (RwFrameGetParent(pframe))
               {
                  RwMatrixCopy(RwFrameGetMatrix(pframe), RwFrameGetLTM(pframe));
                  RwFrameRemoveChild(pframe);
               }
               
               RwFrameTransform(pframe, RwFrameGetLTM(pDestFrame), rwCOMBINEREPLACE);
               
               // Zero all physics attributes
               ZeroVector(&m_force);
               ZeroVector(&m_velocity);
               
               ZeroVector(&m_ang_force);
               ZeroVector(&m_ang_velocity);
               m_teleported_flag = true;

               // run the collision so to ensure that the objects bound sphere does not intersect the world
               ApplyGravity();
               ApplyNumericalIntegration();
               ApplyCollisionDetectionSendMsg();


            }
         }

         // Check if player has touched FPSTriggerPush
         if (pMsg.Id == m_Event_FPS_trigger_push)
         {
            RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic.ptr());
            if (pFrame)
            {
               RwV3d *v = reinterpret_cast<RwV3d*>(pMsg.pData);
               
               // Add force to character, typically (depends on state) during the next tick 
               // the force will be applied to the object, and the force is set to zero.
               //
               // i.e. 
               //
               // Determine Force On Character
               // Apply Forces/Move Object
               // Zero Force
               // Do Collision
               //
               // Determine Force On Character
               // Apply Forces/Move Object
               // Zero Force
               // Do Collision
               // Add force due to push object
               //
               // m_force is the aligned to the direction of the character, so need
               // to apply the inverse transform.
               //
               RwMatrix *m = RwMatrixCreate();
               
               RwMatrixInvert(m, RwFrameGetLTM(pFrame));
               RwV3dTransformVectors(v, v, 1, m);
               RwMatrixDestroy(m);
               RwV3dAdd(&m_force, &m_force, v);
            }
         }
         // Inquire position message, sent by a monster to determine the position of the player,
         // note if this was a co-operative game this wouldn't work as all of the players
         // would respond each overwriting the value stored by the previous player.
         //
         // i.e. monster sends event
         //
         // player 1 fills in reply
         // player 2 fills in reply over-writing player 1's reply
         //
         // monster handles reply
         //
         if (pMsg.Id == m_Event_INQ_POSITION)
         {
            RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic.ptr());
            if (pFrame)
            {
               // Return world position...
               pMsg.pData = const_cast<void*>(reinterpret_cast<const void*>(RwFrameGetLTM(pFrame)));
            }
         }
         // Inquire RpAtomic, if asset has been attached m_pAtomic will point
         // to a RpAtomic
         //
         if (pMsg.Id == m_Event_INQ_RpAtomic)
         {
            RpAtomic *pAtomic = m_pAtomic.ptr();
            pMsg.pData = const_cast<void*>(reinterpret_cast<const void*>(pAtomic));
         }

#ifdef RWS_DESIGN
         // Workspace has requested the current camera, send current player camera as possible camera
         //
         if (pMsg.Id == iMsgRequestCurrentCameraMatrix && m_respondtocamerarequest)
         {
            NetTools::SendTransform(m_cameraFrame);
         }
#endif

         // iMsgPreCClumpPtrDestroy event, needed because the player can be linked to other objects
         // which may be destroyed while the player is attached to them.
         //
         if (pMsg.Id == ClumpHelper::iMsgPreRpClumpDestroy)
         {
            // Make sure the player isn't linked to anything...
            //
            RwFrame *frame = RpAtomicGetFrame(m_pAtomic.ptr());
            if (frame)
            {
               if (RwFrameGetParent(frame))
               {
                  RwMatrixCopy(RwFrameGetMatrix(frame), RwFrameGetLTM(frame));
                  RwFrameRemoveChild(frame);
               }
            }
         }

         // Send out an area trigger query to update the status of any triggers
         // the player may be colliding with.
         //
         if (pMsg.Id == m_Msg_AreaTriggerEvent)
         {
            if (m_Msg_AreaTriggerQuery.Get_registered_count() > 0)
            {
               RwFrame * pFrame = RpAtomicGetFrame(m_pAtomic.ptr());
               if (pFrame)
               {
                  AreaTrigger::ATCollisionQuery CollisionQuery;
                  CollisionQuery.pEventHandler = this;
                  CollisionQuery.Data.t.point = *RwMatrixGetPos(RwFrameGetMatrix(pFrame));
                  CollisionQuery.Data.type = rpINTERSECTPOINT;

                  CMsg Msg(m_Msg_AreaTriggerQuery, &CollisionQuery);
                  SendMsg(Msg);
               }
            }
         }

         RWS_RETURNVOID();
      }
   
   
      class RayCollisionData
      {
      public:
      
         RayCollisionData(CEventHandler *pCEventHandler) :
               MinDistance(1.0f),
               pRpAtomic(0),
               pHitRpAtomic(0),
               pCEventHandler(pCEventHandler){}
      
         RwReal MinDistance;
         RpAtomic *pRpAtomic;    // The nearest collision atomic
         RpAtomic *pHitRpAtomic; // The currently being processed atomic
         CEventHandler *pCEventHandler;
      };
   
   
      /**
      * 
      * \ref CollideCallBack.  Collision callback function
      *
      * \param intersection Pointer to the definition of the collision primitive e.g. a sphere.
      * \param collTriangle Pointer to the triangle which generated this collision.
      * \param distance     Distance from collision point to collision geometry.
      * \param data         Pointer to custom collision data.
      *
      * \return Polygon that has been intersected by a ray.
      *
      * \see AtomicCollideCallBack
      * \see WorldCollideCallBack
      *
      */
      RpCollisionTriangle *FPSPlayer::CollideCallBack( RpIntersection * intersection,
         RpCollisionTriangle * collTriangle, RwReal distance, void *data)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::CollideCallBack");

         RayCollisionData *pRayCollisionData = static_cast<RayCollisionData*>(data);
      
   #ifdef RWS_DEBUGTOOLS
         CCollide::DisplayCollisionTriangle(collTriangle->vertices,
            &collTriangle->normal,
            0, 255, 0);
   #endif
      
         if (distance<pRayCollisionData->MinDistance)
         {
            pRayCollisionData->MinDistance = distance;
         
            pRayCollisionData->pRpAtomic = pRayCollisionData->pHitRpAtomic;
         }
         RWS_RETURN( collTriangle );
      }
   
   
      /**
      *
      * \ref AtomicCollideCallBack. Atomic Collision Callback.  Determine atomic that has been collided with.  
      * When a gun is fired a ray is cast through the world. This callback returns an atomic that has been 
      * intersected by this ray.
      *
      * \param intersection Pointer to the definition of the collision primitive e.g. a sphere.
      * \param sector       Pointer to the sector containing this collision.
      * \param atomic       Pointer to the atomic which generated this collision.
      * \param distance     Distance from collision point to collision geometry.
      * \param data         Pointer to custom collision data
      *
      * \return Atomic that has collided with the ray
      *
      * \see CollideCallBack
      * \see WorldCollideCallBack
      * \see Proc_FireWeapon
      *
      */
      RpAtomic *FPSPlayer::AtomicCollideCallBack(RpIntersection * intersection,
            RpWorldSector * sector,
            RpAtomic * atomic,
            RwReal distance,
            void *data)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::AtomicCollideCallback");

         RayCollisionData *pRayCollisionData = static_cast<RayCollisionData*>(data);
      
         // Check if this atomic is flagged as solid, if not skip it.
         if (!RpAtomicCollisionProperties::GetIsSolid(*atomic)) return atomic;
      
         if (distance < pRayCollisionData->MinDistance)
         {
            CEventHandler *pCEventHandler = RpAtomicToEventHandler::GetEventHandler(*atomic);
         
            // Don't collide with self
            if (pCEventHandler != pRayCollisionData->pCEventHandler)
            {
               pRayCollisionData->pHitRpAtomic = atomic;
            
               RpAtomicForAllIntersections(atomic, intersection, CollideCallBack,data);
            
            }
         }
      
         RWS_RETURN( atomic );
      }

   
      /**
      *
      * \ref WorldCollideCallBack.   World Collision Callback.  Determine polygon that has been collided with.  
      * When a gun is fired a ray is cast through the world. This callback returns a poly that has been 
      * intersected by this ray.
      *
      * \param intersection Pointer to the definition of the collision primitive e.g. a sphere.
      * \param sector       Pointer to the sector containing this collision.
      * \param collTriangle Pointer to the triangle which generated this collision.
      * \param distance     Distance from collision point to collision geometry.
      * \param data         Pointer to custom collision data.
      *
      * \return Polygon that has been intersected by a ray.
      *
      * \see CollideCallBack
      * \see AtomicCollideCallBack
      * \see Proc_FireWeapon
      *
      */
      RpCollisionTriangle *FPSPlayer::WorldCollideCallBack(RpIntersection * intersection,
         RpWorldSector * sector,
         RpCollisionTriangle * collTriangle,
         RwReal distance,
         void *data)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::WorldCollideCallBack");
         
         RayCollisionData *pRayCollisionData = static_cast<RayCollisionData*>(data);
      
   #ifdef RWS_DEBUGTOOLS
         CCollide::DisplayCollisionTriangle(collTriangle->vertices, 
            &collTriangle->normal,
            0, 255, 0);
   #endif
      
         if (distance < pRayCollisionData->MinDistance)
         {
            pRayCollisionData->MinDistance = distance;
         }
      
         RWS_RETURN( collTriangle );
      }
   
   
      /**
      *
      * \ref Proc_FireWeapon. Func to test if weapon has hit anything, uses ray cast to determine
      * first thing the weapon hits.
      *
      * \see CollideCallBack
      * \see AtomicCollideCallBack
      * \see WorldCollideCallBack
      */
      void FPSPlayer::Proc_FireWeapon(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::Proc_FireWeapon");
         
         RwV3d p1;
         RwV3d p2;
      
         if (m_cameraFrame)
         {
            RwMatrix *matrix = RwFrameGetLTM(m_cameraFrame);
         
            if (matrix)
            {
               p1 = matrix->pos;
            
               p2.x = matrix->at.x;
               p2.y = matrix->at.y;
               p2.z = matrix->at.z;
            
               RwV3dScale(&p2, &p2, 1000.0f);
            
               RwV3dAdd(&p2, &p2, &p1);
            
               // Collision detections, line to world collision
               RpIntersection is;
            
               is.type = rpINTERSECTLINE;
               is.t.line.start = p1;
               is.t.line.end = p2;
            
               RayCollisionData tRayCollisionData(this);
            
               // Fill in tRayCollisionData with nearest collision
               RpCollisionWorldForAllIntersections(CLevel::GetOpaqueWorld(),
                  &is,
                  WorldCollideCallBack,
                  &tRayCollisionData);
            
               // Collide with atomics, find closest collision
               RpWorldForAllAtomicIntersections(CLevel::GetOpaqueWorld(),
                  &is,
                  AtomicCollideCallBack,
                  &tRayCollisionData);
            
   #ifdef RWS_DEBUGTOOLS
               {
                  RwV3d diff;
                  RwV3d hpoint;
                  RwV3d hpointA;
                  RwV3d hpointB;
               
                  RwV3dSub(&diff, &p2, &p1);
                  RwV3dScale(&diff, &diff, tRayCollisionData.MinDistance);
               
                  RwV3dAdd(&hpoint, &p1, &diff);
               
                  hpointA = hpoint;hpointA.x -= 10.0f;
                  hpointB = hpoint;hpointB.x += 10.0f;
               
                  DebugTools::Line(&hpointA, &hpointB, 255, 55, 255);
               
                  hpointA = hpoint;hpointA.y -= 10.0f;
                  hpointB = hpoint;hpointB.y += 10.0f;
               
                  DebugTools::Line(&hpointA, &hpointB, 255, 255, 255);
               
                  hpointA = hpoint;hpointA.z -= 10.0f;
                  hpointB = hpoint;hpointB.z += 10.0f;
               
                  DebugTools::Line(&hpointA, &hpointB, 255, 255, 255);
               }
   #endif
            
               if (tRayCollisionData.pRpAtomic)
               {
                  // Shot something, send it the ShotMsg
                  //
                  CEventHandler *pCEventHandler =
                     RpAtomicToEventHandler::GetEventHandler(*tRayCollisionData.pRpAtomic);
               
                  CMsg ShotMsg(m_Msg_Shot, static_cast<CEventHandler*>(this));
               
                  SendMsgToEventHandler(ShotMsg, pCEventHandler);
               }
            }
         }

         RWS_RETURNVOID();
      }
   
   
      /**
      * 
      * \ref ApplyInputPitchToViewMatrix. Polls the INQ_PITCH input and uses it to apply 
      * a transformation to the viewing matrix.
      *
      * \param scale_pitch Scaling factor for pitch.
      *
      * \see ApplyInputPitchToViewMatrix
      * \see ApplyInputFwdToForceZ
      * \see ApplyInputStrafeToForceX
      * \see ApplyInputTurnToAngForceY
      */
      void FPSPlayer::ApplyInputPitchToViewMatrix(RwReal scale_pitch)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::ApplyInputPitchToViewMatrix");
         
         // Apply pitch control to m_ViewingMatrix
         //
         CMsg InqMsg(m_Msg_Inq_Camera_Pitch);
         SendMsg(InqMsg);

         if (InqMsg.pData != 0)
         {
            RwReal pitch = scale_pitch * ( *reinterpret_cast<RwReal*>(&InqMsg.pData) );
         
            m_pitch += pitch;
         
            m_pitch = Clamp(m_pitch, -90.0f, 90.0f);
         }

         RWS_RETURNVOID();
      }
   
   
      /**
      * 
      * \ref ApplyInputFwdToForceZ. Polls the ACTN_FWD input and uses it to apply a
      * forwards or backwards force to the character.
      *
      * \param scale_fwd Scaling factor for forward movement.
      * \param scale_bak Scaling factor for backwards movement.
      *
      * \see ApplyInputPitchToViewMatrix
      * \see ApplyInputFwdToForceZ
      * \see ApplyInputStrafeToForceX
      * \see ApplyInputTurnToAngForceY
      *
      */
      void FPSPlayer::ApplyInputFwdToForceZ(RwReal scale_fwd, RwReal scale_bak)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::ApplyInputFwdToForceZ");
         
         CMsg CMsg(m_Msg_Inq_Forward_Action);
      
         SendMsg(CMsg);
      
         if (CMsg.pData != 0)
         {
            RwReal input = (*reinterpret_cast<RwReal*>(&CMsg.pData));
         
            if (input>0) m_force.z = (scale_fwd) * input;
            else        m_force.z = (scale_bak) * input;
         
            // If the player is pushing fwd or back, modify the pitch value so that
            // it levels out, the more the player pushes forwards the faster it will 
            // level out.
            //
            // m_pitch = m_pitch * (1.0f - 0);  // Not moving
            // m_pitch = m_pitch * (1.0f - 1.0f);  // Moving full speed forward or backwards
            //
            if (input<0) input = - input;
         
            m_pitch = LowPassFilter( (m_pitch * (1.0f - input)) , m_pitch, m_lookup_scale);
         }

         RWS_RETURNVOID();
      }
   
   
      /**
      * 
      * \ref ApplyInputStrafeToForceX. Polls the ACTN_STRAFE input and uses it to 
      * apply a left or right force to the character.
      *
      * \param m_input_strafe_scale Scaling factor for strafe movement.
      *
      * \see ApplyInputPitchToViewMatrix
      * \see ApplyInputFwdToForceZ
      * \see ApplyInputStrafeToForceX
      * \see ApplyInputTurnToAngForceY
      *
      */
      void FPSPlayer::ApplyInputStrafeToForceX(RwReal m_input_strafe_scale)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::ApplyInputStrafeToForceX");
         
         CMsg CMsg(m_Msg_Inq_Strafe_Action);
      
         SendMsg(CMsg);
      
         if (CMsg.pData != 0)
         {
            RwReal input = (*reinterpret_cast<RwReal*>(&CMsg.pData));
         
            m_force.x = (m_input_strafe_scale) * input;
         }

         RWS_RETURNVOID();
      }
   
   
      /** 
      *
      * \ref ApplyInputTurnToAngForceY. Polls the ACTN_TURN input and uses it to apply 
      * a turnings force to the character.
      *
      * \param scale Scaling factor for turn movement.
      *
      * \see ApplyInputPitchToViewMatrix
      * \see ApplyInputFwdToForceZ
      * \see ApplyInputStrafeToForceX
      * \see ApplyInputTurnToAngForceY
      */
      void FPSPlayer::ApplyInputTurnToAngForceY(RwReal scale)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::ApplyInputTurnToAngForceY");
         
         CMsg CMsg(m_Msg_Inq_Turn_Action);
      
         SendMsg(CMsg);
      
         if (CMsg.pData != 0)
         {
            RwReal input = (*reinterpret_cast<RwReal*>(&CMsg.pData));
         
            m_ang_force.y = (scale) * input;
         }

         RWS_RETURNVOID();
      }
   
   
      /**
      * 
      * \ref ApplyDampening. Adds forces due to dampening to current resultant force.
      *
      * \param dampening      Position dampening.
      * \param ang_dampening  Angular dampening.
      *
      * \see ApplyInputPitchToViewMatrix
      * \see ApplyInputFwdToForceZ
      * \see ApplyInputStrafeToForceX
      * \see ApplyInputTurnToAngForceY
      *
      */
      void FPSPlayer::ApplyDampening(RwV3d *dampening, RwV3d *ang_dampening)  
      {      
         RWS_FUNCTION("RWS::FPS::FPSPlayer::ApplyDampening");

   #if defined (RWS_DEBUGTOOLS) && defined (RWS_DESIGN)
         if (m_Flags & uATTRIBUTEHANDLER_FLAG_DEBUG)
         {
            DebugTools::Printf("dampening %f %f %f\n",
               dampening->x,
               dampening->y,
               dampening->z);
         }
   #endif
      
         m_force.x += dampening->x * m_velocity.x;
         m_force.y += dampening->y * m_velocity.y;
         m_force.z += dampening->z * m_velocity.z;
      
   #if defined (RWS_DEBUGTOOLS) && defined (RWS_DESIGN)
         if (m_Flags & uATTRIBUTEHANDLER_FLAG_DEBUG)
         {
            DebugTools::Printf("ang_dampening %f %f %f\n\n",
               ang_dampening->x,
               ang_dampening->y,
               ang_dampening->z);
         }
   #endif
      
         m_ang_force.x += ang_dampening->x * m_ang_velocity.x;
         m_ang_force.y += ang_dampening->y * m_ang_velocity.y;
         m_ang_force.z += ang_dampening->z * m_ang_velocity.z;

         RWS_RETURNVOID();
      }
   
   
      /**
      * 
      * \ref ApplyGravity. Applies force of gravity to character.
      *
      * \see ApplyDampening
      * \see ApplyGravity
      * \see ApplyNumericalIntegration
      * \see ApplyCollisionDetectionSendMsg
      * \see ApplyCollisionDetection
      *
      */
      void FPSPlayer::ApplyGravity()
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::ApplyGravity");

   #if defined (RWS_DEBUGTOOLS) && defined (RWS_DESIGN)
         if (m_Flags & uATTRIBUTEHANDLER_FLAG_DEBUG)
         {
            DebugTools::Printf("m_gravity %f\n", m_gravity);
         }
   #endif
      
         m_force.y += m_gravity;

         RWS_RETURNVOID();
      }
   
   
      /**
      * 
      * \ref ApplyNumericalIntegration. Applies numerical integration to accelerations.
      *
      * accel = force / mass
      * vel = Integral of accel
      * pos = Integral of vel
      *
      * \see ApplyDampening
      * \see ApplyGravity
      * \see ApplyNumericalIntegration
      * \see ApplyCollisionDetectionSendMsg
      * \see ApplyCollisionDetection
      *
      */
      void FPSPlayer::ApplyNumericalIntegration(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::ApplyNumericalIntegration");
         
         RwFrame *frame = RpAtomicGetFrame(m_pAtomic.ptr());
         if (frame)
         {
            m_velocity.x += m_force.x;
            m_velocity.y += m_force.y;
            m_velocity.z += m_force.z;

            RwFrameTranslate (frame, &m_velocity, rwCOMBINEPRECONCAT);
         
            m_ang_velocity.x += m_ang_force.x;
            m_ang_velocity.y += m_ang_force.y;
            m_ang_velocity.z += m_ang_force.z;
         
            RwFrameRotate (frame, &YAxis, m_ang_velocity.y, rwCOMBINEPRECONCAT);
            RwFrameRotate (frame, &ZAxis, m_ang_velocity.z, rwCOMBINEPRECONCAT);
            RwFrameRotate (frame, &XAxis, m_ang_velocity.x, rwCOMBINEPRECONCAT);
         
   #if defined (RWS_DEBUGTOOLS) && defined (RWS_DESIGN)
            if (m_Flags & uATTRIBUTEHANDLER_FLAG_DEBUG)
            {
               DebugTools::Printf("m_force    %f %f %f\n", m_force.x, m_force.y, m_force.z);
               DebugTools::Printf("m_velocity %f %f %f\n\n", m_velocity.x, m_velocity.y, m_velocity.z);
            
               DebugTools::Printf("m_ang_force    %f %f %f\n", m_ang_force.x, m_ang_force.y, m_ang_force.z);
               DebugTools::Printf("m_ang_velocity %f %f %f\n\n", m_ang_velocity.x, m_ang_velocity.y, m_ang_velocity.z);
            }
   #endif
         
            ZeroVector(&m_force);
            ZeroVector(&m_ang_force);
         }

         RWS_RETURNVOID();
      }
   
   
      /**
      * 
      * \ref ApplyCollisionDetectionSendMsg. Applies collision detection, with sending of collision 
      * message if another entity is touched.
      *
      * \see ApplyDampening
      * \see ApplyGravity
      * \see ApplyNumericalIntegration
      * \see ApplyCollisionDetectionSendMsg
      * \see ApplyCollisionDetection
      * 
      */
      void FPSPlayer::ApplyCollisionDetectionSendMsg(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::ApplyCollisionDetectionSendMsg");
         
         CMsg CMsg;
      
         CMsg.Id = m_Msg_Touch;
         CMsg.pData = static_cast<CEventHandler*>(this);
      
         ApplyCollisionDetection(&CMsg);

         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * \ref ApplyCollisionDetection. Applies collision detection,
      *
      * \param pMsg Standard Message Package
      *
      * \see ApplyDampening
      * \see ApplyGravity
      * \see ApplyNumericalIntegration
      * \see ApplyCollisionDetectionSendMsg
      * \see ApplyCollisionDetection
      *
      */
      void FPSPlayer::ApplyCollisionDetection(CMsg *CMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::ApplyCollisionDetection");
         
         RwFrame *frame = RpAtomicGetFrame(m_pAtomic.ptr());
         if (frame)
         {
            // Make a copy of the position before testing the collisions
            RwV3d posa = *RwMatrixGetPos( RwFrameGetLTM(frame) );
            RwV3d E;
         
            E.x = m_width;
            E.y = m_half_height;
            E.z = m_width;
         
            m_onground = false;  
         
            // Calculate the collision
            CCollide::CEllipsoid::CalcCollision(this,
               CMsg, frame, &m_center_offset,
               m_height_when_on_ground, &m_onground, &E);
         
            // This may have been updated by CalcCollision calling RwFrameGetLTM recalculates the absolute
            // transformation matrix LTM

            RwV3d posb = *RwMatrixGetPos( RwFrameGetLTM(frame) );
            RwV3d delta, ndelta, nvelocity;
         
            // Cancel velocity lost due to collision
            delta.x = posb.x - posa.x;
            delta.y = posb.y - posa.y;
            delta.z = posb.z - posa.z;
         
   #if defined (RWS_DEBUGTOOLS) && defined (RWS_DESIGN)
            if (m_Flags & uATTRIBUTEHANDLER_FLAG_DEBUG)
            {
               DebugTools::Printf("delta %f %f %f\n", delta.x, delta.y, delta.z);
            
               if (RwFrameGetParent(RpAtomicGetFrame(m_pAtomic.ptr())))
               {
                  DebugTools::Printf("Parent Frame TRUE\n");
               }
               else
               {
                  DebugTools::Printf("Parent Frame FALSE\n");
               }
            }
   #endif

            // check if we are in the process of teleporting
            if (!m_teleported_flag)
            {
         
               if (RwV3dLength(&delta) > 0.0f)
               {
                  RwMatrix *imatrix = RwMatrixCreate();
                  RwMatrixInvert(imatrix, RwFrameGetLTM( frame ));
                  RwV3dTransformVectors(&delta, &delta, 1, imatrix);          
                  RwMatrixDestroy(imatrix);           
                  RwV3dNormalize(&ndelta, &delta);          // Normalize delta          
                  RwV3dNormalize(&nvelocity, &m_velocity);  // Normalize original velocity          
                  RwReal dot = -1.0f * RwV3dDotProduct(&nvelocity, &ndelta);            
                  RwV3dScale(&delta, &delta, dot);
            
                  // Use elasticity to cancel velocity in direction of collision
                  m_velocity.x += delta.x;
                  m_velocity.y += delta.y;
                  m_velocity.z += delta.z;
               }
            }
               else
            {
               // as we are teleporting we don't want to adjust the velocity cause by a collision with the world
               m_teleported_flag = false;
            }
            RwFrameUpdateObjects(frame);
         }

         RWS_RETURNVOID();
      }
   
   
      /**
      * 
      * \ref State_OnGround_Proc_MsgRunningTick. Process MsgRunningTick while in state State_OnGround
      *
      * \see State_OnGround_Proc_MsgRunningTick
      * \see State_Jump_Proc_MsgRunningTick 
      * \see State_Falling_Proc_MsgRunningTick
      * \see State_Landing_Proc_MsgRunningTick
      *
      */
      void FPSPlayer::State_OnGround_Proc_MsgRunningTick(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::State_OnGround_Proc_MsgRunningTick");


         // Apply Fwd input to Z force, i.e. move forwards and backwards
         ApplyInputFwdToForceZ(m_input_fwd_scale, m_input_bak_scale);
      
         // Apply Strafe input to X force
         ApplyInputStrafeToForceX(m_input_strafe_scale);
      
         // Apply Pitch
         ApplyInputPitchToViewMatrix(m_pitch_scale);
      
         // Turn
         ApplyInputTurnToAngForceY(m_input_turn_scale);
      
         // Apply forces
         ApplyDampening(&m_dampening_on_ground, &m_ang_dampening_on_ground);
         ApplyGravity();
         ApplyNumericalIntegration();
         ApplyCollisionDetectionSendMsg();
      
         if (!m_onground)
         {
            Set_m_state(State_Falling);
         }
         else
         {
            // Poll Jump Action
            CMsg InqMsg(m_Msg_Inq_Jump_Action);
         
            SendMsg(InqMsg);
         
            if (InqMsg.pData != 0)
            {
               RwReal input = *reinterpret_cast<RwReal*>(&InqMsg.pData);
            
               if (input>0.0f)
               {
                  m_force.y = input * m_jump_force_1;
                  m_jumping_time = 0;
                  Set_m_state (State_Jump);
               }
            }
         }

         RWS_RETURNVOID();
      }
   
   
      /**
      * 
      * \ref State_Jump_Proc_MsgRunningTick. Process Tick message when in State_Jump
      *
      * \see State_OnGround_Proc_MsgRunningTick
      * \see State_Jump_Proc_MsgRunningTick 
      * \see State_Falling_Proc_MsgRunningTick
      * \see State_Landing_Proc_MsgRunningTick
      *
      */
      void FPSPlayer::State_Jump_Proc_MsgRunningTick(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::State_Jump_Proc_MsgRunningTick");
         
         m_jumping_time += 1;
      
         if (m_jumping_time<m_jump2_force_time_limit)
         {
            CMsg CMsg(m_Msg_Inq_Jump_Action);
         
            SendMsg(CMsg);
         
            if (CMsg.pData != 0)
            {
               RwReal input = *reinterpret_cast<RwReal*>(&CMsg.pData);
            
               if (input>0.0f)
               {
                  m_force.y += input * m_jump_force_2;         // Jump higher
               }
               else
               {
                  m_jumping_time = m_jump2_force_time_limit;   // Once you let go thats it
               }
            }
         }
      
         // Apply Fwd input to Z force, i.e. move forwards and backwards
         ApplyInputFwdToForceZ(m_input_fwd_scale_in_air, m_input_bak_scale_in_air);
      
         // Apply Strafe input to X force
         ApplyInputStrafeToForceX(m_input_strafe_scale_in_air);
      
         // Apply Pitch
         ApplyInputPitchToViewMatrix(m_pitch_scale);
      
         // Turn
         ApplyInputTurnToAngForceY(m_input_turn_scale_in_air);
      
         // Apply forces
         ApplyDampening(&m_dampening_in_air, &m_ang_dampening_in_air);
         ApplyGravity();
         ApplyNumericalIntegration();
         ApplyCollisionDetectionSendMsg();
      
         if (m_onground) 
         {
            // Can only return to standing if moving down, otherwise bumping
            // into things could allow you to re-jump
            if (m_velocity.y < 0)  
            {
               Set_m_state (State_Landing);
            }
         }

         RWS_RETURNVOID();
      }   
   
   
      /**
      * 
      * \ref Process Tick message when in State_Falling
      *
      * \see State_OnGround_Proc_MsgRunningTick
      * \see State_Jump_Proc_MsgRunningTick 
      * \see State_Falling_Proc_MsgRunningTick
      * \see State_Landing_Proc_MsgRunningTick
      *
      */ 
      void FPSPlayer::State_Falling_Proc_MsgRunningTick(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::State_Falling_Proc_MsgRunningTick");
         
         // Apply Fwd input to Z force, i.e. move forwards and backwards
         ApplyInputFwdToForceZ(m_input_fwd_scale_in_air, m_input_bak_scale_in_air);
      
         // Apply Strafe input to X force
         ApplyInputStrafeToForceX(m_input_strafe_scale_in_air);
      
         // Apply Pitch
         ApplyInputPitchToViewMatrix(m_pitch_scale);
      
         // Turn
         ApplyInputTurnToAngForceY(m_input_turn_scale_in_air);
      
         // Apply forces
         ApplyDampening(&m_dampening_in_air, &m_ang_dampening_in_air);
         ApplyGravity();
         ApplyNumericalIntegration();
         ApplyCollisionDetectionSendMsg();
      
         if (m_onground) 
         {
            // Can only return to standing if moving down, otherwise bumping
            // into things could allow you to re-jump
            if (m_velocity.y < 0)
            {
               Set_m_state (State_Landing);      
            }
         }

         RWS_RETURNVOID();
      }
   
   
      /**
      * 
      * \ref Process MsgRunningTick when in Landing state
      *
      * \see State_OnGround_Proc_MsgRunningTick
      * \see State_Jump_Proc_MsgRunningTick 
      * \see State_Falling_Proc_MsgRunningTick
      * \see State_Landing_Proc_MsgRunningTick
      *
      */
      void FPSPlayer::State_Landing_Proc_MsgRunningTick(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::State_Landing_Proc_MsgRunningTick");
         
         // Apply Fwd input to Z force, i.e. move forwards and backwards
         ApplyInputFwdToForceZ(m_input_fwd_scale, m_input_bak_scale);
      
         // Apply Strafe input to X force
         ApplyInputStrafeToForceX(m_input_strafe_scale);
      
         // Apply Pitch
         ApplyInputPitchToViewMatrix(m_pitch_scale);
      
         // Turn
         ApplyInputTurnToAngForceY(m_input_turn_scale);
         // Apply forces
         ApplyDampening(&m_dampening_on_ground, &m_ang_dampening_on_ground);
         ApplyGravity();
         ApplyNumericalIntegration();
         ApplyCollisionDetectionSendMsg();
      
         if (m_onground)
         {
            Set_m_state (State_OnGround);
         }

         RWS_RETURNVOID();
      } 
   
   
      /**
      * 
      * \ref DeletePickups. Remove specified pickup object
      *
      * \param pMsg Standard Message Package
      *
      */
      void FPSPlayer::DeletePickup(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::DeletePickup");
         
         CEventHandler *pCEventHandler = static_cast<CEventHandler*>(pMsg.pData);
      
         if (pCEventHandler)
         {
            pCEventHandler->Delete();
         }

         RWS_RETURNVOID();
      }
   
   
      /**
      * 
      * \ref Set_m_state. Change main state
      *
      * \param new_m_state A new state value.
      *
      * \verbatim
               enum States
               {
                   State_OnGround = 0,
                   State_Jump,
                   State_Falling,   // Initial State
                   State_Landing,
                   State_Last_State
               };
        \endverbatim
      *
      */ 
      void FPSPlayer::Set_m_state(RwUInt32 new_m_state) 
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::Set_m_state");
         
         m_state = new_m_state;

         RWS_RETURNVOID();
      }
   
   
      /**
      * 
      * \ref ZeroVector. Zero a vector.
      *
      * \param v Vector to be zero'd.
      *
      */ 
      void FPSPlayer::ZeroVector(RwV3d *v)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::ZeroVector");
         
         v->x = 0.0f;
         v->y = 0.0f;
         v->z = 0.0f;

         RWS_RETURNVOID();
      }
   
   
      /**
      *
      * \ref HandleAttributes. Handle attribute updates.
      *
      * \param Attr Reference to a CAttributePacket.
      *
      */   
      void FPSPlayer::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::HandleAttributes");
      
         m_pAtomic.HandleSystemCommands(attr);
      
         // Initialize physics attributes
         ZeroVector(&m_force);
         ZeroVector(&m_velocity);
      
         ZeroVector(&m_ang_force);
         ZeroVector(&m_ang_velocity);
      
         // Make sure the player isn't linked to anything...
         RwFrame *frame = RpAtomicGetFrame(m_pAtomic.ptr());
         if (frame)
         {
            if (RwFrameGetParent(frame))
            {
               RwMatrixCopy(RwFrameGetMatrix(frame), RwFrameGetLTM(frame));
               RwFrameRemoveChild(frame);
            }
         }
      
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(FPSPlayer));
      
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_Set_m_Tx_Pos:
               ReplaceRegisteredMsg(m_Tx_Pos, attrIt->GetAs_RwChar_ptr(), "RwFrame*");
               break;

            case CMD_cameraheight:
               attrIt->GetCommandData(m_cameraheight);
            
               RWS_TRACE("CMD_cameraheight "<<m_cameraheight);
               break;
            
            case CMD_m_pitch_scale:
               attrIt->GetCommandData(m_pitch_scale);
            
               RWS_TRACE("CMD_m_pitch_scale "<<m_pitch_scale);
               break;
            
            case CMD_m_lookup_scale:
               attrIt->GetCommandData(m_lookup_scale);
            
               RWS_TRACE("CMD_m_lookup_scale "<<m_lookup_scale);
               break;
            
            case CMD_Set_m_height:
               attrIt->GetCommandData(m_half_height);
            
               RWS_TRACE("CMD_Set_m_height "<< m_half_height);
               break;
            
            case CMD_Set_m_height_when_on_ground:
               attrIt->GetCommandData(m_height_when_on_ground);
            
               RWS_TRACE("CMD_Set_m_height_when_on_ground "<<m_height_when_on_ground);
               break;
            
            case CMD_Set_m_width:
               attrIt->GetCommandData(m_width);
            
               RWS_TRACE("CMD_Set_m_width "<<m_width);
               break;
            
            case CMD_Set_m_input_fwd_scale:
               attrIt->GetCommandData(m_input_fwd_scale);
            
               RWS_TRACE("CMD_Set_m_input_fwd_scale "<<m_input_fwd_scale);
               break;
            
            case CMD_Set_m_input_bak_scale:
               attrIt->GetCommandData(m_input_bak_scale);
            
               RWS_TRACE("CMD_Set_m_input_bak_scale "<<m_input_bak_scale);
               break;
            
            case CMD_Set_m_input_fwd_scale_in_air:
               attrIt->GetCommandData(m_input_fwd_scale_in_air);
            
               RWS_TRACE("CMD_Set_m_input_fwd_scale_in_air "<<m_input_fwd_scale_in_air);
               break;
            
            case CMD_Set_m_input_bak_scale_in_air:
               attrIt->GetCommandData(m_input_bak_scale_in_air);
            
               RWS_TRACE("CMD_Set_m_input_bak_scale_in_air "<<m_input_bak_scale_in_air);
               break;
            
            case CMD_Set_m_dampening_on_ground:
               attrIt->GetCommandData(m_dampening_on_ground);
            
               RWS_TRACE("CMD_Set_m_dampening_on_ground "<<m_dampening_on_ground.x<<
                  " "<<m_dampening_on_ground.y<<" "<<m_dampening_on_ground.z);
               break;
            
            case CMD_Set_m_dampening_in_air:
               attrIt->GetCommandData(m_dampening_in_air);
            
               RWS_TRACE("CMD_Set_m_dampening_in_air "<<m_dampening_in_air.x<<" "<<
                  m_dampening_in_air.y<<" "<<m_dampening_in_air.z);
               break;
            
            case CMD_Set_m_ang_dampening_on_ground:
               attrIt->GetCommandData(m_ang_dampening_on_ground);
            
               RWS_TRACE("CMD_Set_m_ang_dampening_on_ground "<<m_ang_dampening_on_ground.x<<
                  " "<<m_ang_dampening_on_ground.y<<" "<<m_ang_dampening_on_ground.z);
               break;
            
            case CMD_Set_m_ang_dampening_in_air:
               attrIt->GetCommandData(m_ang_dampening_in_air);
            
               RWS_TRACE("CMD_Set_m_ang_dampening_in_air "<<m_ang_dampening_in_air.x<<" "<<
                  m_ang_dampening_in_air.y<<" "<<m_ang_dampening_in_air.z);
               break;
         
            case CMD_Set_m_input_turn_scale:
               attrIt->GetCommandData(m_input_turn_scale);
            
               RWS_TRACE("CMD_Set_m_input_turn_scale "<<m_input_turn_scale);
               break;
            
            case CMD_Set_m_input_turn_scale_in_air:
               attrIt->GetCommandData(m_input_turn_scale_in_air);
            
               RWS_TRACE("CMD_Set_m_input_turn_scale_in_air "<<m_input_turn_scale_in_air);
               break;
            
            case CMD_Set_m_jump2_force_time_limit:
               attrIt->GetCommandData(m_jump2_force_time_limit);
            
               RWS_TRACE("CMD_Set_m_jump2_force_time_limit "<<m_jump2_force_time_limit);
               break;
            
            case CMD_Set_m_jump_force_1:
               attrIt->GetCommandData(m_jump_force_1);
            
               RWS_TRACE("CMD_Set_m_jump_force_1 "<<m_jump_force_1);
               break;
            
            case CMD_Set_m_jump_force_2:
               attrIt->GetCommandData(m_jump_force_2);
            
               RWS_TRACE("CMD_Set_m_jump_force_2 "<<m_jump_force_2);
               break;
            
            case CMD_Set_m_gravity:
               attrIt->GetCommandData(m_gravity);
            
               RWS_TRACE("CMD_Set_m_gravity "<<m_gravity);
               break;
            
            case CMD_m_input_strafe_scale:
               attrIt->GetCommandData(m_input_strafe_scale);
            
               RWS_TRACE("CMD_m_input_strafe_scale "<<m_input_strafe_scale);
               break;
            
            case CMD_m_input_strafe_scale_in_air:
               attrIt->GetCommandData(m_input_strafe_scale_in_air);
            
               RWS_TRACE("CMD_m_input_strafe_scale_in_air "<<m_input_strafe_scale_in_air);
               break;
            
            case CMD_Set_m_center_offset:
               attrIt->GetCommandData(m_center_offset);
            
               RWS_TRACE("CMD_Set_m_center_offset "<<m_center_offset.x<<" "<<m_center_offset.y<<
                  " "<<m_center_offset.z);
               break;
            
            case CMD_Set_View_Angle:
               {
                  RwReal angle;
               
                  attrIt->GetCommandData(angle);
               
                  m_ViewWindow = static_cast<RwReal>(RwTan(RWDEG2RAD(angle * 0.5f)));
               
                  RWS_TRACE("CMD_Set_View_Angle"<<m_ViewWindow);
               }
               break;
            
   #ifdef RWS_DESIGN
            case CMD_Set_m_respondtocamerarequest:
               attrIt->GetCommandData(m_respondtocamerarequest);
            
               RWS_TRACE("CMD_Set_m_respondtocamerarequest"<<m_respondtocamerarequest);
               break;
   #endif

               // Set incoming render message.

            case CMD_Set_m_InRender:

               UnLinkMsg(m_Render_In);
               UnRegisterMsg(m_Render_In);
               RegisterMsg(m_Render_In, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
               LinkMsg(m_Render_In, "RwCamera*", m_renderInPriority);
               break;

               // Set priority of the rendering message.

            case CMD_priority:

               attrIt->GetCommandData(m_renderInPriority);
               UnLinkMsg(m_Render_In);
               LinkMsg(m_Render_In, "RwCamera*", m_renderInPriority);
               break;

               // Set outgoing render message.

            case CMD_Set_m_OutRender:

               UnRegisterMsg(m_Render_Out);
               RegisterMsg(m_Render_Out, attrIt->GetAs_RwChar_ptr(), "RwCamera*");
               break;

            case MSG_AreaTriggerEvent:
               
               UnLinkMsg(m_Msg_AreaTriggerEvent);
               UnRegisterMsg(m_Msg_AreaTriggerEvent);
               RegisterMsg(m_Msg_AreaTriggerEvent, attrIt->GetAs_RwChar_ptr(), 0);
               LinkMsg(m_Msg_AreaTriggerEvent);
               break;

            case MSG_AreaTriggerQuery:
               
               UnRegisterMsg(m_Msg_AreaTriggerQuery);
               RegisterMsg(m_Msg_AreaTriggerQuery, attrIt->GetAs_RwChar_ptr(), "ATCollisionQuery*");
               break;    
            }
            ++attrIt;
         }
         RWS_RETURNVOID();
      }
   
   
      /**
      * 
      * \ref FPSPlayer. Initialize FPSPlayer
      *
      * \see ~FPSPlayer.
      *
      */
      FPSPlayer::FPSPlayer(const CAttributePacket& rAttr) :
         InitCEventHandler(&m_pAtomic),
         m_cameraheight(24.0f), 
         m_pitch_scale(1.0f),
         m_pitch(0.0f),
         m_lookup_scale(0.1f),
         m_gravity(-0.1f),
         m_half_height(25.5f),
         m_height_when_on_ground(23.5f),
         m_width(20.0f),   
         m_input_strafe_scale(1),
         m_input_strafe_scale_in_air(0),
         m_input_fwd_scale(2.0f),
         m_input_bak_scale(1.75f),
         m_input_fwd_scale_in_air(1.0f),
         m_input_bak_scale_in_air(0.75f),    
         m_input_turn_scale(2.0f),
         m_input_turn_scale_in_air(2.0f),
         m_jump_force_1(0.75f),
         m_jump_force_2(0.5f),
         m_jump2_force_time_limit(7),
         m_state(State_OnGround),
         m_teleported_flag(false)
   #ifdef RWS_DESIGN
         , m_respondtocamerarequest(true)
   #endif
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::FPSPlayer");
         
         // Can't use initializer ('this' isn't valid) so...
         m_pAtomic = CreateAtomicInWorldFromResource(rAttr, this);
      
         RWS_ASSERT(m_pAtomic, "Failed to create atomic");
      
         // Initial physics attributes
         ZeroVector(&m_force);
         ZeroVector(&m_velocity);
      
         ZeroVector(&m_ang_force);
         ZeroVector(&m_ang_velocity);
      
         ZeroVector(&m_center_offset);
      
         // Init linear dampening 
         m_dampening_on_ground.x = -0.5f;
         m_dampening_on_ground.y = -0.0f;
         m_dampening_on_ground.z = -0.5f;
      
         m_dampening_in_air.x = -0.5f;
         m_dampening_in_air.y = -0.0f;
         m_dampening_in_air.z = -0.5f;
      
         // Init angular dampening
         m_ang_dampening_on_ground.x = -0.5f;
         m_ang_dampening_on_ground.y = -0.5f;
         m_ang_dampening_on_ground.z = -0.5f;
      
         m_ang_dampening_in_air.x = -0.5f;
         m_ang_dampening_in_air.y = -0.5f;
         m_ang_dampening_in_air.z = -0.5f;
      
         m_ViewWindow = static_cast<RwReal>(RwTan(RWDEG2RAD(90.0f * 0.5f)));
      
         // Register the events that the player character responds to
      
         // Player has collected...
         RegisterMsg ( m_Event_FPS_Example_Pickup, "FPS_Example_Pickup","CEventHandler*");
      
      
         // Player has been teleported
         RegisterMsg ( m_Event_ACTN_TELEPORT, "ACTN_TELEPORT", "RwFrame*");
      
         // Player has been pushed
         RegisterMsg ( m_Event_FPS_trigger_push, "FPS_trigger_push", "RwV3d*");
      
         // Controller Input Request Events
         RegisterMsg(m_Msg_Inq_Forward_Action, "INQ_ACTN_FWD", "return RwReal");
         RegisterMsg(m_Msg_Inq_Turn_Action, "INQ_ACTN_TURN", "return RwReal");
         RegisterMsg(m_Msg_Inq_Jump_Action, "INQ_ACTN_JUMP", "return RwReal");
         RegisterMsg(m_Msg_Inq_Strafe_Action, "INQ_ACTN_STRAFE", "return RwReal");
         RegisterMsg(m_Msg_Inq_Camera_Pitch, "INQ_CAMERA_PITCH", "return RwReal");
         RegisterMsg(m_Msg_Inq_Fire_Action, "INQ_ACTN_FIRE", "return RwReal");
      
         // Inquire the position of the player 
         RegisterMsg(m_Event_INQ_POSITION, "Event_INQ_PLYR_POSITION", "return RwMatrix*");
         LinkMsg(m_Event_INQ_POSITION, "return RwMatrix*");
      
         // Inquire the 3D Entity of the player
         RegisterMsg(m_Event_INQ_RpAtomic, "Event_INQ_PLYR_RpAtomic", "return RpAtomic*");
         LinkMsg(m_Event_INQ_RpAtomic, "return RpAtomic*");
      
         RegisterMsg(m_Msg_Touch, "ACTN_PLAYERTOUCH", "CEventHandler*");
         RegisterMsg(m_Msg_Shot, "ACTN_PLAYERSHOOT", "CEventHandler*");
      
         LinkMsg(iMsgRunningTick);

         m_cameraFrame = RwFrameCreate();
   #ifdef RWS_DESIGN
         LinkMsg(iMsgRequestCurrentCameraMatrix, "RwUInt32 InstanceID");
   #endif      

         LinkMsg(ClumpHelper::iMsgPreRpClumpDestroy, "RwFrame*");
      
         m_ViewingMatrix = RwMatrixCreate();

         RWS_RETURNVOID();
      }
   
   
      /**
      * 
      * \ref ~FPSPlayer. Deconstruct FPSPlayer
      *
      * \see FPSPlayer.
      *
      */
      FPSPlayer::~FPSPlayer(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlayer::~FPSPlayer");


         UnLinkMsg(m_Render_In);

         UnRegisterMsg (m_Event_FPS_Example_Pickup);
      
         UnRegisterMsg ( m_Event_ACTN_TELEPORT); 
         UnRegisterMsg ( m_Event_FPS_trigger_push);
      
         UnRegisterMsg( m_Msg_Inq_Forward_Action );
         UnRegisterMsg( m_Msg_Inq_Turn_Action );
         UnRegisterMsg( m_Msg_Inq_Jump_Action );
         UnRegisterMsg( m_Msg_Inq_Strafe_Action );
         UnRegisterMsg( m_Msg_Inq_Camera_Pitch );
         UnRegisterMsg( m_Msg_Inq_Fire_Action );
      
         UnRegisterMsg(m_Render_In);
         UnRegisterMsg(m_Render_Out);

         UnLinkMsg( m_Event_INQ_POSITION );
         UnRegisterMsg ( m_Event_INQ_POSITION );
      
         UnLinkMsg( m_Event_INQ_RpAtomic );
         UnRegisterMsg ( m_Event_INQ_RpAtomic );
      
         UnRegisterMsg(m_Msg_Touch);
         UnRegisterMsg(m_Msg_Shot);

         UnRegisterMsg(m_Tx_Pos);
      
         UnLinkMsg(iMsgRunningTick);

         RwFrameDestroy(m_cameraFrame);
   #ifdef RWS_DESIGN
         UnLinkMsg(iMsgRequestCurrentCameraMatrix);
   #endif

         UnLinkMsg(ClumpHelper::iMsgPreRpClumpDestroy);

         RwMatrixDestroy(m_ViewingMatrix);

         UnLinkMsg(m_Msg_AreaTriggerEvent);
         UnRegisterMsg(m_Msg_AreaTriggerEvent);
         UnRegisterMsg(m_Msg_AreaTriggerQuery);

         RWS_RETURNVOID();
      }
   }
}
