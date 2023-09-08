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

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include "precomp.h"


#if defined(DOXYGEN) || defined(SKY)

//////////////////////////////////////////////////////////////////
//
//  ps2_controller.cpp
//
//  © 2001
//
//  @doc
//
// Notes: Need to be able to represent a joystick as +1 to -1 and a buton pair as +1 to -1
//
// Create two types of keyboard reader a +1 to -1 for button pairs and a +1 to -1 for buttons,
// which version is used is specified by the tvKey1 and tvKey2 parameters to the CKeyboardReader
// constructor, if they are the same then it is a single button interface.
//
#include "cpsx2controller.h"
#include "framework/core/macros/debugmacros.h"

namespace
{
   const unsigned int kPadStabDelay = 50;
   const int kPadNormal = 4;
   const int kPadAnalog = 7;

   //////////////////////////////////////////////////////////////////
   //
   bool skyPadCanBeRead(int port, int slot)
   {
      const int padState = scePadGetState(port, slot);
      return scePadStateStable == padState || scePadStateFindCTP1 == padState;
   }

   //////////////////////////////////////////////////////////////////
   //
   void skyPadSetAnalogMode(int port, int slot)
   {
      // Wait until the pad is stable (subject to a timeout)...
      bool wait = true;
      bool stable = false;
      unsigned int nbStabFrame = 0;
      while (!stable && wait)
      {
         int state = scePadGetState(port, slot);
         ++nbStabFrame;
         
         stable = scePadStateStable == state || scePadStateFindCTP1 == state;
         wait = scePadStateExecCmd == state || nbStabFrame < kPadStabDelay;
      }

      // Did the pad get stable?
      if (stable)
      {
         // Get the id of the pad
         int id = scePadInfoMode(port, slot, InfoModeCurID, 0);
         int exid = scePadInfoMode(port, slot, InfoModeCurExID, 0);
         if (exid > 0) id = exid;

         switch (id)
         {
            case kPadNormal:      // Standard

               // Check the pad supports analogue/vibration functionality...
               if (scePadInfoMode(port, slot, InfoModeCurExID, 0) != 0)
               {
                  // Change to analogue mode and lock...
                  do
                  {
                    scePadSetMainMode(port, slot, 1, 1);
                  }
                  while (scePadGetReqState(port, slot) == scePadReqStateFaild);
               }

               break;

            case kPadAnalog:       // Analog
            default:              // Unknown

               // In both cases, leave as-is
               break;
         }
      }
   }
}

namespace RWS
{
   namespace InputDevices
   {
      RWS_IMPLEMENT_CLASSID(CPSX2StndController);
      RWS_REGISTERCLASS(CPSX2StndController);
   
      static u_long128 padDma[scePadDmaBufferMax] __attribute__((aligned (64)));
   
      static unsigned int usage_refcount = 0;
   
      //////////////////////////////////////////////////////////////////
      //
      CPSX2StndController::CPSX2StndController(const CAttributePacket&) : 
         InitCEventHandler(0),
         m_DeadZone(0.25),
         m_PortSelect(0)
      {
            
         RWS_FUNCTION("RWS::InputDevices::CPSX2StndController::CPSX2StndController");

         if (usage_refcount == 0)
         {
            // Init the controllers
            //
            scePadInit (0);    
         }

         usage_refcount ++;
      
         // Open the controller port and set it to analog
         //
         scePadPortOpen (0, 0, padDma);
         skyPadSetAnalogMode(0, 0);
      
         // Do an initial read
         //
         ReadInput();
      
         LinkMsg( iMsgRunningTick );

         ReplaceLinkedMsg(m_Inq_Triangle, "INQ_ACTN_JUMP", "return RwReal");
         ReplaceLinkedMsg(m_Inq_LR1, "INQ_ACTN_STRAFE", "return RwReal");
         ReplaceLinkedMsg(m_LPAD_X, "INQ_ACTN_TURN", "return RwReal");
         ReplaceLinkedMsg(m_LPAD_Y, "INQ_ACTN_FWD", "return RwReal");
         ReplaceLinkedMsg(m_RPAD_Y, "INQ_CAMERA_PITCH", "return RwReal");
         ReplaceLinkedMsg(m_Inq_Cross, "INQ_ACTN_FIRE", "return RwReal");

         RWS_RETURNVOID();
      }
   
      //////////////////////////////////////////////////////////////////
      //
      CPSX2StndController::~CPSX2StndController()
      {
         RWS_FUNCTION("RWS::InputDevices::CPSX2StndController::~CPSX2StndController");

         // Close the controller port
         //
         scePadPortClose (0, 0 );

         usage_refcount --;

         // Shutdown controllers
         //
         if (usage_refcount == 0)
         {
            scePadEnd ();
         }

         UnLinkMsg( iMsgRunningTick );
      
         UnLinkMsg( m_Inq_Start );
         UnLinkMsg( m_Inq_Select );
         UnLinkMsg( m_Inq_Triangle );
         UnLinkMsg( m_Inq_Cross );
         UnLinkMsg( m_Inq_Square );
         UnLinkMsg( m_Inq_Circle );
         UnLinkMsg( m_Inq_PAD_Up );
         UnLinkMsg( m_Inq_PAD_Down );
         UnLinkMsg( m_Inq_PAD_Left );
         UnLinkMsg( m_Inq_PAD_Right );
         UnLinkMsg( m_Inq_DPAD_X );
         UnLinkMsg( m_Inq_DPAD_Y );
         UnLinkMsg( m_Inq_L1 );
         UnLinkMsg( m_Inq_L2 );
         UnLinkMsg( m_Inq_R1 );
         UnLinkMsg( m_Inq_R2 );
         UnLinkMsg( m_LPAD_X );
         UnLinkMsg( m_LPAD_Y );
         UnLinkMsg( m_RPAD_X );
         UnLinkMsg( m_RPAD_Y );
         UnLinkMsg( m_Inq_LR1 );
      
         UnRegisterMsg( m_Inq_Start );
         UnRegisterMsg( m_Inq_Select );
         UnRegisterMsg( m_Inq_Triangle );
         UnRegisterMsg( m_Inq_Cross );
         UnRegisterMsg( m_Inq_Square );
         UnRegisterMsg( m_Inq_Circle );
         UnRegisterMsg( m_Inq_PAD_Up );
         UnRegisterMsg( m_Inq_PAD_Down );
         UnRegisterMsg( m_Inq_PAD_Left );
         UnRegisterMsg( m_Inq_PAD_Right );
         UnRegisterMsg( m_Inq_DPAD_X );
         UnRegisterMsg( m_Inq_DPAD_Y );
         UnRegisterMsg( m_Inq_L1 );
         UnRegisterMsg( m_Inq_L2 );
         UnRegisterMsg( m_Inq_R1 );
         UnRegisterMsg( m_Inq_R2 );
         UnRegisterMsg( m_LPAD_X );
         UnRegisterMsg( m_LPAD_Y );
         UnRegisterMsg( m_RPAD_X );
         UnRegisterMsg( m_RPAD_Y );
         UnRegisterMsg( m_Inq_LR1 );

         RWS_RETURNVOID();
      }
   
      void CPSX2StndController::CalcJoyVar(float &joyvar)
      {
         RWS_FUNCTION("RWS::InputDevices::CPSX2StndController::CalcJoyVar");

         joyvar = (128.0f - joyvar) / 128.0f;

         // Check for deadzone, prevents drift when joystick is near center position
         //
         if (joyvar < m_DeadZone && joyvar> - m_DeadZone)
         {
            joyvar = 0.0f;
         }
         // Make non-linear : Improves feel of controls makes them less twitchy near
         // center position.
         //
         else if (joyvar>0)
         {
            joyvar = (joyvar*joyvar);
         }
         else
         {
            joyvar = - (joyvar*joyvar);
         }

         RWS_RETURNVOID();
      }   
      //////////////////////////////////////////////////////////////////
      //
      void CPSX2StndController::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::InputDevices::CPSX2StndController::HandleEvents");

         if (pMsg.Id == iMsgRunningTick)
         {
            ReadInput();
         }
         else
         {
            if (pMsg.Id == m_Inq_Start )
            {
               if (paddata&SCE_PADstart) { *(float*)(&pMsg.pData) = 1.0f; }
               else                      { *(float*)(&pMsg.pData) = -1.0f; }
            }
            else if (pMsg.Id == m_Inq_Select )
            {
               if (paddata&SCE_PADselect) { *(float*)(&pMsg.pData) = 1.0f; }
               else                       { *(float*)(&pMsg.pData) = -1.0f; }
            }
            else if (pMsg.Id == m_Inq_Triangle )
            {
               if (paddata&SCE_PADRup) { *(float*)(&pMsg.pData) = 1.0f; }
               else                    { *(float*)(&pMsg.pData) = -1.0f; }
            }
            else if (pMsg.Id == m_Inq_Cross )
            {
               if (paddata&SCE_PADRdown) { *(float*)(&pMsg.pData) = 1.0f; }
               else                      { *(float*)(&pMsg.pData) = -1.0f; }
            }
            else if (pMsg.Id == m_Inq_Square )
            {
               if (paddata&SCE_PADRleft) { *(float*)(&pMsg.pData) = 1.0f; }
               else                      { *(float*)(&pMsg.pData) = -1.0f; }
            }
            else if (pMsg.Id == m_Inq_Circle )
            {
               if (paddata&SCE_PADRright) { *(float*)(&pMsg.pData) = 1.0f; }
               else                       { *(float*)(&pMsg.pData) = -1.0f; }
            }
            else if (pMsg.Id == m_Inq_PAD_Up )
            {
               if (paddata&SCE_PADLup)   { *(float*)(&pMsg.pData) = 1.0f; }
               else                      { *(float*)(&pMsg.pData) = -1.0f; }
            }
            else if (pMsg.Id == m_Inq_PAD_Down )
            {
               if (paddata&SCE_PADLdown)   { *(float*)(&pMsg.pData) = 1.0f; }
               else                      { *(float*)(&pMsg.pData) = -1.0f; }
            }
            else if (pMsg.Id == m_Inq_PAD_Left )
            {
               if (paddata&SCE_PADLleft)   { *(float*)(&pMsg.pData) = 1.0f; }
               else                      { *(float*)(&pMsg.pData) = -1.0f; }
            }
            else if (pMsg.Id == m_Inq_PAD_Right )
            {
               if (paddata&SCE_PADLright)   { *(float*)(&pMsg.pData) = 1.0f; }
               else                      { *(float*)(&pMsg.pData) = -1.0f; }
            }
            else if (pMsg.Id == m_Inq_DPAD_X )
            {
               float joyvar = 0.0f;
            
               if (paddata&SCE_PADLright)   { joyvar = 1.0f; }
               if (paddata&SCE_PADLleft)   { joyvar -= 1.0f; }
            
               *(float*)(&pMsg.pData) = joyvar;
            }
            else if (pMsg.Id == m_Inq_DPAD_Y )
            {
               float joyvar = 0.0f;
            
               if (paddata&SCE_PADLup)   { joyvar = 1.0f; }
               if (paddata&SCE_PADLdown)   { joyvar -= 1.0f; }
            
               *(float*)(&pMsg.pData) = joyvar;
            }
            else if (pMsg.Id == m_Inq_LR1 )
            {
               float joyvar = 0.0f;
            
               if (paddata&SCE_PADL1)   { joyvar = 1.0f; }
               if (paddata&SCE_PADR1)   { joyvar -= 1.0f; }
            
               *(float*)(&pMsg.pData) = joyvar;
            }
            else if (pMsg.Id == m_Inq_LR2 )
            {
               float joyvar = 0.0f;
            
               if (paddata&SCE_PADL2)   { joyvar = 1.0f; }
               if (paddata&SCE_PADR2)   { joyvar -= 1.0f; }
            
               *(float*)(&pMsg.pData) = joyvar;
            }
            else if (pMsg.Id == m_Inq_L1 )
            {
               if (paddata&SCE_PADL1)   { *(float*)(&pMsg.pData) = 1.0f; }
               else                     { *(float*)(&pMsg.pData) = -1.0f; }
            }
            else if (pMsg.Id == m_Inq_L2 )
            {
               if (paddata&SCE_PADL2)   { *(float*)(&pMsg.pData) = 1.0f; }
               else                     { *(float*)(&pMsg.pData) = -1.0f; }
            }
            else if (pMsg.Id == m_Inq_R1 )
            {
               if (paddata&SCE_PADR1)   { *(float*)(&pMsg.pData) = 1.0f; }
               else                    { *(float*)(&pMsg.pData) = -1.0f; }
            }
            else if (pMsg.Id == m_Inq_R2 )
            {
               if (paddata&SCE_PADR2)   { *(float*)(&pMsg.pData) = 1.0f; }
               else                     { *(float*)(&pMsg.pData) = -1.0f; }
            }
            else if (pMsg.Id == m_LPAD_X )
            {
               float joyvar = (float)rdata[6];
            
               CalcJoyVar(joyvar);
            
               *(float*)(&pMsg.pData) = joyvar;
            }
            else if (pMsg.Id == m_LPAD_Y )
            {
               float joyvar = (float)rdata[7];
            
               CalcJoyVar(joyvar);
            
               *(float*)(&pMsg.pData) = joyvar;
            }
            else if (pMsg.Id == m_RPAD_X )
            {
               float joyvar = (float)rdata[4];
            
               CalcJoyVar(joyvar);

               *(float*)(&pMsg.pData) = joyvar;
            }
            else if (pMsg.Id == m_RPAD_Y )
            {
               float joyvar = (float)rdata[5];
            
               CalcJoyVar(joyvar);
            
               *(float*)(&pMsg.pData) = joyvar;
            }
         }

         RWS_RETURNVOID();
      }  
   
      //////////////////////////////////////////////////////////////////
      //
      void CPSX2StndController::ReadInput()
      {
         RWS_FUNCTION("RWS::InputDevices::CPSX2StndController::ReadInput");

         if ( skyPadCanBeRead(0, 0) && scePadRead(0, 0, rdata) > 0 )
         {
            paddata = 0xffff ^ ((((u_int)rdata[2]) << 8) | (u_int)rdata[3]);
         }
         else
         {
            // No input was read - assume centered analogs
            rdata[4] = 128;
            rdata[5] = 128;
            rdata[6] = 128;
            rdata[7] = 128;
            paddata = 0;
         }

         RWS_RETURNVOID();
      }
   
      //////////////////////////////////////////////////////////////////
      //
      void CPSX2StndController::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::InputDevices::CPSX2StndController::HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);

         CAttributeCommandIterator attrIt (attr, RWS_CLASSID_OF(CPSX2StndController));
      
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
            case CMD_m_SlotSelect:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_SlotSelect);
            break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_Start:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg (m_Inq_Start, attrIt->GetAs_RwChar_ptr(), "return RwReal");
            break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_DeadZone:
               //////////////////////////////////////////////////////////////////
               attrIt->GetCommandData(m_DeadZone);
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_Select:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg (m_Inq_Select, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_Triangle:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg (m_Inq_Triangle, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_Cross:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg (m_Inq_Cross, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break; 
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_Square:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg (m_Inq_Square, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_Circle:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_Inq_Circle, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_PAD_Up:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_Inq_PAD_Up, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_PAD_Down:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_Inq_PAD_Down, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_PAD_Left:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_Inq_PAD_Left, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_PAD_Right:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_Inq_PAD_Right, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_DPAD_Y:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_Inq_DPAD_Y, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_DPAD_X:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_Inq_DPAD_X, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_LR1:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_Inq_LR1, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
            case CMD_m_Inq_LR2:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_Inq_LR2, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_L1:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_Inq_L1, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_L2:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_Inq_L2, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_R1:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_Inq_R1, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_Inq_R2:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_Inq_R2, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_LPAD_X:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_LPAD_X, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;     
               //////////////////////////////////////////////////////////////////
            case CMD_m_LPAD_Y:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_LPAD_Y, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_RPAD_X:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_RPAD_X, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
               //////////////////////////////////////////////////////////////////
            case CMD_m_RPAD_Y:
               //////////////////////////////////////////////////////////////////
               ReplaceLinkedMsg(m_RPAD_Y, attrIt->GetAs_RwChar_ptr(), "return RwReal");
               break;
            }
            ++attrIt;
         }

         RWS_RETURNVOID();
      }
   }//namespace InputDevices
}//namespace RWS

#endif