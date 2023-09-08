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
#include "precomp.h"

//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cwin32keyboard.h"

#include "framework/core/macros/debugmacros.h"
#include "framework/startup/win32/win.h"

#if defined(_WINDOWS) || defined(DOXYGEN)

//////////////////////////////////////////////////////////////////
//
// System Includes
//
#include <windows.h>

namespace RWS
{
   namespace InputDevices
   {
      RWS_IMPLEMENT_CLASSID(CWin32Keyboard);
      RWS_REGISTERCLASS(CWin32Keyboard);
   
      const unsigned int _string2vkcode_tble_[] = {
         {VK_LBUTTON}, //01Leftmousebutton
         {VK_RBUTTON}, //02Rightmousebutton
         {VK_CANCEL}, //03Control-breakprocessing
         {VK_MBUTTON}, //04Middlemousebutton(three-buttonmouse)
         {VK_BACK}, //08backspacekey
         {VK_TAB}, //09tabkey
         {VK_CLEAR}, //0Cclearkey
         {VK_RETURN}, //0Denterkey
         {VK_SHIFT}, //10shiftkey
         {VK_CONTROL}, //11ctrlkey
         {VK_MENU}, //12altkey
         {VK_PAUSE}, //13pausekey
         {VK_CAPITAL}, //14capslockkey
         {VK_ESCAPE}, //1Besckey
         {VK_SPACE}, //20spacebar
         {VK_PRIOR}, //21pageupkey
         {VK_NEXT}, //22pagedownkey
         {VK_END}, //23endkey
         {VK_HOME}, //24homekey
         {VK_LEFT}, //25leftarrowkey
         {VK_UP}, //26uparrowkey
         {VK_RIGHT}, //27rightarrowkey
         {VK_DOWN}, //28downarrowkey
         {VK_SELECT}, //29selectkey
         {VK_EXECUTE}, //2Bexecutekey
         {VK_SNAPSHOT}, //2Cprintscreenkey
         {VK_INSERT}, //2Dinskey
         {VK_DELETE}, //2Edelkey
         {VK_HELP}, //2Fhelpkey
         {0x30}, //300key
         {0x31}, //311key
         {0x32}, //322key
         {0x33}, //333key
         {0x34}, //344key
         {0x35}, //355key
         {0x36}, //366key
         {0x37}, //377key
         {0x38}, //388key
         {0x39}, //399key
         {0x41}, //41akey
         {0x42}, //42bkey
         {0x43}, //43ckey
         {0x44}, //44dkey
         {0x45}, //45ekey
         {0x46}, //46fkey
         {0x47}, //47gkey
         {0x48}, //48hkey
         {0x49}, //49ikey
         {0x4a}, //4Ajkey
         {0x4b}, //4Bkkey
         {0x4c}, //4Clkey
         {0x4d}, //4Dmkey
         {0x4e}, //4Enkey
         {0x4f}, //4Fokey
         {0x50}, //50pkey
         {0x51}, //51qkey
         {0x52}, //52rkey
         {0x53}, //53skey
         {0x54}, //54tkey
         {0x55}, //55ukey
         {0x56}, //56vkey
         {0x57}, //57wkey
         {0x58}, //58xkey
         {0x59}, //59ykey
         {0x5a}, //5Azkey
         {VK_LWIN}, //5BLeftWindowskey(MicrosoftNaturalKeyboard)
         {VK_RWIN}, //5CRightWindowskey(MicrosoftNaturalKeyboard)
         {VK_APPS}, //5DApplicationskey(MicrosoftNaturalKeyboard)
         {VK_NUMPAD0}, //60Numerickeypad0key
         {VK_NUMPAD1}, //61Numerickeypad1key
         {VK_NUMPAD2}, //62Numerickeypad2key
         {VK_NUMPAD3}, //63Numerickeypad3key
         {VK_NUMPAD4}, //64Numerickeypad4key
         {VK_NUMPAD5}, //65Numerickeypad5key
         {VK_NUMPAD6}, //66Numerickeypad6key
         {VK_NUMPAD7}, //67Numerickeypad7key
         {VK_NUMPAD8}, //68Numerickeypad8key
         {VK_NUMPAD9}, //69Numerickeypad9key
         {VK_MULTIPLY}, //6AMultiplykey
         {VK_ADD}, //6BAddkey
         {VK_SEPARATOR}, //6CSeparatorkey
         {VK_SUBTRACT}, //6DSubtractkey
         {VK_DECIMAL}, //6EDecimalkey
         {VK_DIVIDE}, //6FDividekey
         {VK_F1}, //70f1key
         {VK_F2}, //71f2key
         {VK_F3}, //72f3key
         {VK_F4}, //73f4key
         {VK_F5}, //74f5key
         {VK_F6}, //75f6key
         {VK_F7}, //76f7key
         {VK_F8}, //77f8key
         {VK_F9}, //78f9key
         {VK_F10}, //79f10key
         {VK_F11}, //7Af11key
         {VK_F12}, //7Bf12key
         {VK_F13}, //7Cf13key
         {VK_F14}, //7Df14key
         {VK_F15}, //7Ef15key
         {VK_F16}, //7Ff16key
         {VK_F17}, //80Hf17key
         {VK_F18}, //81Hf18key
         {VK_F19}, //82Hf19key
         {VK_F20}, //83Hf20key
         {VK_F21}, //84Hf21key
         {VK_F22}, //85Hf22key
         {VK_F23}, //86Hf23key
         {VK_F24}, //87Hf24key
         {VK_NUMLOCK}, //90numlockkey
         {VK_SCROLL}, //91scrolllockkey
         {VK_ATTN}, //F6Attnkey
         {VK_CRSEL}, //F7CrSelkey
         {VK_EXSEL}, //F8ExSelkey
         {VK_EREOF}, //F9EraseEOFkey
         {VK_PLAY}, //FAPlaykey
         {VK_ZOOM}, //FBZoomkey
         {VK_NONAME}, //FCReservedforfutureuse.
         {VK_PA1}, //FDPA1key
         {VK_OEM_CLEAR} //FEClearkey
      };
   
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | CWin32Keyboard | Default Initialization for CWin32Keyboard class
      //
      //
      //
      //
      CWin32Keyboard::CWin32Keyboard(const CAttributePacket&): 
         InitCEventHandler(0),
         m_input_val(0.0f),
         vKey1(0),
         vKey2(0)
      {

         RWS_FUNCTION("RWS::InputDevices::CWin32Keyboard::CWin32Keyboard");
            
         LinkMsg(iMsgRunningTick, 0);   // Check Key state when game is running
         LinkMsg(iMsgPausedTick, 0); // Check Key state when game is paused

         RWS_RETURNVOID();
      }
   
      CWin32Keyboard::~CWin32Keyboard(void)
      {
         RWS_FUNCTION("RWS::InputDevices::CWin32Keyboard::~CWin32Keyboard");

         UnLinkMsg(iMsgRunningTick);
         UnLinkMsg(iMsgPausedTick);
      
         UnLinkMsg(m_InqKey_Msg_Id);
         UnRegisterMsg(m_InqKey_Msg_Id);

         RWS_RETURNVOID();
      }
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | HandleEvents | Process events
      //
      //
      //
      //
      void CWin32Keyboard::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::InputDevices::CWin32Keyboard::HandleEvents");

         // Process running tick message
         //
         if (pMsg.Id == m_InqKey_Msg_Id)
         {
            *(RwReal*)(&pMsg.pData) = m_input_val;
         
            RWS_RETURNVOID();
         }
      
         if (pMsg.Id == iMsgRunningTick)
         {
            bool hasFocus = (Win::GetMainWindow() == GetFocus());
            RwReal last_input_val = m_input_val;
         
            if ( vKey1 != vKey2 )   // Key Pair 1..0..-1 Emulates a Joystick
            {
               if (hasFocus)
               {               
                   SHORT keystate1 = GetAsyncKeyState(vKey1);

                   if ( (keystate1&0x00008000) != 0) m_input_val = 1.0f;
                   else                              m_input_val = 0.0f;
            
                   SHORT keystate2 = GetAsyncKeyState(vKey2);
            
                   if ( (keystate2&0x00008000) != 0) m_input_val -= 1.0f;
                   else                              ;
               }
               else
               {
                   m_input_val = 0.0f;
               }
            }
            else  // 1..-1 Emulates a single button
            {
               if (hasFocus)
               {               
                   SHORT keystate1 = GetAsyncKeyState(vKey1);

                   if ( (keystate1&0x00008000) != 0) m_input_val = 1.0f;
                   else                              m_input_val = -1.0f;
               }
               else
               {
                   m_input_val = -1.0f;
               }
            }
            RWS_RETURNVOID();
         }
         RWS_RETURNVOID();
      }
   
      //////////////////////////////////////////////////////////////////
      // 
      // @mfunc void | HandleAttributes | Load Parameters from RenderWare Studio
      //
      //
      //
      //
      void CWin32Keyboard::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::InputDevices::CWin32Keyboard::HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CWin32Keyboard));
      
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               //////////////////////////////////////////////////////////////////
            case CMD_vKey1:
               //////////////////////////////////////////////////////////////////
               vKey1 = _string2vkcode_tble_[attrIt->GetAs_unsigned_int()];
               break;
            
               //////////////////////////////////////////////////////////////////
            case CMD_vKey2:
               //////////////////////////////////////////////////////////////////
               vKey2 = _string2vkcode_tble_[attrIt->GetAs_unsigned_int()];
               break;

               //////////////////////////////////////////////////////////////////
            case CMD_m_InqKey_Msg_Id:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_InqKey_Msg_Id, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
            }
            ++attrIt;
         }
         RWS_RETURNVOID();
      }
   }//namespace InputDevices
}//namespace RWS

#endif
