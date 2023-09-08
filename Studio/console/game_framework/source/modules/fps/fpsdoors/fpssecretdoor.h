/*****************************************************************************
*
* File :     FPSSecretDoor.h
*
* Abstract : Secret doors await a trigger event, and then use two transforms 
*            to move themselves. i.e. back followed by to the side.
*
*            This is implemented as a Finite State Machine
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

#if !defined (__FPS_SECRET_DOOR_H__)
#define __FPS_SECRET_DOOR_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "../fpstoggle/fpstoggle.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace FPS
   {
      /**
      *
      * \ingroup Mod_FPS
      *
      * Implementation for a secret Door.  This door can be triggered by shooting it,  touching it,  or by
      * sending it a message. The action for the door is split into several states,  these states control
      * the movement of the door.
      *
      */
      class FPSSecretDoor : public CSystemCommands, public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(FPSSecretDoor);
         RWS_DECLARE_CLASSID(FPSSecretDoor);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Sliding Secret Door", "A generic four state sliding door.");
         
         FPSSecretDoor (const CAttributePacket&);
         ~FPSSecretDoor (void);
         
         RWS_BEGIN_COMMANDS
            RWS_MESSAGE( CMD_targetname, "Target name","Trigger the secret door", RECEIVE,0,"ACTN_PLAYERTOUCH")
            RWS_ATTRIBUTE( CMD_angle,    "Angle","Determines the opening direction", SLIDER,RwReal,RANGE(0,0,360))
            RWS_ATTRIBUTE( CMD_spawnflags, "Flags", "", BITFIELD, RwUInt32, LIST("SECRET_OPEN_ONCE|SECRET_1ST_LEFT|SECRET_1ST_DOWN"))
            RWS_ATTRIBUTE( CMD_speed,   "Speed","Movement speed (150 default) units per second",SLIDER,RwUInt32,RANGE(1,150,999))
            RWS_END_COMMANDS;
         
         enum {
            SECRET_OPEN_ONCE = 1,   // stays open
               SECRET_1ST_LEFT  = 2,   // 1st move is left of arrow
               SECRET_1ST_DOWN  = 4   // 1st move is down from arrow
         };
         
         virtual void HandleAttributes(const CAttributePacket& attr);
         virtual void HandleEvents(CMsg &pMsg);
         
      private:
         void TriggerDoor(void);
         static RpAtomic *CalculateBoundingBox (RpAtomic *atomic, void *data);
         void GenerateTransformationMatrices (void);
         
         void Door_ShiftBack( void );
         void Door_Wait( void );
         void Door_Slide( void );
         void Door_Wait_Return( void );
         void Door_Return_Slide( void );
         void Door_Wait_Slide( void );
         void Door_Shift_Return( void );
         
         CEventId m_MsgTargetName;
         
         RwMatrix *m_Slide1;                     /**< Matrices used to represent shift and slide of doors */
         RwMatrix *m_Slide2;                     /**< Matrices used to represent shift and slide of doors */
         RwMatrix *m_Shift1;                     /**< Matrices used to represent shift and slide of doors */
         RwMatrix *m_Shift2;                     /**< Matrices used to represent shift and slide of doors */
         
         RwUInt32 m_FrameCount;                  /**< Used as a frame counter to determine the state of the door */
         
         RwUInt32 m_speed;                       /**< Door speed */
         
         RwUInt32 m_NoShiftFrames;               /**< Number of frames used for shift / slide animation */ 
         RwUInt32 m_NoSlideFrames;               /**< Number of frames used for shift / slide animation */ 
         
         RwReal   m_Angle;
         RwUInt32 m_spawnflags;
         
         // Are we animating ?
         RwBool   m_bAnimating;
         
         enum DoorState
         {
            DoorState0,          // Event not yet fired
            DoorState1,          // Sliding back
            DoorState2,          // Wait after first movement
            DoorState3,          // Sliding sideways
            DoorState4,          // Wait here to go back
            DoorState5,          // Return sliding sideways
            DoorState6,          // Wait for final slide
            DoorState7,          // Return sliding forwards
            DoorState_Complete   // Open Once Doors end up in this state,
                                 // after completing the first slide operation.                
         } m_DoorState;
         
         CAtomicPtr m_pAtomic;
      };
      
   } // namespace FPS
}// namespace RWS

#endif