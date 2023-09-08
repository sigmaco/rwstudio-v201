/**********************************************************************
*
* File :     CXboxStndController.h
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

#ifndef __CXBOXSTANDARDCONTROLLER_H__
#define __CXBOXSTANDARDCONTROLLER_H__

#if defined(DOXYGEN) || defined(_XBOX)

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"

#include "../../generic/cdebugtools/cdebugtools.h"

#include "framework/core/attributehandler/allocation policies/linearallocationpolicy.h"

#include "framework/mainloop/logic.h"

#include <xtl.h>

namespace RWS
{
   namespace InputDevices
   {
      //-----------------------------------------------------------------------------------------
   
      //-----------------------------------------------------------------------------
      // Name: struct XBGAMEPAD
      // Desc: structure for holding Game pad data
      //-----------------------------------------------------------------------------
      struct XBGAMEPAD 
      {
         // New Controller State
         //
         XINPUT_STATE xiState; 
      
         // Device properties
         XINPUT_CAPABILITIES caps;
      
         // Device Id
         HANDLE     hDevice;
      
         // TRUE if controller was just inserted
         BOOL       bInserted;
      };
   
      //-----------------------------------------------------------------------------------------
   
      class CXboxStndCntrlDevice : public CEventHandler, public LinearAllocationPolicy
      {
      public:
         static CXboxStndCntrlDevice* new_Instance();
         static void delete_Instance(void);
      
         virtual void HandleEvents(CMsg &pMsg);
      
         static XBGAMEPAD *GetXBGAMEPAD(unsigned int device_num);
      
      protected:
      
         // Protected, clients must use new_Instance & delete_Instance
         //
         CXboxStndCntrlDevice();
         ~CXboxStndCntrlDevice();
      
      private:
      
         static unsigned int refcount;
      
         static CXboxStndCntrlDevice* _instance;
      
         enum {NUM_GAME_CONTROLLERS = 4};
      
         // Initialize game controllers
         //
         HRESULT XBInput_CreateGamepads( void );
      
         // Scan for input
         //
         void XBInput_GetInput(void);
      
         // Instance of game pad device
         //
         static XBGAMEPAD g_Gamepads[NUM_GAME_CONTROLLERS];
      };
   
      //-----------------------------------------------------------------------------------------
      class CInput
      {
      public:
         CInput() : m_value(0) {};

         CEventId m_Inq;    // Inquire Button value -1 or 1
         
         void SendInputValue(const CMsg *p_Msg)
         {
            *(float*)(&p_Msg->pData) = m_value;
         }
         
      protected:
         RwReal m_value;
      };
   
      //-----------------------------------------------------------------------------------------
      class CButtonInput : public CInput
      {
      public:
         void CalcAnalogueButtonValue(unsigned char &var);
      };
   
      //-----------------------------------------------------------------------------------------
   
      class CWButtonInput : public CInput
      {
      public:
         void CalcButtonValue(WORD rawButton, WORD maskButton);
      };
   
      //-----------------------------------------------------------------------------------------
   
      class CJoystickInput: public CInput
      {
      public:
         void CalcJoystickValue(SHORT &var);
         void CalcJoystickValueNeg(SHORT &var);
      };
   
      //-----------------------------------------------------------------------------------------
   
      class CWJoystickInput: public CInput
      {
      public:
         CWJoystickInput() : m_old_value(0)
         {
         }
      
         // ------------------------------------------------------------------------------------------
      
         void CalcJoystickValue(WORD rawButton, WORD maskButtonA, WORD maskButtonB);
      
         // -----------------------------------------------------------------------------------------
      
      private:
         RwReal m_old_value;
      };
   
      //-----------------------------------------------------------------------------------------
   
      /**
      *
      *  \ingroup Mod_InputDevices
      *
      *  Behavior which maps a standard XBox control pad to various events.
      *
      */

      class CXboxStndController : public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy 
      {
      public:
         RWS_MAKENEWCLASS(CXboxStndController);
         RWS_DECLARE_CLASSID(CXboxStndController);
         RWS_CATEGORY("InputDevices");
         RWS_DESCRIPTION("Generic XBOX Pad Handler",
                         "Maps all buttons and pads to inquire events, return value of each is always (1 to - 1)");
      
         virtual void HandleAttributes(const CAttributePacket& attr);
         virtual void HandleEvents(CMsg &pMsg);
      
         CXboxStndController(const CAttributePacket&);
         ~CXboxStndController(void);
      
         RWS_BEGIN_COMMANDS

            RWS_ATTRIBUTE(CMD_m_PortSelect,    
                          "Port Select",                 
                          "Select which Game Pad",   
                          LIST,    
                          RwUInt32,   
                          LIST("Pad 1|Pad 2|Pad 3|Pad 4"))
         
            RWS_MESSAGE( CMD_m_Inq_LJX,    
                         "Left Joystick X Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         "INQ_ACTN_TURN")
         
            RWS_MESSAGE( CMD_m_Inq_LJY,    
                         "Left Joystick Y Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         "INQ_ACTN_FWD")
         
            RWS_MESSAGE( CMD_m_Inq_RJX,    
                         "Right Joystick X Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         "INQ_ACTN_STRAFE")
         
            RWS_MESSAGE( CMD_m_Inq_RJY,    
                         "Right Joystick Y Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         "INQ_CAMERA_PITCH")
         
            RWS_MESSAGE( CMD_m_Inq_B1,    
                         "Button 1 (A) Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         "INQ_ACTN_FIRE")
         
            RWS_MESSAGE( CMD_m_Inq_B2,    
                         "Button 2 (B) Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         "INQ_ACTN_JUMP")
         
            RWS_MESSAGE( CMD_m_Inq_B3,    
                         "Button 3 (X) Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         0)
         
            RWS_MESSAGE( CMD_m_Inq_B4,    
                         "Button 4 (Y) Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         0)
         
            RWS_MESSAGE( CMD_m_Inq_B5,    
                         "Button 5 (BLACK) Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         0)
         
            RWS_MESSAGE( CMD_m_Inq_B6,    
                         "Button 6 (WHITE) Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         0)
         
            RWS_MESSAGE( CMD_m_Inq_B7,    
                         "Button 7 (L Shoulder) Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         0)
         
            RWS_MESSAGE( CMD_m_Inq_B8,    
                         "Button 8 (R Shoulder) Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         0)
         
            RWS_MESSAGE( CMD_m_Inq_wB1,    
                         "Directional Pad Up Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 0)
         
            RWS_MESSAGE( CMD_m_Inq_wB2,    
                         "Directional Pad Down Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         0)
         
            RWS_MESSAGE( CMD_m_Inq_wB3,    
                         "Directional Pad Left Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         0)
         
            RWS_MESSAGE( CMD_m_Inq_wB4,    
                         "Directional Pad Right Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         0)
         
            RWS_MESSAGE( CMD_m_Inq_wB5,    
                         "Start Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         0)
         
            RWS_MESSAGE( CMD_m_Inq_wB6,    
                         "Back Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         0)
         
            RWS_MESSAGE( CMD_m_Inq_wB7,    
                         "Left Thumb Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         0)
         
            RWS_MESSAGE( CMD_m_Inq_wB8,    
                         "Right Thumb Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         0)
         
            RWS_MESSAGE( CMD_m_Inq_BJX,    
                         "Digital Joystick X Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         0)
         
            RWS_MESSAGE( CMD_m_Inq_BJY,    
                         "Digital Joystick Y Inquire",
                         "return's +-1.0 depending on state of this input device",
                         RECEIVE,
                         return RwReal, 
                         0)
         
            RWS_END_COMMANDS;
         
      private:
      
         void LinkEvent(CEventId &eventId, const RwChar *event_name);
      
         // Inquire Controller Stats
         //
         CJoystickInput m_LJX;   // Left Joystick X
         CJoystickInput m_LJY;   // Left Joystick Y
         CJoystickInput m_RJX;   // Right Joystick X
         CJoystickInput m_RJY;   // Right Joystick Y
      
         CButtonInput m_B[8];  // Analogue Buttons
         CWButtonInput m_wB[8];  // Digital Buttons
      
         CWJoystickInput m_BJX;  // Button Joystick X
         CWJoystickInput m_BJY;  // Button Joystick Y
      
         CXboxStndCntrlDevice *pCXboxStndCntrlDevice;

         unsigned int m_PortSelect;       // Select which port to read
      };
   }//namespace InputDevices
}//namespace RWS

#else
#pragma message ("CXboxStndController design for xbox only");
#endif
#endif
