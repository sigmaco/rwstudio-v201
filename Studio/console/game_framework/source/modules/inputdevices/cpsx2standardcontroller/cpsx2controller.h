/**********************************************************************
*
* File :     CPSX2StndController.h
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
* 
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


#ifndef __GFPSX2_CONTROLLER_H__
#define __GFPSX2_CONTROLLER_H__

#if defined(DOXYGEN) || defined(SKY)

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"

#include "../../generic/cdebugtools/cdebugtools.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

#include "framework/mainloop/logic.h"

#include <libpad.h>

namespace RWS
{
   namespace InputDevices
   {
      // a Few defines for controlling button mapping
      static const RwUInt32 PSX2Control_ButtonA = 2; 
      static const RwUInt32 PSX2Control_ButtonB = 3;
      static const RwUInt32 PSX2Control_ButtonC = 6;
      static const RwUInt32 PSX2Control_ButtonD = 0;
      static const RwUInt32 PSX2Control_ButtonE = 1;
      static const RwUInt32 PSX2Control_ButtonF = 3;

      static const RwUInt32 PSX2Control_LTrigger = 6;
      static const RwUInt32 PSX2Control_RTrigger = 7;

      /**
      *
      *  \ingroup Mod_InputDevices
      *
      *  Behavior which maps a standard PS2 control pad to various events.
      (
      */

      class CPSX2StndController : public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(CPSX2StndController);
         RWS_DECLARE_CLASSID(CPSX2StndController);
         RWS_CATEGORY("InputDevices");
         RWS_DESCRIPTION("Generic PSX Pad Handler", "Maps all buttons and pads to inquire events, return value of "
                         "each is always (1 to -1)");

         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);

         CPSX2StndController(const CAttributePacket&);
      
         virtual ~CPSX2StndController();

         RWS_BEGIN_COMMANDS

            RWS_ATTRIBUTE(CMD_m_PortSelect,  
                          "Port Select",
                          "Select port number UNIMPLEMENTED",
                          LIST,
                          RwUInt32,
                          LIST("Port 1|Port 2"))

            RWS_ATTRIBUTE(CMD_m_SlotSelect,  
                          "Slot Select",
                          "Select slot number UNIMPLEMENTED",
                          LIST,
                          RwUInt32,
                          LIST("Slot 1|Slot 2|Slot 3|Slot 4"))

            RWS_ATTRIBUTE(CMD_m_DeadZone,    
                          "Joystick DeadZone",
                          "Set joystick dead zone",   
                          SLIDER,  
                          RwReal,  
                          RANGE(0, 0.25, 1))

            RWS_MESSAGE( CMD_m_Inq_Start,    
                         "Inquire Start Button",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         0)

            RWS_MESSAGE( CMD_m_Inq_Select,   
                         "Inquire Select Button",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         0)

            RWS_MESSAGE( CMD_m_Inq_Triangle, 
                         "Inquire Triangle Button",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         "INQ_ACTN_JUMP")
                         
            RWS_MESSAGE( CMD_m_Inq_Cross,    
                         "Inquire Cross Button",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         "INQ_ACTN_FIRE")

            RWS_MESSAGE( CMD_m_Inq_Square,   
                         "Inquire Square Button",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         0)

            RWS_MESSAGE( CMD_m_Inq_Circle,   
                         "Inquire Circle Button",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         0)

            RWS_MESSAGE( CMD_m_Inq_PAD_Up,   
                         "Inquire Up Button",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         0)

            RWS_MESSAGE( CMD_m_Inq_PAD_Down, 
                         "Inquire Down Button",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         0)

            RWS_MESSAGE( CMD_m_Inq_PAD_Left, 
                         "Inquire Left Button",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         0)

            RWS_MESSAGE( CMD_m_Inq_PAD_Right,
                         "Inquire Right Button",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         0)

            RWS_MESSAGE( CMD_m_Inq_DPAD_Y,   
                         "Inquire Up/Down As Joystick",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         0)

            RWS_MESSAGE( CMD_m_Inq_DPAD_X,   
                         "Inquire Left/Right As Joystick",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         0)

            RWS_MESSAGE( CMD_m_Inq_L1,       
                         "Inquire L1 Button",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         0)

            RWS_MESSAGE( CMD_m_Inq_L2,       
                         "Inquire L2 Button",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         0)

            RWS_MESSAGE( CMD_m_Inq_R1,       
                         "Inquire R1 Button",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         0)

            RWS_MESSAGE( CMD_m_Inq_R2,       
                         "Inquire R2 Button",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         0)

            RWS_MESSAGE( CMD_m_Inq_LR1,       
                         "Inquire L1 R1 As Joystick",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         "INQ_ACTN_STRAFE")

            RWS_MESSAGE( CMD_m_Inq_LR2,       
                         "Inquire L2 R2 As Joystick",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         0)

            RWS_MESSAGE( CMD_m_LPAD_X,       
                         "Inquire Left Analogue X Joystick",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         "INQ_ACTN_TURN")

            RWS_MESSAGE( CMD_m_LPAD_Y,       
                         "Inquire Left Analogue Y Joystick",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         "INQ_ACTN_FWD")

            RWS_MESSAGE( CMD_m_RPAD_X,       
                         "Inquire Right Analogue X Joystick",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         0)

            RWS_MESSAGE( CMD_m_RPAD_Y,       
                         "Inquire Right Analogue Y Joystick",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal,
                         "INQ_CAMERA_PITCH")
         RWS_END_COMMANDS;

      protected:     
         u_int paddata;    // Extracted Pad Data

         u_char rdata[32]; // Raw Pad Data

         RwUInt32 m_PortSelect;  // Port Number
         RwUInt32 m_SlotSelect;  // Slot Number

         RwReal m_DeadZone;
      
         CEventId m_Inq_Start;
         CEventId m_Inq_Select;

         CEventId m_Inq_Triangle;
         CEventId m_Inq_Cross;
         CEventId m_Inq_Square;
         CEventId m_Inq_Circle;

         CEventId m_Inq_PAD_Up;
         CEventId m_Inq_PAD_Down;
         CEventId m_Inq_PAD_Left;
         CEventId m_Inq_PAD_Right;

         CEventId m_Inq_DPAD_X;
         CEventId m_Inq_DPAD_Y;

         CEventId m_Inq_LR1;
         CEventId m_Inq_LR2;

         CEventId m_Inq_L1;
         CEventId m_Inq_L2;

         CEventId m_Inq_R1;
         CEventId m_Inq_R2;

         CEventId m_LPAD_X;
         CEventId m_LPAD_Y;

         CEventId m_RPAD_X;
         CEventId m_RPAD_Y;

         void CalcJoyVar(float &joyvar);
         void ReadInput();
      };
   }//namespace InputDevices
}//namespace RWS

#else
#pragma message ("CPSX2StndController design for PSX2 only");
#endif

#endif