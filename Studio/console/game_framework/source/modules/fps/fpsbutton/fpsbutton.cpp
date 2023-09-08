/*****************************************************************************
*
* File :     FPSButton.cpp
*
* Abstract : When a button is touched, it moves some distance in the direction 
*            of it's angle, triggers all of it's targets, waits some time, 
*            then returns to it's original position where it can be triggered again.
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
#include "precomp.h"
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>
#include <rpworld.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpworld.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "fpsbutton.h"

namespace RWS
{
   namespace FPS
   { 
      RWS_IMPLEMENT_CLASSID(FPSButton);
      RWS_REGISTERCLASS(FPSButton);
      
      /**
      *
      * \ref FPSButton. Constructor for FPSButton
      *
      * \see ~FPSButton
      *
      */
      FPSButton::FPSButton(const CAttributePacket& attr)  : FPSToggle(attr, 0, 4, 0, 40) // angle,lip,height,speed
      {
         RWS_FUNCTION("RWS::FPS::FPSButton::FPSButton");
         
         // Receive Message off to on
         Set_m_msg_rx_off_to_on("ACTN_PLAYERTOUCH");         
         
         // Number of messages off to on required to trigger
         m_num_msg_rx_off_to_on = 1;    
         
         Set_m_msg_rx_on_to_off(iMsgRunningTick);
         
         // Number of messages on to off required to trigger
         m_num_msg_rx_on_to_off = (RWS::MainLoop::Logic::Rate);    
         
         // Receive Message off to on Reverse Event
         Set_m_msg_rx_on_to_off_cancel("ACTN_PLAYERTOUCH");  
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref HandleAttributes. Handle attribute updates.
      *
      * \param Attr reference to a CAttributePacket.
      *
      */  
      void FPSButton::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSButton::HandleAttributes");
         
         // Initialize base class first
         FPSToggleInterface::HandleAttributes(attr);
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(FPSButton));
         
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CMD_targetname:
               {
                  const RwChar *name;
                  attrIt->GetCommandData(&name);
                  Set_m_msg_rx_off_to_on(name);
               }
               break;
               
            case CMD_target:
               {
                  const RwChar *name;
                  attrIt->GetCommandData(&name);
                  Set_m_msg_tx_finished_off_to_on(name);
               }
               break;
               
            case CMD_angle:
               Set_m_angle(*attrIt);
               break;
               
            case CMD_wait:
               Set_m_wait(*attrIt);
               break;
               
            case CMD_lip:
               Set_m_lip(*attrIt);
               break;
               
            case CMD_speed:
               Set_m_speed(*attrIt);
               GenerateAngleTransform();
               break;
            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      * \ref HandleEvents(). Handle events.    
      *
      * \param pMsg Standard Message Package.
      *  
      * \ref iMsgRunningTick
      */
      void FPSButton::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSButton::HandleEvents");
         
         // Need to generate the transform for the platforms movement
         if (Get_m_state() == State_off)
         {
            if (pMsg.Id == Get_m_msg_rx_off_to_on()) 
            {
               GenerateAngleTransform();
            }
         }
         
         FPSToggle::HandleEvents(pMsg);
         
         RWS_RETURNVOID();
      }
   }// namespace FPS
}// namespace RWS