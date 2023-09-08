/**********************************************************************
 *
 * File :     CWin32Keyboard.h
 *
 * Abstract : Win32 keyboard handler.
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

#ifndef __CWIN32KEYBOARD_H__
#define __CWIN32KEYBOARD_H__

#if defined(DOXYGEN) || defined(_WINDOWS)

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/mainloop/logic.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace InputDevices
   {
      /**
      *
      *  \ingroup Mod_InputDevices
      *
      *  Behaviour class that reads input from a windows keyboard, if key1 and key2 are the same then the key
      *  is treated as a button i.e. either off or on -1 or 1, if key1 and keys are different then the keys
      *  are treated as a joystick fwd/middle/back -1 0 or 1.
      *
      */
      class CWin32Keyboard : public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(CWin32Keyboard);
         RWS_DECLARE_CLASSID(CWin32Keyboard);
         RWS_CATEGORY("InputDevices");
         RWS_DESCRIPTION("Basic Win32 Key Handler", "Key handler, by using 1 or 2 keys can emulate a button "
                         "(1 or -1) or joystick (1 to -1)");

         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);

         // These Macro's define the interface between this entity and RenderWare Studio
         //
         RWS_BEGIN_COMMANDS

            RWS_ATTRIBUTE(CMD_vKey1,
                          "Key 1",            
                          "If you want this key to act as a button then set it to the same value as key 2,  "
                          "otherwise set to an opposite,  e.g. key 1 = VK_UP and key 2 = VK_DOWN.  This will act "
                          "as a 2 state switch.",
                          LIST,    
                          RwUInt32,   
                          LIST("VK_LBUTTON|VK_RBUTTON|VK_CANCEL|VK_MBUTTON|VK_BACK|VK_TAB|VK_CLEAR|VK_RETURN|"
                          "VK_SHIFT|VK_CONTROL|VK_MENU|VK_PAUSE|VK_CAPITAL|VK_ESCAPE|VK_SPACE|VK_PRIOR|VK_NEXT|"
                          "VK_END|VK_HOME|VK_LEFT|VK_UP|VK_RIGHT|VK_DOWN|VK_SELECT|VK_EXECUTE|VK_SNAPSHOT|VK_INSERT|"
                          "VK_DELETE|VK_HELP|VK_0|VK_1|VK_2|VK_3|VK_4|VK_5|VK_6|VK_7|VK_8|VK_9|VK_A|VK_B|VK_C|VK_D|"
                          "VK_E|VK_F|VK_G|VK_H|VK_I|VK_J|VK_K|VK_L|VK_M|VK_N|VK_O|VK_P|VK_Q|VK_R|VK_S|VK_T|VK_U|VK_V|"
                          "VK_W|VK_X|VK_Y|VK_Z"))

            RWS_ATTRIBUTE(CMD_vKey2,                 
                         "Key 2",            
                         "If you want this key to act as a button then set it to the same value as key 1,  "
                         "otherwise set to an opposite,  e.g. key 1 = VK_UP and key 2 = VK_DOWN.  This will "
                         "act as a 2 state switch.",
                         LIST,    
                         RwUInt32,   
                         LIST("VK_LBUTTON|VK_RBUTTON|VK_CANCEL|VK_MBUTTON|VK_BACK|VK_TAB|VK_CLEAR|VK_RETURN|VK_SHIFT|"
                         "VK_CONTROL|VK_MENU|VK_PAUSE|VK_CAPITAL|VK_ESCAPE|VK_SPACE|VK_PRIOR|VK_NEXT|VK_END|VK_HOME|"
                         "VK_LEFT|VK_UP|VK_RIGHT|VK_DOWN|VK_SELECT|VK_EXECUTE|VK_SNAPSHOT|VK_INSERT|VK_DELETE|"
                         "VK_HELP|VK_0|VK_1|VK_2|VK_3|VK_4|VK_5|VK_6|VK_7|VK_8|VK_9|VK_A|VK_B|VK_C|VK_D|VK_E|VK_F|"
                         "VK_G|VK_H|VK_I|VK_J|VK_K|VK_L|VK_M|VK_N|VK_O|VK_P|VK_Q|VK_R|VK_S|VK_T|VK_U|VK_V|VK_W|"
                         "VK_X|VK_Y|VK_Z"))

            RWS_MESSAGE( CMD_m_InqKey_Msg_Id,
                     "Inquire Input",
                     "return's +-1.0 depending on state of this input device",
                     RECEIVE,
                     return RwReal,
                     0)

         RWS_END_COMMANDS;

         CWin32Keyboard(const CAttributePacket&);

         ~CWin32Keyboard();

      private:
         RwReal m_input_val;  // Input value

         RwUInt32 vKey1;   // If these are a button pair vKey1 != vKey2 otherwise they do
         RwUInt32 vKey2;

         CEventId m_InqKey_Msg_Id;        // Inquire Key State Message 0..0.5..1
      };
   }//namespace InputDevices
}//namespace RWS
#else
#pragma message ("CWin32Keyboard: _WINDOWS only")
#endif
#endif

