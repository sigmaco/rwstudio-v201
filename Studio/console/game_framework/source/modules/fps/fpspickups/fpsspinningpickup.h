/*****************************************************************************
*
* File :     FPSSpinningPickup.h
*
* Abstract : Declaration of FPSSpinningPickup class
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
#ifndef __FPS_SPINNINGPICKUP_H__
#define __FPS_SPINNINGPICKUP_H__

#include "fpspickup.h"
#include "framework/core/attributehandler/allocation policies/linearallocationpolicy.h"

namespace RWS
{
   namespace FPS
   {
      /**
      *
      *  \ingroup Mod_FPS
      *
      *  This behavior builds upon FPSPickup and makes the pickup spin.
      */
      class FPSSpinningPickup: public FPSPickup, public LinearAllocationPolicy
      {
         public:
            RWS_MAKENEWCLASS(FPSSpinningPickup);
            RWS_DECLARE_CLASSID(FPSSpinningPickup);
            RWS_CATEGORY("FPS");
            RWS_DESCRIPTION("Spinning Pickup", "A spinning pickup.");

            RWS_BEGIN_COMMANDS
                RWS_ATTRIBUTE(CMD_Set_Spin_Rate,
                     "Spin Rate",
                     "Set pickup spin rate.",
                     SLIDER,
                     RwReal,
                     RANGE(0,8,50))
            RWS_END_COMMANDS;

            FPSSpinningPickup(const CAttributePacket& attr);
            ~FPSSpinningPickup();

            void HandleEvents(CMsg &pMsg);
            void HandleAttributes(const CAttributePacket& attr);

         protected:

            RwMatrix m_pRotation;

         private:
            RwReal m_RotRate;
            RwUInt32 m_count;             /**< Counts the number of frame before matrix normalize occurs. */


      };
   }
}
#endif