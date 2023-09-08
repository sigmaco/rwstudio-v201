/*****************************************************************************
*
* File :   FPSPlayer.h
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

#ifndef __FPS_PLAYER_H__
#define __FPS_PLAYER_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/toolkits/collision/collide.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "modules/generic/csubrastercamera/csubrastercamera.h"
#include "modules/areatriggers/atbase.h"
#include "framework/core/attributehandler/allocation policies/linearallocationpolicy.h"

namespace RWS
{
   namespace FPS
   {
      using namespace Generic;
      /**
      *
      * \ingroup Mod_FPS
      *
      * Implementation of a First Person Shooter Player. This player behavior lets you run, jump, strafe etc,
      * the controls are linked together by using the messaging system and separate behaviors for each platforms
      * controller pads.  The state for the player is controlled from the HandleEvents function.  This function
      * also handles all the messaging for when a player fires it weapon.  
      *
      * When this happens a ray is fired into the scene and returns any atomic collided with along with the
      * intersected polygon.  If the ray collides with the BSP then the polygon is also returned.
      *
      * Collision detection for the player is implemented by placing an ellipse around the player,  then an
      * ellipse collision test is performed with the bsp and other objects in the world.
      *
      * The player also supports teleporting,  if the player collides with a teleporter object,  it will then
      * be moved to a new position, again this is all handled via the messaging system.
      * 
      */
      class FPSPlayer : public CSystemCommands, public CAttributeHandler , public CEventHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(FPSPlayer);
         RWS_DECLARE_CLASSID(FPSPlayer);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Generic First Person Player", "A generic first person player.");
         
         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);
         
         RWS_BEGIN_COMMANDS
            RWS_SEPARATOR("Messages", 1)
            
            RWS_MESSAGE( CMD_Set_m_Tx_Pos,
            "Send Player Position",
            "Send Position Of Player",
            TRANSMIT,
            RwFrame*,
            0)
            
            RWS_MESSAGE(CMD_Set_m_InRender,
            "Render msg (input)",
            "Incoming rendering message, notes camera settings in data",
            RECEIVE,
            RwCamera*,
            0)
            
            RWS_ATTRIBUTE(CMD_priority, "Render Priority", "Specify the render priority",
            SLIDER, RwUInt32, RANGE(0,65535,65535))
            
            RWS_MESSAGE(CMD_Set_m_OutRender,
            "Render msg (output)",
            "Outgoing rendering message, same as world / incoming, but with updated position and view",
            TRANSMIT,
            RwCamera*,
            0)
            
            RWS_SEPARATOR("Camera Attributes",  0)

            RWS_ATTRIBUTE(CMD_cameraheight,
               "Camera Height",
               "Height of camera relative to objects origin.",
               SLIDER, RwReal, RANGE(0, 24, 99))

            RWS_ATTRIBUTE(CMD_m_pitch_scale,
               "Camera Pitch Input Scale",
               "Pitch input multiplier,  increasing this value increases the effect of the pitch control.",
               SLIDER, RwReal, RANGE(0, 1, 5))

            RWS_ATTRIBUTE(CMD_m_lookup_scale,
               "Camera Look Up Scale",
               "Controls the rate that the camera auto levels,  pushing fwd/back auto levels the camera.",
               SLIDER, RwReal, RANGE(0, 0.1, 1))

            RWS_ATTRIBUTE(CMD_Set_View_Angle,
               "Field of view",
               "View cone angle",
               SLIDER,   RwReal,   RANGE(1, 90, 180))
            
            RWS_SEPARATOR("Collision Attributes",  0)

            RWS_ATTRIBUTE(CMD_Set_m_height,
               "Collision height",
               "Height of collision ellipse Y axis.",
               SLIDER,   RwReal,   RANGE(0,  25.5,  250))

            RWS_ATTRIBUTE(CMD_Set_m_height_when_on_ground,
               "Collision on ground flag",
               "Collision below this point are considered ground,  when character is in contact with"
               " ground then it is allowed to jump.",
               SLIDER,   RwReal,   RANGE(0,  23.5,  250))

            RWS_ATTRIBUTE(CMD_Set_m_width,
               "Collision width",
               "Diameter of collision ellipse XZ axis",
               SLIDER,   RwReal,   RANGE(0,  20,  250))
            
            RWS_ATTRIBUTE(CMD_Set_m_center_offset,
               "Offset for center of collision",
               "Vector used to offset the collision center. Enables collision primitive to be moved"
               "relative to geometry center.",
               VECTOR,   RwV3d,    RANGES((-50,  0,  50), (-50,  0,  50), (-50,  0,  50)))

            RWS_SEPARATOR("Strafe Movement Attributes",  0)               
            RWS_ATTRIBUTE(CMD_m_input_strafe_scale,
               "Strafe action speed on ground",  "Multiplier,  determines speed of strafe action while on ground.",
               SLIDER,   RwReal,   RANGE(0,  1,  8))

            RWS_ATTRIBUTE(CMD_m_input_strafe_scale_in_air,
               "Strafe action speed in air",
               "Multiplier,  determines speed of strafe action while in the air.",
               SLIDER,   RwReal,   RANGE(0,  0,  8))
            
            RWS_SEPARATOR("Forward Movement Attributes",  0)
            RWS_ATTRIBUTE(CMD_Set_m_input_fwd_scale,
               "Fwd action speed on ground",
               "Multiplier,  determines speed of forward action while on ground.",
               SLIDER,   RwReal,   RANGE(0,  2,  8))

            RWS_ATTRIBUTE(CMD_Set_m_input_bak_scale,
               "Back action speed on ground",
               "Multiplier,  determines speed of backwards action while on ground.",
               SLIDER,   RwReal,   RANGE(0,  1.75,  8))

            RWS_ATTRIBUTE(CMD_Set_m_input_fwd_scale_in_air,
               "Fwd action speed in air",     "Multiplier,  determines speed of forward action while in the air.",
               SLIDER,   RwReal,   RANGE(0,  1,  4))

            RWS_ATTRIBUTE(CMD_Set_m_input_bak_scale_in_air,
               "Back action speed in air",    "Multiplier,  determines speed of backwards action while in the air.",
               SLIDER,   RwReal,   RANGE(0,  0.75,  4))
            
            RWS_ATTRIBUTE(CMD_Set_m_dampening_on_ground,
               "Dampening while on ground",
               "0 implies no dampening,  -1 implies full dampening",
               VECTOR,   RwV3d,    RANGES((0,  -0.5,  -1), (0,  -0.0,  -1), (0,  -0.5,  -1)))

            RWS_ATTRIBUTE(CMD_Set_m_dampening_in_air,
               "Dampening while in air",
               "0 implies no dampening,  -1 implies full dampening",
               VECTOR,   RwV3d,    RANGES((0,  -0.5,  -1), (0,  -0.0,  -1), (0,  -0.5,  -1)))
            
            RWS_SEPARATOR("Turn Movement Attributes",  0)
            
            RWS_ATTRIBUTE(CMD_Set_m_input_turn_scale,
               "Turn scale on ground",
               "Multiplier,  determines rate of turn action while on ground.",
               SLIDER,   RwReal,   RANGE(0,  2.0,  4))

            RWS_ATTRIBUTE(CMD_Set_m_input_turn_scale_in_air,
               "Turn scale in air",
               "Multiplier,  determines rate of turn  of forward action while in the air.",
               SLIDER,   RwReal,   RANGE(0,  2.0,  4))
            
            RWS_ATTRIBUTE(CMD_Set_m_ang_dampening_on_ground,
               "Angular Dampening while on ground",
               "0 implies no dampening,  -1 implies full dampening",
               VECTOR,   RwV3d,    RANGES((0,  -0.5,  -1), (0,  -0.5,  -1), (0,  -0.5,  -1)))

            RWS_ATTRIBUTE(CMD_Set_m_ang_dampening_in_air,
               "Angular Dampening while in air",
               "0 implies no dampening,  -1 implies full dampening",
               VECTOR,   RwV3d,    RANGES((0,  -0.5,  -1), (0,  -0.5,  -1), (0,  -0.5,  -1)))
            
            RWS_SEPARATOR("Jump Movement Attributes",  0)
            
            RWS_ATTRIBUTE(CMD_Set_m_jump2_force_time_limit,
               "Jump Boost,  time limit in frames",
               "Time limit that jump boost is applied.",
               SLIDER,   RwUInt32, RANGE(0,  7,  60))

            RWS_ATTRIBUTE(CMD_Set_m_jump_force_1,
               "Jump impulse",
               "Force applied when jump starts.",
               SLIDER,   RwReal,   RANGE(0,  0.75,  5))

            RWS_ATTRIBUTE(CMD_Set_m_jump_force_2,
               "Jump boost force",
               "Force applied until jump button released or jump boost time expired.",
               SLIDER,   RwReal,   RANGE(0,  0.5,  5))

            RWS_ATTRIBUTE(CMD_Set_m_gravity,
               "Gravity",
               "Force applied as gravity.",
               SLIDER,   RwReal,   RANGE(-1, -0.1,  1))
            
            RWS_SEPARATOR("Workspace Sync Camera Request",  1)
            
            RWS_ATTRIBUTE(CMD_Set_m_respondtocamerarequest,
               "Enable Respond to Workspace Sync Camera Request",
               "If enabled,  when a camera sync is performed in the workspace, "
               "this behavior returns its camera.",
               BOOLEAN,   RwUInt32, DEFAULT(1))
               
            RWS_SEPARATOR("Area Triggers", 0)

            RWS_MESSAGE(MSG_AreaTriggerEvent, 
               "Trigger Event", 
               "Set the event on which the area trigger query is performed.", 
               RECEIVE, 0, 0)

            RWS_MESSAGE(MSG_AreaTriggerQuery, 
               "Trigger Query", 
               "", 
               TRANSMIT, ATCollisionQuery*, 0)
               
            RWS_END_COMMANDS;
         
         
         // Initialization
         FPSPlayer(const CAttributePacket&);
         
         // Deletion
         virtual ~FPSPlayer(void);
         
      protected:
         
         // Delete a pickup from the scene
         void DeletePickup(CMsg &pMsg);
         
         RwReal m_cameraheight;                         /**< Position of camera relative to center of object */
         RwMatrix *m_ViewingMatrix;                     /**< Player position matrix is copied into the camera
                                                             matrix and then the viewing matrix applied.*/
         RwReal m_pitch_scale;                          /**< Scale of Input +-1 to pitch mofifier */
         RwReal m_pitch;                                /**< Current Pitch of viewing matrix */
         RwReal m_lookup_scale;                         /**< dampening for look up effect, when the player pushes
                                                             forward if the camera is pitched down it levels out */
         RwReal m_ViewWindow;                           /**< Camera view angle */
         
         CEventId m_Event_ACTN_TELEPORT;                /**< Player has been teleported */
         
         // Player has collected an item, example see FPS_item_armor1.h 
         //
         CEventId m_Event_FPS_Example_Pickup;
         
         CEventId m_Event_FPS_trigger_push;              /**< Push Event */ 
         CEventId m_Event_INQ_POSITION;                  /**< Inquire position of player character */
         CEventId m_Event_INQ_RpAtomic;                  /**< Inquire RpAtomic of player */
         
         CEventId m_Render_In;                           /**< Revieve Render Event.*/
         RwUInt16 m_renderInPriority;                    /**< Input render message's priority value, 0 = last. */
         CEventId m_Render_Out;                          /**< Send Render Event.*/
         
         CAtomicPtr m_pAtomic;                          /**< 3D Representation of player */
         
         // Player Physics            
         RwV3d m_velocity;                              /**< Velocity */
         RwV3d m_force;                                 /**< Resultant force */
         RwV3d m_ang_velocity;                          /**< Angular Velocity */
         RwV3d m_ang_force;                             /**< Angular Resultant force */
         RwReal m_gravity;                              /**< Gravity accel */
         RwBool m_onground;                             /**< Character standing on the ground flag, character can't
                                                               jump unless this is set */
         
         RwV3d m_center_offset;                         /**< Center of collision primitive */
         
         // Collision Attributes
         RwReal m_half_height;                          /**< Half height of character, i.e. position of origin above
                                                               ground level */

         RwReal m_height_when_on_ground;                /**< Height of collision ellipse which set the onground flag,
                                                               need to look at the ellipse collision code for a full
                                                               explanantion.*/

         RwReal m_width;                                /**< Width of character */
         
         // Strafe Movement Attributes
         RwReal m_input_strafe_scale;                   /**< How much to scale the strafe input by to produce the
                                                               associated force, while on the ground */

         RwReal m_input_strafe_scale_in_air;            /**< How much to scale the strafe input by to produce the
                                                               associated force, while in the air */
         
         // Forward Movement Attributes
         RwReal m_input_fwd_scale;                      /**< How much to scale the fwd input by to produce the
                                                            associated force, while on the ground */

         RwReal m_input_bak_scale;                      /**< How much to scale the bak input by to produce the
                                                            associated force, while on the ground */

         RwReal m_input_fwd_scale_in_air;               /**< How much to scale the fwd input by to produce the
                                                            associated force, while in the air */

         RwReal m_input_bak_scale_in_air;               /**< How much to scale the bak input by to produce the
                                                            associated force, while in the air */

         RwV3d m_dampening_on_ground;                     /**< dampening while on the ground, i.e. friction */
         RwV3d m_dampening_in_air;                        /**< Dampening while in the air, i.e. friction */
         
         // Turn Movement Attributes
         RwReal m_input_turn_scale;                     /**< Scale turn input to produce a turning force,
                                                            while on ground */

         RwReal m_input_turn_scale_in_air;              /**< Scale turn input to produce a turning force,
                                                            while in the air */

         RwV3d m_ang_dampening_on_ground;                 /**< dampening while on the ground, i.e. friction */
         RwV3d m_ang_dampening_in_air;                    /**< Dampening while in the air, i.e. friction */
         
         // Jump Movement Attributes
         RwReal m_jump_force_1;                         /**< Instant jump accel */
         RwReal m_jump_force_2;                         /**< Modify jump accel */
         
         RwUInt32 m_jump2_force_time_limit;             /**< Jumping consists of 2 actions, m_jump_force_1 is
                                                             applied instantly, m_jump_force_2 is applied until
                                                             m_jump2_force_time_limit reaches 0 or the player
                                                             m_jump_force_2 is applied until m_jump2_force_time_limit
                                                             reaches 0 or the player  stops presssing the jump button.
                                                             */
         
         RwUInt32 m_jumping_time;
         
         enum States
         {
            State_OnGround = 0,
               State_Jump,
               State_Falling,    // Initial State
               State_Landing,
               State_Last_State
         };
         
         RwUInt32 m_state;                              /**< Current player character state */
         
         void State_OnGround_Proc_MsgRunningTick(void);
         void ApplyInputFwdToForceZ(RwReal scale_fwd,RwReal scale_bak);
         void ApplyInputTurnToAngForceY(RwReal scale);
         void ApplyDampening(RwV3d *dampening,RwV3d *ang_dampening);
         void ApplyGravity(void);
         void ApplyNumericalIntegration(void);
         void ApplyCollisionDetectionSendMsg(void);
         void ApplyCollisionDetection(CMsg *CMsg);
         void Set_m_state(RwUInt32 new_m_state);
         void ZeroVector(RwV3d *v);
         void State_Jump_Proc_MsgRunningTick(void);
         void State_Falling_Proc_MsgRunningTick(void);
         void State_Landing_Proc_MsgRunningTick(void);
         void ApplyInputStrafeToForceX(RwReal m_input_strafe_scale);
         void ApplyInputPitchToViewMatrix(RwReal scale_pitch);
         void Proc_FireWeapon(void);
         
         static RpCollisionTriangle *CollideCallBack( RpIntersection * intersection,
                                                      RpCollisionTriangle * collTriangle,
                                                      RwReal distance, void *data );

         static RpAtomic *AtomicCollideCallBack(   RpIntersection * intersection,
                                                   RpWorldSector * sector,
                                                   RpAtomic * atomic,
                                                   RwReal distance, void *data );

         static RpCollisionTriangle *WorldCollideCallBack( RpIntersection * intersection,
            RpWorldSector * sector,
            RpCollisionTriangle * collTriangle,
            RwReal distance __RWUNUSED__, void *data );
         
         CEventId m_Msg_Inq_Forward_Action;             /**< Sent to inquire the current value of the forward action */
         CEventId m_Msg_Inq_Turn_Action;                /**< Sent to inquire the current value of the turn action */
         CEventId m_Msg_Inq_Jump_Action;                /**< Sent to inquire the current value of the jump action */
         CEventId m_Msg_Inq_Strafe_Action;              /**< Sent to inquire the current value of the strafe action */
         CEventId m_Msg_Inq_Fire_Action;                /**< Sent to inquire the current value of the fire action */
         CEventId m_Msg_Inq_Camera_Pitch;               /**< Sent to inquire the current value of the camera
                                                               pitch action */

         CEventId m_Msg_Touch;                          /**< Sent to other objects when the player collides with
                                                               them */

         CEventId m_Msg_Shot;                           /**< Sent to other objects when the player shoots them */
         
         CEventId m_Tx_Pos;                             /**< Send player pos */
         
         RwBool m_teleported_flag;
         
#ifdef RWS_DESIGN
         RwBool m_respondtocamerarequest;               /**< Design Mode only, enable/disable respond toe workspace
                                                               sync camera request */
#endif
         RwFrame *m_cameraFrame;                        /**< Design Mode only, player camera RwFrame */

         CEventId m_Msg_AreaTriggerEvent;               /**< Event on which to perform the area trigger query */
         CEventId m_Msg_AreaTriggerQuery;               /**< Sent out to query for collisions with area triggers */
   };
}
}
#endif
