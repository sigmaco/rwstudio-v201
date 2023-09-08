/*****************************************************************************
*
* File :     FPSBouncyPickup.h
*
* Abstract : Declaration of FPSBouncyPickup class
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
#ifndef __FPS_BOUNCYPICKUP_H__
#define __FPS_BOUNCYPICKUP_H__

#include "fpspickup.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace FPS
   {
      /**
      *
      *  \ingroup Mod_FPS
      *
      *  This behavior builds upon FPSPickup and makes the pickup bounce.
      */
      class FPSBouncyPickup: public FPSPickup, public LinearAllocationPolicy
      {
         public:
            RWS_MAKENEWCLASS(FPSBouncyPickup);
            RWS_DECLARE_CLASSID(FPSBouncyPickup);
            RWS_CATEGORY("FPS");
            RWS_DESCRIPTION("Bouncy Pickup", "A bouncy spinning pickup.");

            RWS_BEGIN_COMMANDS
                RWS_ATTRIBUTE(CMD_Set_Spin_Rate,    "Spin Rate",    "Set pickup spin rate, controls the rate at which the pickup spins.",         SLIDER,  RwReal,  RANGE(0,8,50))
                RWS_ATTRIBUTE(CMD_Set_Bounce_Height,  "Bounce Height",  "Set pickup bounce height, controls the magnitude of the vertical movement.",   SLIDER,  RwReal,  RANGE(0,3,50))
                RWS_ATTRIBUTE(CMD_Set_Bounce_Rate,  "Bounce Rate",  "Set pickup bounce rate, controls the rate the pickup bounces.",   SLIDER,  RwReal,  RANGE(0,8,360))
            RWS_END_COMMANDS;


            FPSBouncyPickup(const CAttributePacket& attr);
            ~FPSBouncyPickup();

            void HandleEvents(CMsg &pMsg);
            void HandleAttributes(const CAttributePacket& rAttr);

         protected:

            RwMatrix m_pRotation;

            RwReal m_rTheta;
            RwReal m_rActualHeight;

         private:
            RwReal m_RotRate;
            RwReal m_BounceHeight;
            RwReal m_BounceRate;

            RwUInt32 m_Count;
      };
   }
}
#endif