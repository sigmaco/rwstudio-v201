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
#include "precomp.h"

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#endif

#if defined(DOXYGEN) || defined(_XBOX)

#include <xtl.h>
#include "cxboxstndcontroller.h"
#include "framework/core/macros/debugmacros.h"

namespace RWS
{
   namespace InputDevices
   {
      RWS_IMPLEMENT_CLASSID(CXboxStndController);
      RWS_REGISTERCLASS(CXboxStndController);
      const float XBOX_THUMB_DEAD_ZONE = 0.35f;      

      //-----------------------------------------------------------------------------
      // Globals
      //-----------------------------------------------------------------------------
      
      // See Singleton pattern,
      //
      CXboxStndCntrlDevice* CXboxStndCntrlDevice::_instance;
   
      // Storage of raw game pad data, also maintains a handle to each device
      //
      XBGAMEPAD CXboxStndCntrlDevice::g_Gamepads[NUM_GAME_CONTROLLERS];
   
      // Number of CXboxStndCntrlDevice opened, used to determine when the device can be released
      //
      unsigned int CXboxStndCntrlDevice::refcount = 0;
  
   
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | CalcJoystickValue | convert raw joystick values -1 to 1
      //
      //
      //
      //
      void CJoystickInput::CalcJoystickValue(SHORT &var)
      {
         RWS_FUNCTION("RWS::InputDevices::CJoystickInput::CalcJoystickValue");
         
         m_value = 1.0f - (((float)var + (float)0x8000) / 32767.5f); // -0x8000 to 7fff -> -1 to 1
      
         // Hysteresis in center area
         if (m_value >- XBOX_THUMB_DEAD_ZONE && m_value < XBOX_THUMB_DEAD_ZONE) m_value = 0.0f;   

         RWS_RETURNVOID();
      }

      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | CalcJoystickValue | convert raw joystick values -1 to 1
      //
      //
      //
      //
      void CJoystickInput::CalcJoystickValueNeg(SHORT &var)
      {
         RWS_FUNCTION("RWS::InputDevices::CJoystickInput::CalcJoystickValueNeg");

         m_value = (((float)var + (float)0x8000) / 32767.5f) - 1.0f; // -0x8000 to 7fff -> -1 to 1
      
         // Hysteresis in center area
         if (m_value >- XBOX_THUMB_DEAD_ZONE && m_value < XBOX_THUMB_DEAD_ZONE) m_value = 0.0f;   

         RWS_RETURNVOID();
      }
   
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | CalcButtonValue | Convert raw button values 0 to 255, to -1 to 1
      //
      //
      //
      //
      void CButtonInput::CalcAnalogueButtonValue(unsigned char &var)
      {
         RWS_FUNCTION("RWS::InputDevices::CButtonInput::CalcAnalogueButtonValue");

         m_value = (((float)var) / 127.5f) - 1.0f; // 0 to 255 -> -1 to 1

         RWS_RETURNVOID();
      }
   
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | CalcButtonValue | Convert raw button values (single bit in WORD), to -1 to 1
      //
      //
      //
      //
      void CWButtonInput::CalcButtonValue(WORD rawButton, WORD maskButton)
      {
         RWS_FUNCTION("RWS::InputDevices::CButtonInput::CalcButtonValue");

         if ( (rawButton&maskButton) != 0 )
         {
            m_value = 1.0f;
         }
         else
         {
            m_value = -1.0f;
         }

         RWS_RETURNVOID();
      }
   
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | CalcButtonValue | Convert raw button values (single bit in WORD), to -1 to 1
      //
      //
      //
      //
      void CWJoystickInput::CalcJoystickValue(WORD rawButton, WORD maskButtonA, WORD maskButtonB)
      {
         RWS_FUNCTION("RWS::InputDevices::CWJoystickInput::CalcJoystickValue");

         if ( (rawButton&maskButtonA) != 0 )
         {
            m_value = 1.0f;
         }
         else
         {
            m_value = 0;
         }
      
         if ( (rawButton & maskButtonB) != 0 )
         {
            m_value = -1.0f;
         }

         RWS_RETURNVOID();
      }
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | new_Instance | Create a single CXboxStndCntrlDevice device
      //
      //
      //
      //
      CXboxStndCntrlDevice* CXboxStndCntrlDevice::new_Instance(void)
      {
         RWS_FUNCTION("RWS::InputDevices::CXboxStndCntrlDevice::new_Instance");

         if (_instance == 0)
         {
            _instance = ::RWS_NEW CXboxStndCntrlDevice;
         }

         // Temporary Fix: there dosn't seem to be a way to shutdown an xbox pad so until that is fixed cannot delete
         // the instance of CXboxStndCntrlDevice
         //
//       refcount +=1;
         refcount = 1;
      
         RWS_RETURN(_instance);
      }
   
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | delete_Instance |  Deletes single instance of CXboxStndCntrlDevice if, reference count is 0
      //
      //
      //
      //
      void CXboxStndCntrlDevice::delete_Instance(void)
      {
         // Temporary Fix: see CXboxStndCntrlDevice* CXboxStndCntrlDevice::new_Instance(void)
         //
/*
         refcount-=1;
      
         // FE Shut Down of XBox game device,
         //
         // There don't seem to be any OS calls to close down the controller device, so until this is worked out
         // the device cannot be deleted...
         
         if(refcount==0) 
         {
            delete _instance;
      
           _instance = 0;
         }
*/
      }
   
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | CXboxStndCntrlDevice | Initialize CXboxStndCntrlDevice class
      //
      //
      //
      //
      CXboxStndCntrlDevice::CXboxStndCntrlDevice(void) : InitCEventHandler(0)
      {
         RWS_FUNCTION("RWS::InputDevices::CXboxStndCntrlDevice::CXboxStndCntrlDevice");

         XBInput_CreateGamepads();
      
         LinkMsg (iMsgRunningTick, 0);
         LinkMsg (iMsgPausedTick, 0);

         RWS_RETURNVOID();
      }
   
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | ~CXboxStndCntrlDevice | Deconstruct CXboxStndCntrlDevice class
      //
      //
      //
      //
      CXboxStndCntrlDevice::~CXboxStndCntrlDevice(void)
      {
         RWS_FUNCTION("RWS::InputDevices::CXboxStndCntrlDevice::~CXboxStndCntrlDevice");

         UnLinkMsg (iMsgPausedTick);
         UnLinkMsg (iMsgRunningTick);

         RWS_RETURNVOID();
      }
   
   
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc HRESULT | XBInput_CreateGamepads | Initialize game pad devices
      //
      //
      //
      //
      //-----------------------------------------------------------------------------
      // Name: XBInput_CreateGamepads()
      // Desc: Creates the game pad devices
      //-----------------------------------------------------------------------------
      HRESULT CXboxStndCntrlDevice::XBInput_CreateGamepads( void )
      {
         RWS_FUNCTION("RWS::InputDevices::CXboxStndCntrlDevice::XBInput_CreateGamepads");
         
         static bool initialized = false;
         
         if (!initialized)
         {
            
            // Initialize Devices
            //
            XInitDevices(0, 0);    
            
            DWORD dwDeviceMask = 0;
            DWORD count = 0;
            
            while (dwDeviceMask == 0)
            {
               RWS_TRACE("CXboxStndCntrlDevice::XBInput_CreateGamepads XGetDevices( XDEVICE_TYPE_GAMEPAD )");
               
               // Sleep while devices are initialized
               //
               Sleep(100); 
               
               // Get a mask of all currently available devices
               //
               dwDeviceMask = XGetDevices( XDEVICE_TYPE_GAMEPAD );
               
               if (dwDeviceMask) break;
               else
               {
                  RWS_TRACE("Not Found :: Retry");
               }
               
               count ++;
               
               if (count >= 100) 
               {
                  RWS_ASSERTFAIL("CXboxStndCntrlDevice::XBInput_CreateGamepads No Game pads Connected");
               }
            };
            
            if (XGetPortCount() > NUM_GAME_CONTROLLERS)
            {
               RWS_ASSERT(false, "XBInput_CreateGamepads XGetPortCount() > NUM_GAME_CONTROLLERS)");
            }
            
            // Open the devices
            //
            for ( DWORD i = 0; i < XGetPortCount(); i++ )
            {
               ZeroMemory( &g_Gamepads[i], sizeof(XBGAMEPAD) );
               if ( dwDeviceMask & (1<<i) ) 
               {
                  // Get a handle to the device
                  //
                  g_Gamepads[i].hDevice = XInputOpen( XDEVICE_TYPE_GAMEPAD, i, XDEVICE_NO_SLOT, 0 );
                  
                  // Store capabilities of the device
                  //
                  XInputGetCapabilities( g_Gamepads[i].hDevice, &g_Gamepads[i].caps );
               }
            }
            
            initialized = true;
         }
         
         RWS_RETURN( S_OK );
      }
   
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | XBInput_GetInput | Process input from game pads
      //
      //
      //
      //
      //-----------------------------------------------------------------------------
      // Name: XBInput_GetInput()
      // Desc: Processes input from the game pads
      //-----------------------------------------------------------------------------
      void CXboxStndCntrlDevice::XBInput_GetInput( void)
      {
         RWS_FUNCTION("RWS::InputDevices::CXboxStndCntrlDevice::XBInput_GetInput");
         
         XBGAMEPAD *pGamepads = g_Gamepads;
         
         // Get status about game pad insertions and removal. Note that, in order to
         // not miss devices, we will check for removed device BEFORE checking for
         // insertions
         //
         DWORD dwInsertions, dwRemovals;
         XGetDeviceChanges( XDEVICE_TYPE_GAMEPAD, &dwInsertions, &dwRemovals );
         
         // Loop through all game pads
         //
         for ( DWORD i = 0; i < XGetPortCount(); i++ )
         {
            // Handle removed devices.
            //
            if ( dwRemovals & (1 << i) ) 
            {
               RWS_TRACE("Pad " << i << " Removed");
               
               XInputClose( pGamepads[i].hDevice );
               pGamepads[i].hDevice = 0;
               
               // FE Add pad remove event
            }
            
            // Handle inserted devices
            //
            pGamepads[i].bInserted = FALSE;
            if ( dwInsertions & (1 << i) ) 
            {
               RWS_TRACE("Pad " << i << " Inserted");
               
               pGamepads[i].hDevice = XInputOpen( XDEVICE_TYPE_GAMEPAD, i, XDEVICE_NO_SLOT, 0 );

               XInputGetCapabilities( g_Gamepads[i].hDevice, &g_Gamepads[i].caps );
               pGamepads[i].bInserted = TRUE;
               
               // FE Add pad inserted event
            }
            
            // If we have a valid device, poll it's state
            if ( pGamepads[i].hDevice )
            {
               XInputGetState( pGamepads[i].hDevice, &pGamepads[i].xiState );
            }
         }
         
         RWS_RETURNVOID();
      }
      
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | GetXBGAMEPAD | Return XBGAMEPAD for a specified device
      //
      //
      //
      //
      XBGAMEPAD *CXboxStndCntrlDevice::GetXBGAMEPAD(unsigned int device_num)
      {
         RWS_FUNCTION("RWS::InputDevices::CXboxStndCntrlDevice::GetXBGAMEPAD");

         if (!_instance) RWS_RETURN(0);
      
         if (device_num < NUM_GAME_CONTROLLERS)
         {
            RWS_RETURN(&g_Gamepads[device_num]);
         }
         else
         {
            RWS_RETURN(0);
         }
      }
   
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | HandleEvents | Process events
      //
      //
      //
      //
      void CXboxStndCntrlDevice::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::InputDevices::CXboxStndCntrlDevice::HandleEvents");

         if (pMsg.Id == iMsgRunningTick) XBInput_GetInput();

         RWS_RETURNVOID();
      }
   
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | CXboxStndController | Initialize controller Input
      //
      //
      //
      //
      CXboxStndController::CXboxStndController(const CAttributePacket&) : InitCEventHandler(0), m_PortSelect(0)
      {
         RWS_FUNCTION("RWS::InputDevices::CXboxStndController::CXboxStndController");

         pCXboxStndCntrlDevice = CXboxStndCntrlDevice::new_Instance();
      
         // Link to the running tick in order to poll controller devices
         //
         LinkMsg (iMsgRunningTick, 0);

         // Defaults
         //
         ReplaceLinkedMsg(m_LJX.m_Inq,  "INQ_ACTN_TURN",   "return RwReal");
         ReplaceLinkedMsg(m_LJY.m_Inq,  "INQ_ACTN_FWD",    "return RwReal");
         ReplaceLinkedMsg(m_RJX.m_Inq,  "INQ_ACTN_STRAFE", "return RwReal");
         ReplaceLinkedMsg(m_RJY.m_Inq,  "INQ_CAMERA_PITCH","return RwReal");
         ReplaceLinkedMsg(m_B[0].m_Inq, "INQ_ACTN_JUMP",   "return RwReal");
         ReplaceLinkedMsg(m_B[1].m_Inq, "INQ_ACTN_FIRE",   "return RwReal");

         RWS_RETURNVOID();
      }
   
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | ~CXboxStndController | Deconstruct controller Input
      //
      //
      //
      //
      CXboxStndController::~CXboxStndController()
      {
         RWS_FUNCTION("RWS::InputDevices::CXboxStndController::~CXboxStndController");

         UnLinkMsg (m_LJX.m_Inq);
         UnRegisterMsg (m_LJX.m_Inq);
      
         UnLinkMsg (m_LJY.m_Inq);
         UnRegisterMsg (m_LJY.m_Inq);
      
         UnLinkMsg (m_RJX.m_Inq);
         UnRegisterMsg (m_RJX.m_Inq);
      
         UnLinkMsg (m_RJY.m_Inq);
         UnRegisterMsg (m_RJY.m_Inq);
      
         for (int i = 0; i < 8; i++)
         {
            UnLinkMsg (m_B[i].m_Inq);
            UnRegisterMsg (m_B[i].m_Inq);
         
            UnLinkMsg (m_wB[i].m_Inq);
            UnRegisterMsg (m_wB[i].m_Inq);
         }
      
         UnLinkMsg (m_BJX.m_Inq);
         UnRegisterMsg (m_BJX.m_Inq);
      
         UnLinkMsg (m_BJY.m_Inq);
         UnRegisterMsg (m_BJY.m_Inq);
      
         UnLinkMsg(iMsgRunningTick);
         // Attempt to shutdown the device driver
         //
         CXboxStndCntrlDevice::delete_Instance();  

         RWS_RETURNVOID();
      }
   
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | HandleAttributes | Process Parameters
      //
      //
      //
      //
      void CXboxStndController::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::InputDevices::CXboxStndController::HandleAttributes");
      
         CAttributeHandler::HandleAttributes(attr);

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CXboxStndController));
      
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               //////////////////////////////////////////////////////////////////
            case CMD_m_PortSelect:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_PortSelect);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_LJX:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_LJX.m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_LJY:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_LJY.m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_RJX:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_RJX.m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_RJY:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_RJY.m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_B1:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_B[0].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_B2:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_B[1].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_B3:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_B[2].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_B4:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_B[3].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_B5:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_B[4].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_B6:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_B[5].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_B7:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_B[6].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_B8:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_B[7].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_wB1:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_wB[0].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_wB2:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_wB[1].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_wB3:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_wB[2].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_wB4:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_wB[3].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_wB5:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_wB[4].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_wB6:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_wB[5].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_wB7:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_wB[6].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_wB8:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_wB[7].m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_BJX:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_BJX.m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_BJY:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_BJY.m_Inq, attrIt->GetAs_RwChar_ptr(), "return RwReal");
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
      void CXboxStndController::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::InputDevices::CXboxStndController::HandleEvents");

         if (pMsg.Id == iMsgRunningTick)
         {
            XBGAMEPAD* pGP = CXboxStndCntrlDevice::GetXBGAMEPAD(m_PortSelect);   // Pointer to Game Pad
         
            // Display Debug Stuff
            //
   #ifdef RWS_DEBUGTOOLS
   #if 0
            if (g_GFDebugTools)
            {
               g_GFDebugTools->Printf ("wButtons 0x%x\n", (unsigned int)pGP->xiState.Gamepad.wButtons);
               g_GFDebugTools->Printf ("bAnalogButtons 0x%x\n", (unsigned int)pGP->xiState.Gamepad.bAnalogButtons[0]);
               g_GFDebugTools->Printf ("bAnalogButtons 0x%x\n", (unsigned int)pGP->xiState.Gamepad.bAnalogButtons[1]);
               g_GFDebugTools->Printf ("bAnalogButtons 0x%x\n", (unsigned int)pGP->xiState.Gamepad.bAnalogButtons[2]);
               g_GFDebugTools->Printf ("bAnalogButtons 0x%x\n", (unsigned int)pGP->xiState.Gamepad.bAnalogButtons[3]);
               g_GFDebugTools->Printf ("bAnalogButtons 0x%x\n", (unsigned int)pGP->xiState.Gamepad.bAnalogButtons[4]);
               g_GFDebugTools->Printf ("bAnalogButtons 0x%x\n", (unsigned int)pGP->xiState.Gamepad.bAnalogButtons[5]);
               g_GFDebugTools->Printf ("bAnalogButtons 0x%x\n", (unsigned int)pGP->xiState.Gamepad.bAnalogButtons[6]);
               g_GFDebugTools->Printf ("bAnalogButtons 0x%x\n", (unsigned int)pGP->xiState.Gamepad.bAnalogButtons[7]);
               g_GFDebugTools->Printf ("sThumbLX 0x%x\n", (unsigned int)pGP->xiState.Gamepad.sThumbLX);
               g_GFDebugTools->Printf ("sThumbLY 0x%x\n", (unsigned int)pGP->xiState.Gamepad.sThumbLY);
               g_GFDebugTools->Printf ("sThumbRX 0x%x\n", (unsigned int)pGP->xiState.Gamepad.sThumbRX);
               g_GFDebugTools->Printf ("sThumbRY 0x%x\n", (unsigned int)pGP->xiState.Gamepad.sThumbRY);
            }
   #endif
   #endif
            // Convert raw button values to -1 to 1
            //
            static unsigned int mask_table[] =
            {
               XINPUT_GAMEPAD_DPAD_UP,
                  XINPUT_GAMEPAD_DPAD_DOWN,
                  XINPUT_GAMEPAD_DPAD_LEFT,
                  XINPUT_GAMEPAD_DPAD_RIGHT,
                  XINPUT_GAMEPAD_START,
                  XINPUT_GAMEPAD_BACK,
                  XINPUT_GAMEPAD_LEFT_THUMB,
                  XINPUT_GAMEPAD_RIGHT_THUMB
            };
          
            // Convert raw joystick values to -1 to 1
            //
            m_LJX.CalcJoystickValue (pGP->xiState.Gamepad.sThumbLX);
            m_LJY.CalcJoystickValueNeg (pGP->xiState.Gamepad.sThumbLY);
            m_RJX.CalcJoystickValue (pGP->xiState.Gamepad.sThumbRX);
            m_RJY.CalcJoystickValueNeg (pGP->xiState.Gamepad.sThumbRY);

            m_BJX.CalcJoystickValue(pGP->xiState.Gamepad.wButtons ,
               XINPUT_GAMEPAD_DPAD_RIGHT, XINPUT_GAMEPAD_DPAD_LEFT);

            m_BJY.CalcJoystickValue(pGP->xiState.Gamepad.wButtons, 
               XINPUT_GAMEPAD_DPAD_UP, XINPUT_GAMEPAD_DPAD_DOWN);
         
            for (unsigned int i = 0;i < 8;i++)
            {
               // Analogue Buttons
               //
               m_B[i].CalcAnalogueButtonValue (pGP->xiState.Gamepad.bAnalogButtons[i]);
            
               // Digital Buttons
               //
               m_wB[i].CalcButtonValue (pGP->xiState.Gamepad.wButtons, mask_table[i]);
            }
         }
      
         // Check For Inquire Joysticks
         //
         else if (pMsg.Id == m_LJX.m_Inq) m_LJX.SendInputValue(&pMsg);
         else if (pMsg.Id == m_LJY.m_Inq) m_LJY.SendInputValue(&pMsg);
         else if (pMsg.Id == m_RJX.m_Inq) m_RJX.SendInputValue(&pMsg);
         else if (pMsg.Id == m_RJY.m_Inq) m_RJY.SendInputValue(&pMsg);
      
         // Digital Joystick
         //
         else if (pMsg.Id == m_BJX.m_Inq) m_BJX.SendInputValue(&pMsg);
         else if (pMsg.Id == m_BJY.m_Inq) m_BJY.SendInputValue(&pMsg);
      
         //Check for Inquire Buttons...
         //
         else
         {
            for (unsigned int i = 0; i < 8; i++)
            {
               // Analogue Buttons
               //
               if (pMsg.Id == m_B[i].m_Inq) {m_B[i].SendInputValue(&pMsg);break;}
               // Digital Buttons
               //
               if (pMsg.Id == m_wB[i].m_Inq) {m_wB[i].SendInputValue(&pMsg);break;}
            }
         }

         RWS_RETURNVOID();
      }
   }//namespace InputDevices
}//namespace RWS

#endif
