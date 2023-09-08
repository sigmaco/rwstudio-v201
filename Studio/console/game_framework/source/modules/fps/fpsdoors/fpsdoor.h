/*****************************************************************************
*
* File :     FPSDoor.h
*
* Abstract : Doors can either move horizontally or vertically depending on 
*            the setting of angle
*
*            They are based on the FPSToggleInterface class, but directly derived from 
*            FPSToggle which handles initializing the FPSToggleInterface class.
* 
*            Doors which overlap, will act as a single door
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

#ifndef __FPS_DOOR_H__
#define __FPS_DOOR_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>
#include <rpcollis.h>
#include <rtintsec.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpcollis.lib")
#pragma comment (lib, "rtintsec.lib") 
#endif

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
      * Implementation for a Door.  This class lets you set up and set of doors which move from side to side. 
      * If the doors overlap, they will be come linked together,  so if one door gets activated it will also 
      * activate its linked door.  If you do not wish for this to happen you can set a 'no linked' 
      * flag in the attributes.
      *
      */
      class FPSDoor : public FPSToggle, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(FPSDoor);
         RWS_DECLARE_CLASSID(FPSDoor);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Sliding Door", "A generic two state sliding door.");

         FPSDoor(const CAttributePacket&);
         ~FPSDoor(void);

         RWS_BEGIN_COMMANDS
             RWS_MESSAGE( CMD_targetname, "Target name", "Trigger the door", RECEIVE, 0, "ACTN_PLAYERTOUCH")
             RWS_ATTRIBUTE( CMD_angle, "Angle", "Determines the opening direction. "
                            "Note: There are two special values, -1 opens the door up and -2 opens down.", 
                            SLIDER, RwReal, RANGE(-2,0,360))
             RWS_ATTRIBUTE( CMD_wait, "Wait", "Wait before returning (3 default, -1 = never return)",
                            SLIDER, RwReal, RANGE(-1,3,60))
             RWS_ATTRIBUTE( CMD_speed, "Speed", "Movement speed (100 default) units per second",
                            SLIDER, RwUInt32, RANGE(1,100,999))
             RWS_ATTRIBUTE( CMD_lip, "Lip", "Lip remaining at end of move (8 default)",
                            SLIDER, RwUInt32, RANGE(0,8,32))
             RWS_ATTRIBUTE( CMD_spawnflags, "Flags", "", 
                            BITFIELD, RwUInt32, LIST("(1)Door Starts Open|(2)Unused|(4)Door Not Linked|(8)Unused|(16)Unused|(32)Unused|(64)Unused|(128)Unused|(256)Unused|(512)Unused(medium)|(1024)Unused|(2048)Unused"))
         RWS_END_COMMANDS;

         virtual void HandleAttributes(const CAttributePacket& attr);            
         virtual void HandleEvents(CMsg &pMsg);

      protected:

         RwUInt32 m_bStartOpen:1;                /**< Flag specifies that this door starts open */

         FPSDoor *pLinked;                       /**< Overlapping doors act as one door, this pointer
                                                      links the two FPSDoors allowing events to be
                                                      shared between them. */

         static CEventId m_Inq_FPS_func_door;    /**< Inquire Event, sent between doors to determine that
                                                      they are doors. */

         static RpAtomic   *CheckIfTouching( RpIntersection * intersection,
                                             RpWorldSector * sector,
                                             RpAtomic * atomic,
                                             RwReal distance,
                                             void *data);

         void SearchForLinks(void);
         void ClearLinks(void);
         void ApplyOpenTranslation(RwReal scale);

         RwBool StartsOpen();
         RwBool NotLinked();
      };
   }// namespace FPS
}// namespace RWS
#endif