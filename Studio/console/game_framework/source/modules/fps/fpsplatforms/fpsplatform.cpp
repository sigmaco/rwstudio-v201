/*****************************************************************************
*
* File :     FPSPlatform.cpp
*
* Abstract : 
*
* A vertically moving platform, triggered by the player standing on it, moves
* up and the waits 3 seconds before moving down.
*
* The behavior of this object is implemented using the FPSToggleInterface class.
*
* FPSToggle handles initializing the FPSToggleInterface class
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
* Copyright (c) 2000 Criterion Software Ltd.
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
// RenderWare Studio Includes
//
#include "fpsplatform.h"

namespace RWS
{
   namespace FPS
   {
      RWS_IMPLEMENT_CLASSID(FPSPlatform);
      RWS_REGISTERCLASS(FPSPlatform);
      
      /**
      *
      * \ref FPSPlatform(). Create FPSPlatform object. Set up trigger states for start and stop states for platform.
      *
      * \ref ~FPSPlatform
      */
      FPSPlatform::FPSPlatform(const CAttributePacket& attr) : FPSToggle(attr, 0, 8, 0, 100) // angle,lip,height,speed
      {
         RWS_FUNCTION("RWS::FPS::FPSPlatform::FPSPlatform");
         
         // Initial state is State_off, setup trigger when ACTN_PLAYERTOUCH is received
         Set_m_msg_rx_off_to_on("ACTN_PLAYERTOUCH");                 // Receive Message off to on
         m_num_msg_rx_off_to_on = 1;                                 // Number of messages off to on required to trigger
         
         // Time out using the default 3 seconds, setup trigger when 3*60 
         // ticks are received.
         Set_m_msg_rx_on_to_off(iMsgRunningTick);
         m_num_msg_rx_on_to_off = (3 * RWS::MainLoop::Logic::Rate);    // Number of messages on to off required to trigger
         
         RWS_RETURNVOID();
      }
      
      
      /*
      *
      * \ref HandleAttributes(). Handle attributes for FPSPlatform.
      *
      * \param attr Standard Attribute Package
      *
      */
      void FPSPlatform::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::FPS::FPSPlatform::HandleAttributes");
         
         // Initialize base class first
         FPSToggle::HandleAttributes(attr);
         
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(FPSPlatform));
         
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
               
            case CMD_height:
               Set_m_height(*attrIt);
               break;
               
            case CMD_speed:
               Set_m_speed(*attrIt);
               GenerateHeightTransform();
               break;
            }
            ++attrIt;
         }
         
         
         // FPSPlatform position is dependent on the size of the geometry, i.e. it is 
         // not positioned at the position in the editor, the editor shows its final position
         // the console shows its start position. Therefore need to modify the position by 
         // detecting a CAtomicPtr::CMD_LoadMatrix and modifying it.
         attrIt = CAttributeCommandIterator(attr, RWS_CLASSID_OF(CSystemCommands));
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CSystemCommands::CMD_LoadMatrix:
               {
                  RwV3d translation;
                  
                  if (GenerateHeightTransform(&translation))
                  {
                     RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic.ptr());
                     if (pFrame)
                     {
                        // Position atomic at start point, position in editor is end point
                        translation.y = - translation.y;
                        
                        RwFrameTranslate(pFrame, &translation, rwCOMBINEPOSTCONCAT);
                     }
                  }
               }
               break;
            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
   }// namespace FPS
}// namespace RWS