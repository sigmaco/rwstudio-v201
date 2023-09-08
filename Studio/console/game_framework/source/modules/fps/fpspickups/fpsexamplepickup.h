/*****************************************************************************
*
* File :     FPSExamplePickup.h
*
* Abstract : 
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

#ifndef __FPS_EXAMPLE_PICKUP_H__
#define __FPS_EXAMPLE_PICKUP_H__

#include "fpspickup.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"


namespace RWS
{
   namespace FPS
   { 
      class FPSExamplePickup : public FPSPickup, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(FPSExamplePickup);
         RWS_DECLARE_CLASSID(FPSExamplePickup);
         RWS_CATEGORY("FPS");
         RWS_DESCRIPTION("Standard Pickup", "A static pickup.");
         
         FPSExamplePickup(const CAttributePacket& attr) : FPSPickup(attr)
         {
            Set_m_msg_response_tx("FPS_Example_Pickup");
         }
      };
   } // namespace FPS
} // namespace RWS

#endif
