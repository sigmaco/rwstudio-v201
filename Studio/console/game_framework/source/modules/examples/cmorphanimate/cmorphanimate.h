/*****************************************************************************
*
* File :       CMorphAnimate.h
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
* Copyright (c) 2000 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"

#include "framework/toolkits/world/smartptrs/cclumpptr.h"
#include "framework/mainloop/logic.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace Examples
   {
      /**
      *
      *  \ingroup Mod_Examples
      *
      *  CMorphAnimate updates RpMorphAtomicAddTime each tick with the values specified by the attribute, Morph rate.
      *  The behavior MUST be used with an asset which has morph targets and animation within its data. See
      *  RenderWare exporter documentation for more information.
      *
      *  \see CAttributeHandler
      *  \see CEventHandler
      *  \see CSystemCommands
      *
      */
      
      class CMorphAnimate : public CSystemCommands, public CAttributeHandler , public CEventHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(CMorphAnimate);
         RWS_DECLARE_CLASSID(CMorphAnimate);
         RWS_CATEGORY("Examples");
         RWS_DESCRIPTION("Morph Animate", "Example of using the RpMorph plugin.");
         
         RWS_BEGIN_COMMANDS
            RWS_ATTRIBUTE( CMD_rDelta,
                           "Morph Rate",
                           "Specified the rate at which morphing proceeds i.e. the higher the value the faster the animation",
                           SLIDER,
                           RwReal,
                           RANGE(0, 0.01, 1.0))

            RWS_END_COMMANDS;
         
         CMorphAnimate(const CAttributePacket& attr);
         ~CMorphAnimate(void);
         
         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);
         
      protected:
         
         CClumpPtr m_pClump;     /**< Clump of the behavior */
         RwReal m_rDelta;          /**< Delta value controlling speed of morph */
      };
   }//namespace Examples
}//namespace RWS
