/**********************************************************************
*
* File :     CGCNStandardController.h
*
* Abstract : This handlers converting raw joystick/button inputs to
* our abstract system where all inputs are mapped from -1 to 1 regardless
* of the type.
*
* Joysticks - respond to inquire event
*           - send down event if there value is increasing
*           - send up event if there value is decreasing
*
* Buttons   - respond to inquire event
*           - send down event if there value is increasing (pressed)
*           - send up event if there value is decreasing (released)
*
* DirectionPad - Treated as both a joystick and a set of 4 buttons
*
**********************************************************************
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
* Copyright (c) 1999 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
************************************************************************/

#ifndef __CGCNSTANDARDCONTROLLER_H__
#define __CGCNSTANDARDCONTROLLER_H__

#if (defined(DOXYGEN) || defined(DOLPHIN))

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"

#include "framework/core/attributehandler/allocation policies/linearallocationpolicy.h"

#include "../../generic/cdebugtools/cdebugtools.h"

#include "framework/mainloop/logic.h"

#include <dolphin.h>

namespace RWS
{
   namespace InputDevices
   {
      //-----------------------------------------------------------------------------------------

      class CGCNStandardControlDevice : public CEventHandler
      {
      public:
         static CGCNStandardControlDevice* new_Instance();
         static void delete_Instance(void);
         static PADStatus *GetGCNPad(unsigned int device_num);

         virtual void HandleEvents(CMsg &pMsg);

      protected:

         // Protected, clients must use new_Instance & delete_Instance
         //
         CGCNStandardControlDevice();
         ~CGCNStandardControlDevice();

      private:

         enum {NUM_GAME_CONTROLLERS = PAD_MAX_CONTROLLERS};

         //-----------------------------------------------------------------------------------------

         static unsigned int refcount;
         static CGCNStandardControlDevice* _instance;
         static PADStatus g_Gamepads[NUM_GAME_CONTROLLERS];

         //-----------------------------------------------------------------------------------------

         // Initialize game controllers
         //
         static BOOL GCNInput_CreateGamepads( void );

         // Scan for input
         //
         void GCNInput_GetInput(void);

      };

      //-----------------------------------------------------------------------------------------

      class CInput
      {
      public:
         CInput() : m_value(0) {};

         CEventId m_Inq;    // Inquire Button value -1 or 1

         void SendInputValue(const CMsg *p_Msg)
         {
            *(float *)(&p_Msg->pData) = m_value;
         }
         void Scale(float scaleFactor) { m_value *= scaleFactor; }

      protected:
         RwReal m_value;
      };

      //-----------------------------------------------------------------------------------------

      class CAButtonInput : public CInput
      {
      public:
         void CalcAnalogueButtonValue(u8 &var);
      };

      //-----------------------------------------------------------------------------------------

      class CDButtonInput : public CInput
      {
      public:
         unsigned int m_mask;
         void CalcDigitalButtonValue(u16 button);
      };

      //-----------------------------------------------------------------------------------------

      class CAJoystickInput: public CInput
      {
      public:
         void CalcAnalogueJoystickValue(s8 &var);
      };

      //-----------------------------------------------------------------------------------------

      /**
      *
      *  \ingroup Mod_InputDevices
      *
      *  Behavior which maps a standard GCN control pad to various events.
      *
      */

      class CGCNStandardController : public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(CGCNStandardController);
         RWS_DECLARE_CLASSID(CGCNStandardController);
         RWS_CATEGORY("InputDevices");
         RWS_DESCRIPTION("Generic GameCube Pad Handler", "Maps all buttons and pads to inquire events, return value of each is always (1 to -1)");


         virtual void HandleAttributes(const CAttributePacket& attr);
         virtual void HandleEvents(CMsg &pMsg);

         CGCNStandardController(const CAttributePacket&);
         ~CGCNStandardController(void);

         /*
          * We've got the following controller configuration.
          *
          *          Shoulder Left          [SL]                     [SR]           Shoulder Right
          *                           _____________________________________[Z]__    Z Button
          *                          /                                          \
          *                         /                               [Y]          \  Y Button
          *          Joystick Left |    [LJ]                                [X]   | X Button
          *                        |                                    [A]       | A Button
          *                        |                     [SP]                     | Start/Pause Button
          *                        |                                [B]           | B Button
          *                Up DPad |      [UDPAD]        ____                     |
          *                        |                    /    \                    |
          *  Left DPad, Right DPad |  [LDPAD] [RDPAD]  |      |       [CJ]        | C Joystick
          *                        |                   |      |                   |
          *              Down DPad  \     [DDPAD]     /        \                 /
          *                          \               /          \               /
          *                           \_____________/            \_____________/
          */

         RWS_BEGIN_COMMANDS

            RWS_ATTRIBUTE(CMD_m_PortSelect, "Port Select", "Select which Game Pad", LIST, RwUInt32, LIST("Pad 1|Pad 2|Pad 3|Pad 4"))

            RWS_MESSAGE( CMD_m_Inq_Start, "Inquire Start Pause Button","return's +-1.0 depending on state of this input device",RECEIVE,return RwReal, 0)
            RWS_MESSAGE( CMD_m_Inq_LeftX, "Inquire Left Joystick X","return's +-1.0 depending on state of this input device",RECEIVE,return RwReal, "INQ_ACTN_TURN")
            RWS_MESSAGE( CMD_m_Inq_LeftY, "Inquire Left Joystick Y","return's +-1.0 depending on state of this input device",RECEIVE,return RwReal, "INQ_ACTN_FWD")
            RWS_MESSAGE( CMD_m_Inq_CX, "Inquire C Joystick X","return's +-1.0 depending on state of this input device",RECEIVE,return RwReal, "INQ_ACTN_STRAFE")
            RWS_MESSAGE( CMD_m_Inq_CY, "Inquire C Joystick Y","return's +-1.0 depending on state of this input device",RECEIVE,return RwReal, "INQ_CAMERA_PITCH")

            RWS_MESSAGE( CMD_m_Inq_A, "Inquire Button A","return's +-1.0 depending on state of this input device",RECEIVE,return RwReal, "INQ_ACTN_FIRE")
            RWS_MESSAGE( CMD_m_Inq_B, "Inquire Button B","return's +-1.0 depending on state of this input device",RECEIVE,return RwReal, "INQ_ACTN_JUMP")
            RWS_MESSAGE( CMD_m_Inq_X, "Inquire Button X","return's +-1.0 depending on state of this input device",RECEIVE,return RwReal, 0)
            RWS_MESSAGE( CMD_m_Inq_Y, "Inquire Button Y","return's +-1.0 depending on state of this input device",RECEIVE,return RwReal, 0)
            RWS_MESSAGE( CMD_m_Inq_Z, "Inquire Button Z Trigger","return's +-1.0 depending on state of this input device",RECEIVE,return RwReal, 0)

            RWS_MESSAGE( CMD_m_Inq_L, "Inquire Button L Shoulder","return's +-1.0 depending on state of this input device",RECEIVE,return RwReal, 0)
            RWS_MESSAGE( CMD_m_Inq_R, "Inquire Button R Shoulder","return's +-1.0 depending on state of this input device",RECEIVE,return RwReal, 0)

            RWS_MESSAGE( CMD_m_Inq_DPadUp,    "Inquire Directional Pad Up","return's +-1.0 depending on state of this input device",RECEIVE,return RwReal, 0)
            RWS_MESSAGE( CMD_m_Inq_DPadDown,  "Inquire Directional Pad Down","return's +-1.0 depending on state of this input device",RECEIVE,return RwReal, 0)
            RWS_MESSAGE( CMD_m_Inq_DPadLeft,  "Inquire Directional Pad Left","return's +-1.0 depending on state of this input device",RECEIVE,return RwReal, 0)
            RWS_MESSAGE( CMD_m_Inq_DPadRight, "Inquire Directional Pad Right","return's +-1.0 depending on state of this input device",RECEIVE,return RwReal, 0)

         RWS_END_COMMANDS;

      private:

         enum { GCN_AJOYSTICK_LeftX, GCN_AJOYSTICK_LeftY, GCN_AJOYSTICK_CX, GCN_AJOYSTICK_CY, GCN_AJOYSTICK_MAX };
         enum { GCN_DBUTTON_DPadDown, GCN_DBUTTON_DPadLeft, GCN_DBUTTON_DPadRight, GCN_DBUTTON_DPadUp,
                GCN_DBUTTON_A, GCN_DBUTTON_B, GCN_DBUTTON_X, GCN_DBUTTON_Y, GCN_DBUTTON_Z, GCN_DBUTTON_MAX };
         enum { GCN_ABUTTON_L, GCN_ABUTTON_R, GCN_ABUTTON_MAX };

         void LinkEvent(CEventId &eventId, const RwChar *event_name);

         // Inquire Controller Stats
         //
         CDButtonInput m_dStart;                         // Start Pause button
         CAJoystickInput m_aJoystick[GCN_AJOYSTICK_MAX]; // Analogue Joystick: LeftX, LeftY, CX, CY
         CDButtonInput m_dButtons[GCN_DBUTTON_MAX];      // Digital Buttons: DPadDown, DPadLeft, DPadRight, DPadUp, A, B, X, Y, Z
         CAButtonInput m_aButtons[GCN_ABUTTON_MAX];      // Analogue Buttons: L, R

         CGCNStandardControlDevice *pCGCNStandardControlDevice;

         unsigned int m_PortSelect;   // Select which port to read
      };
   }//namespace InputDevices
}//namespace RWS

#else
#pragma message ("CGCNStandardController design for GameCube only");
#endif
#endif //__CGCNSTANDARDCONTROLLER_H__
