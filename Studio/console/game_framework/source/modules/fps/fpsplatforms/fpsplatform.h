/*****************************************************************************
*
* File :     FPSPlatform.h
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

#ifndef __FPS_PLATFORM_H__
#define __FPS_PLATFORM_H__

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
      *      
      *  \ingroup Mod_FPS
      *    
      *   Platforms are always drawn in the extended position, so they will light correctly.
      *  
      *   If the platform is the target of another trigger or button, it will start out 
      *   disabled in the extended position until it is trigger, when it will lower 
      *   and become a normal platform.
      *   
      *   If the "height" key is set, that will determine the amount the platform moves, 
      *   instead of being implicitly determined by the model's height.
      *     
      *   The use of this entity is a bit counter-intuitive. This entity attaches to 
      *   a brush that will transport the player vertically. The tricky part of 
      *   FPSPlatform is that it will not appear where you place it. It will 
      *   appear lowered 
      *
      *   It should therefore be placed at the highest point and the height value 
      *   adjusted so it starts of where you want it. To make a platform that lowers 
      *   to the ground, use func_door. 
      *        
      *   Parameters 
      *
      *   TRIGGER  Trigger the platform
      *   HEIGHT   How high it will rise. 
      *   SPEED    Speed of it's movement. If not set it will default to 100. 
      *
      *  \see CAttributeHandler, CEventHandler, CSystemCommands
      */
      class FPSPlatform : public FPSToggle, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(FPSPlatform);
         RWS_DECLARE_CLASSID(FPSPlatform);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Standard Platform", "A Platform which can be raised and lowered, the extent of the movement is defined by the geometry of the platform.");
         
         FPSPlatform(const CAttributePacket&);
         
         RWS_BEGIN_COMMANDS                
            RWS_MESSAGE( CMD_targetname, "Target name", "Trigger the platform",  RECEIVE, 0, "ACTN_PLAYERTOUCH")
            RWS_ATTRIBUTE( CMD_height,  "Height", "How high it will rise, If height is zero, the size of the geometry is used.", SLIDER, RwReal, RANGE(0, 0, 999))
            RWS_ATTRIBUTE( CMD_speed,   "Speed", "Speed of it's movement, units per second", SLIDER, RwUInt32, RANGE(1, 100, 999))                
            RWS_END_COMMANDS;
         
         virtual void HandleAttributes(const CAttributePacket& attr);
      };
   }
}

#endif