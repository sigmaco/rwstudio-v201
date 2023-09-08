/**********************************************************************
*
* File :     CGCNStandardController.h
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
#include "precomp.h"

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#endif

#if (defined(DOXYGEN) || defined(DOLPHIN))

#include <dolphin.h>

#include "cgcnstandardcontroller.h"
#include "framework/core/macros/debugmacros.h"

namespace RWS
{
   namespace InputDevices
   {
      RWS_IMPLEMENT_CLASSID(CGCNStandardController);
      RWS_REGISTERCLASS(CGCNStandardController);

      //-----------------------------------------------------------------------------
      // Globals
      //-----------------------------------------------------------------------------

      // See Singleton pattern,
      //
      CGCNStandardControlDevice* CGCNStandardControlDevice::_instance;

      // Storage of raw game pad data, also maintains a handle to each device
      //
      PADStatus CGCNStandardControlDevice::g_Gamepads[NUM_GAME_CONTROLLERS];

      // Number of CGCNStandardControlDevice opened, used to determine when the device can be released
      //
      unsigned int CGCNStandardControlDevice::refcount = 0;

      //////////////////////////////////////////////////////////////////
      //
      // @mfunc void | CalcAnalogueJoystickValue | convert raw joystick [-128, 128] -> [-1, 1]
      //
      //
      //
      //
      void CAJoystickInput::CalcAnalogueJoystickValue(s8 &var)
      {
         m_value = ((float)var) / 128.0f;

         if ((m_value > -0.15f) && (m_value < 0.15f)) m_value = 0.0f; // Hysteresis in center area
         m_value = (m_value < 0.0) ? (-(m_value * m_value)):(m_value * m_value); // asymmetric paraboloid
      }

      //////////////////////////////////////////////////////////////////
      //
      // @mfunc void | CalcAnalogueButtonValue | Convert raw button values [0, 255] -> [-1, 1]
      //
      //
      //
      //
      void CAButtonInput::CalcAnalogueButtonValue(u8 &var)
      {
         m_value = (((float)var) / 127.5f) - 1.0f; // 0 to 255 -> -1 to 1
      }

      //////////////////////////////////////////////////////////////////
      //
      // @mfunc void | CalcDigitalButtonValue | Convert raw button values to {-1, 1}
      //
      //
      //
      //
      void CDButtonInput::CalcDigitalButtonValue(u16 button)
      {
         m_value = (button & m_mask) ? 1.0f : -1.0f;
      }

      //////////////////////////////////////////////////////////////////
      //
      // @mfunc void | new_Instance | Create a single CGCNStandardControlDevice device
      //
      //
      //
      //
      CGCNStandardControlDevice* CGCNStandardControlDevice::new_Instance(void)
      {
         if (_instance == 0)
         {
            _instance = ::RWS_NEW CGCNStandardControlDevice;
         }

         refcount += 1;

         return _instance;
      }

      //////////////////////////////////////////////////////////////////
      //
      // @mfunc void | delete_Instance |  Deletes single instance of CGCNStandardControlDevice if, reference count is 0
      //
      //
      //
      //
      void CGCNStandardControlDevice::delete_Instance(void)
      {
         refcount -= 1;

         if (refcount == 0)
         {
//            delete _instance;
//
            _instance = 0;
         }
      }

      //////////////////////////////////////////////////////////////////
      //
      // @mfunc void | CGCNStandardControlDevice | Initialize CGCNStandardControlDevice class
      //
      //
      //
      //
      CGCNStandardControlDevice::CGCNStandardControlDevice(void) : InitCEventHandler(0)
      {
         GCNInput_CreateGamepads();

         LinkMsg(iMsgRunningTick, 0);
         LinkMsg(iMsgPausedTick, 0);
      }

      //////////////////////////////////////////////////////////////////
      //
      // @mfunc void | ~CGCNStandardControlDevice | Deconstruct CGCNStandardControlDevice class
      //
      //
      //
      //
      CGCNStandardControlDevice::~CGCNStandardControlDevice(void)
      {
         UnLinkMsg(iMsgPausedTick);
         UnLinkMsg(iMsgRunningTick);
      }

      //////////////////////////////////////////////////////////////////
      //
      // @mfunc BOOL | GCNInput_CreateGamepads | Initialize gamepad devices
      //
      //
      //
      //
      //-----------------------------------------------------------------------------
      // Name: GCNInput_CreateGamepads()
      // Desc: Creates the gamepad devices
      //-----------------------------------------------------------------------------
      BOOL CGCNStandardControlDevice::GCNInput_CreateGamepads( void )
      {
         RWS_FUNCTION("CGCNStandardControlDevice::GCNInput_CreateGamepads");

         /* We can only initialize the hardware once. */
         static BOOL initialized = FALSE;

         /* If the pad isn't initialized attempt to initialize it. */
         if (FALSE == initialized)
         {
            /*
             * Initialize the pad.
             *
             * The video interface (VI) must be initialized via VIInit() before calling
             * PADInit() since the VI controls the timing of the hardware controller
             * data sampling. PADInit must go after RwEngineStart.
             */
            BOOL success = PADInit();
            RWS_ASSERT(success,"CGCNStandardControlDevice::GCNInput_CreateGamepads PADInit() failed");
            initialized = success;
         }

         RWS_RETURN(initialized);
      }

      //////////////////////////////////////////////////////////////////
      //
      // @mfunc void | GCNInput_GetInput | Process input from game pads
      //
      //
      //
      //
      //-----------------------------------------------------------------------------
      // Name: GCNInput_GetInput()
      // Desc: Processes input from the gamepads
      //-----------------------------------------------------------------------------
      void CGCNStandardControlDevice::GCNInput_GetInput( void)
      {
         RWS_FUNCTION("CGCNStandardControlDevice::GCNInput_GetInput");

         /* Read controller data */
         PADRead(g_Gamepads);

         RWS_RETURNVOID();
      }

      //////////////////////////////////////////////////////////////////
      //
      // @mfunc void | GetGCNPad | Return GCNPAD for a specified device
      //
      //
      //
      //
      PADStatus *CGCNStandardControlDevice::GetGCNPad(unsigned int device_num)
      {
         if (!_instance) return 0;

         if (device_num<NUM_GAME_CONTROLLERS)
         {
            return &g_Gamepads[device_num];
         }
         else
         {
            return 0;
         }
      }

      //////////////////////////////////////////////////////////////////
      //
      // @mfunc void | HandleEvents | Process events
      //
      //
      //
      //
      void CGCNStandardControlDevice::HandleEvents(CMsg &pMsg)
      {
         if (pMsg.Id == iMsgRunningTick) GCNInput_GetInput();
      }

      //////////////////////////////////////////////////////////////////
      //
      // @mfunc void | CXboxStndController | Initialize controller Input
      //
      //
      //
      //
      CGCNStandardController::CGCNStandardController(const CAttributePacket&) : InitCEventHandler(0), m_PortSelect(0)
      {
         pCGCNStandardControlDevice = CGCNStandardControlDevice::new_Instance();

         // Link to the running tick in order to poll controller devices
         //
         LinkMsg(iMsgRunningTick,0);

         // Setup button masks
         //
         static unsigned int mask_table[]=
         {
            PAD_BUTTON_DOWN,
            PAD_BUTTON_LEFT,
            PAD_BUTTON_RIGHT,
            PAD_BUTTON_UP,
            PAD_BUTTON_A,
            PAD_BUTTON_B,
            PAD_BUTTON_X,
            PAD_BUTTON_Y,
            PAD_TRIGGER_Z
         };

         for(unsigned int item = 0; item < GCN_DBUTTON_MAX; item++)
         {
            m_dButtons[item].m_mask = mask_table[item];
         }

         //
         // Defaults
         //
         ReplaceLinkedMsg(m_aJoystick[GCN_AJOYSTICK_LeftX].m_Inq, "INQ_ACTN_TURN", "return RwReal");
         ReplaceLinkedMsg(m_aJoystick[GCN_AJOYSTICK_LeftY].m_Inq, "INQ_ACTN_FWD", "return RwReal");
         ReplaceLinkedMsg(m_aJoystick[GCN_AJOYSTICK_CX].m_Inq, "INQ_ACTN_STRAFE", "return RwReal");
         ReplaceLinkedMsg(m_aJoystick[GCN_AJOYSTICK_CY].m_Inq, "INQ_CAMERA_PITCH", "return RwReal");
         ReplaceLinkedMsg(m_dButtons[GCN_DBUTTON_A].m_Inq, "INQ_ACTN_JUMP", "return RwReal");
         ReplaceLinkedMsg(m_dButtons[GCN_DBUTTON_B].m_Inq, "INQ_ACTN_FIRE", "return RwReal");
      }

      //////////////////////////////////////////////////////////////////
      //
      // @mfunc void | ~CGCNStandardController | Deconstruct controller Input
      //
      //
      //
      //
      CGCNStandardController::~CGCNStandardController()
      {
         int item;

         UnLinkMsg(m_dStart.m_Inq);
         UnRegisterMsg(m_dStart.m_Inq);

         for(item = 0; item < GCN_AJOYSTICK_MAX; item++)
         {
            UnLinkMsg(m_aJoystick[item].m_Inq);
            UnRegisterMsg(m_aJoystick[item].m_Inq);
         }

         for(item = 0; item < GCN_DBUTTON_MAX; item++)
         {
            UnLinkMsg(m_dButtons[item].m_Inq);
            UnRegisterMsg(m_dButtons[item].m_Inq);
         }

         for(item = 0; item < GCN_ABUTTON_MAX; item++)
         {
            UnLinkMsg(m_aButtons[item].m_Inq);
            UnRegisterMsg(m_aButtons[item].m_Inq);
         }

         UnLinkMsg(iMsgRunningTick);

         // Attempt to shutdown the device driver
         //
         CGCNStandardControlDevice::delete_Instance();
      }

      //////////////////////////////////////////////////////////////////
      //
      // @mfunc void | HandleAttributes | Process Parameters
      //
      //
      //
      //
      void CGCNStandardController::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("CGCNStandardController::HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);

         CAttributeCommandIterator attrIt(attr,RWS_CLASSID_OF(CGCNStandardController));

         while(!attrIt.IsFinished())
         {
            switch(attrIt->GetCommandId())
            {
               //////////////////////////////////////////////////////////////////
               case CMD_m_PortSelect:
                  attrIt->GetCommandData(m_PortSelect);
                  break;

               //////////////////////////////////////////////////////////////////
               case CMD_m_Inq_Start:
                  ReplaceLinkedMsg(m_dStart.m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
                  break;

               //////////////////////////////////////////////////////////////////
               case CMD_m_Inq_LeftX:
                  ReplaceLinkedMsg(m_aJoystick[GCN_AJOYSTICK_LeftX].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
                  break;
               case CMD_m_Inq_LeftY:
                  ReplaceLinkedMsg(m_aJoystick[GCN_AJOYSTICK_LeftY].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
                  break;
               case CMD_m_Inq_CX:
                  ReplaceLinkedMsg(m_aJoystick[GCN_AJOYSTICK_CX].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
                  break;
               case CMD_m_Inq_CY:
                  ReplaceLinkedMsg(m_aJoystick[GCN_AJOYSTICK_CY].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
                  break;

               //////////////////////////////////////////////////////////////////
               case CMD_m_Inq_DPadUp:
                  ReplaceLinkedMsg(m_dButtons[GCN_DBUTTON_DPadUp].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
                  break;
               case CMD_m_Inq_DPadDown:
                  ReplaceLinkedMsg(m_dButtons[GCN_DBUTTON_DPadDown].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
                  break;
               case CMD_m_Inq_DPadLeft:
                  ReplaceLinkedMsg(m_dButtons[GCN_DBUTTON_DPadLeft].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
                  break;
               case CMD_m_Inq_DPadRight:
                  ReplaceLinkedMsg(m_dButtons[GCN_DBUTTON_DPadRight].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
                  break;
               case CMD_m_Inq_A:
                  ReplaceLinkedMsg(m_dButtons[GCN_DBUTTON_A].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
                  break;
               case CMD_m_Inq_B:
                  ReplaceLinkedMsg(m_dButtons[GCN_DBUTTON_B].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
                  break;
               case CMD_m_Inq_X:
                  ReplaceLinkedMsg(m_dButtons[GCN_DBUTTON_X].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
                  break;
               case CMD_m_Inq_Y:
                  ReplaceLinkedMsg(m_dButtons[GCN_DBUTTON_Y].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
                  break;
               case CMD_m_Inq_Z:
                  ReplaceLinkedMsg(m_dButtons[GCN_DBUTTON_Z].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
                  break;

               //////////////////////////////////////////////////////////////////
               case CMD_m_Inq_L:
                  ReplaceLinkedMsg(m_aButtons[GCN_ABUTTON_L].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
                  break;
               case CMD_m_Inq_R:
                  ReplaceLinkedMsg(m_aButtons[GCN_ABUTTON_R].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
                  break;
            }
            ++attrIt;
         }
         RWS_RETURNVOID();
      }

      //////////////////////////////////////////////////////////////////
      //
      // @mfunc void | HandleEvents | Process events
      //
      //
      //
      //
      void CGCNStandardController::HandleEvents(CMsg &pMsg)
      {
         if (pMsg.Id == iMsgRunningTick)
         {
            PADStatus *pGCNPad = CGCNStandardControlDevice::GetGCNPad(m_PortSelect);   // Pointer to Game Pad

            // Convert raw pad values to [-1, 1]
            //
            m_aJoystick[GCN_AJOYSTICK_LeftX].CalcAnalogueJoystickValue(pGCNPad->stickX);
            m_aJoystick[GCN_AJOYSTICK_LeftX].Scale(-1.5);
            m_aJoystick[GCN_AJOYSTICK_LeftY].CalcAnalogueJoystickValue(pGCNPad->stickY);
            m_aJoystick[GCN_AJOYSTICK_LeftY].Scale(1.5);
            m_aJoystick[GCN_AJOYSTICK_CX].CalcAnalogueJoystickValue(pGCNPad->substickX);
            m_aJoystick[GCN_AJOYSTICK_CX].Scale(-4.0);
            m_aJoystick[GCN_AJOYSTICK_CY].CalcAnalogueJoystickValue(pGCNPad->substickY);
            m_aJoystick[GCN_AJOYSTICK_CY].Scale(4.0);
            m_aButtons[GCN_ABUTTON_L].CalcAnalogueButtonValue(pGCNPad->triggerLeft);
            m_aButtons[GCN_ABUTTON_R].CalcAnalogueButtonValue(pGCNPad->triggerRight);

            for(unsigned int item = 0; item < GCN_DBUTTON_MAX; item++)
            {
               m_dButtons[item].CalcDigitalButtonValue(pGCNPad->button);
            }
         }
         //Check for Inquire items...
         //
         else
         {
            BOOL search = TRUE;
            unsigned int item;

            //Analogue Joystick
            //
            for(item = 0; search && (item < GCN_AJOYSTICK_MAX); item++)
            {
               if (pMsg.Id == m_aJoystick[item].m_Inq)
               {
                  m_aJoystick[item].SendInputValue(&pMsg);
                  search = FALSE;
               }
            }

            //Digital Buttons
            //
            for(item = 0; search && (item < GCN_DBUTTON_MAX); item++)
            {
               if (pMsg.Id == m_dButtons[item].m_Inq)
               {
                  m_dButtons[item].SendInputValue(&pMsg);
                  search = FALSE;
               }
            }

            //Analogue Buttons
            //
            for(item = 0; search && (item < GCN_ABUTTON_MAX); item++)
            {
               if (pMsg.Id == m_aButtons[item].m_Inq)
               {
                  m_aButtons[item].SendInputValue(&pMsg);
                  search = FALSE;
               }
            }
         }
      }
   }//namespace InputDevices
}//namespace RWS

#endif
