/*****************************************************************************
*
* File :     FPSButton.h
*
* Abstract : When a button is touched, it moves some distance in the direction 
*            of it's angle, triggers all of it's targets, waits some time, 
*            then returns to it's original position where it can be triggered again.
*   
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

#ifndef __FPS_BUTTON_H__
#define __FPS_BUTTON_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "../fpstoggle/fpstoggle.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace FPS
   { 
      /**
      *
      * \ingroup Mod_FPS
      *
      * Behavior for a standard button.  The button can be triggered by events,  by default this is ACTN_PLAYERTOUCH, 
      * but can be another event.  When the button has been triggered another event is sent out which can be picked 
      * up by another behavior.  Using this you can trigger of doors etc.
      *
      */
      class FPSButton : public FPSToggle, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(FPSButton);
         RWS_DECLARE_CLASSID(FPSButton);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Push Button", "Generic sliding push button.");

         FPSButton(const CAttributePacket&); 

         virtual void HandleAttributes(const CAttributePacket& attr);
         virtual void HandleEvents(CMsg &pMsg);

         RWS_BEGIN_COMMANDS
            RWS_MESSAGE( CMD_targetname, "Target name", "Trigger button event", RECEIVE, 0, "ACTN_PLAYERTOUCH")
            RWS_MESSAGE( CMD_target, "Target", "Button triggered event", TRANSMIT, 0, 0)
            RWS_ATTRIBUTE( CMD_angle, "Angle", "Determines the opening direction. "
                           "Note: There are two special values, -1 opens the door up and -2 opens down.", 
                           SLIDER, RwReal, RANGE(-2,0,360))
            RWS_ATTRIBUTE( CMD_wait, "Wait", "Wait before returning (3 default, -1 = never return)", 
                           SLIDER, RwReal, RANGE(-1,3,60))
            RWS_ATTRIBUTE( CMD_speed, "Speed", "Movement speed (40 default) units per second", 
                           SLIDER, RwUInt32, RANGE(1,40,999))
            RWS_ATTRIBUTE( CMD_lip, "Lip", "Lip remaining at end of move (4 default)", SLIDER, RwUInt32, RANGE(0,4,32))
         RWS_END_COMMANDS;
      };
   }
}
#endif