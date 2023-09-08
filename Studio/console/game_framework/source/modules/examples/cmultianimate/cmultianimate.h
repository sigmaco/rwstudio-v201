/*****************************************************************************
 *
 * File :     CMultiAnimate.h
 *
 * Abstract : A simple animation behavior
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
 * Copyright (c) 2001 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/

#ifndef __CMULTIANIMATE_H__
#define __CMULTIANIMATE_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/mainloop/logic.h"
#include "framework/toolkits/world/smartptrs/cclumpptr.h"
#include "framework/toolkits/animation/canimset.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace Examples
   {
      /**
      *
      *  \ingroup Mod_Examples
      *
      * Hierarchical animation behavior. Performs animation on objects, skeletons, etc. Also performs animation
      * blending over a variable time period. The behavior is placed onto an animatable entity and animations
      * attached as assets of the entity by dragging them onto the entity in the World Lister. The
      * 'Next animation' is then used to cycle through them. Finally, the rate of animation can be adjusted and
      * looping enabled or disabled.
      *
      * The entity data and animations are exported from a 3D package (such as Maya or Max) using the
      * RenderWare export utilities.
      *
      * \see CClumpPtr
      * \see CAnimSet
      *
      */

      class CMultiAnimate: public CSystemCommands, public CAttributeHandler, public CEventHandler, public LinearAllocationPolicy
      {
      public:

         RWS_MAKENEWCLASS(CMultiAnimate);
         RWS_DECLARE_CLASSID(CMultiAnimate);
         RWS_CATEGORY("Examples");
         RWS_DESCRIPTION("Generic Animation", "Example of Animation blending and playback behavior.");

         // commands

         RWS_BEGIN_COMMANDS

            RWS_MESSAGE( CMD_MsgNextAnimTrigger,
                     "Next animation",
                     "When this message is received, the next animation is played",
                     RECEIVE,
                     0,
                     0)

            RWS_ATTRIBUTE(  CMD_Rate,
                        "Animation rate",
                        "Animation rate multiplier - use 1.0 for no multiplication, 0.0 for stop, -ve values for reverse",
                        SLIDER,
                        RwReal,
                        RANGE(-1.5, 1.0, 1.5)
                        )
            RWS_ATTRIBUTE(  CMD_BlendDuration,
                        "Blend duration",
                        "Blend duration, in seconds",
                        SLIDER,
                        RwReal,
                        RANGE(0.0, 0.0, 2.0)
                        )
            RWS_ATTRIBUTE(  CMD_Loop,
                        "Looping",
                        "Enables animation looping",
                        BOOLEAN,
                        RwUInt32,
                        DEFAULT(1)
                        )

         RWS_END_COMMANDS;

         virtual void HandleAttributes(const CAttributePacket& attr);
         virtual void HandleEvents(CMsg&);

         CMultiAnimate(const CAttributePacket&);
         ~CMultiAnimate();

      protected:

         // Revoked ops
         CMultiAnimate( const CMultiAnimate& );
         CMultiAnimate& operator = ( const CMultiAnimate& );

         // Implementation
         void SetRate( RwReal rate );
         void SetLooping( bool doLoop );

         // Data
         CClumpPtr m_pClump;                 /**< RenderWare clump holding the object to animate. */
         CAnimSet m_animSet;                 /**< The animation set class, handles update, blending, etc. */
         CEventId m_msgNextAnimTrigger;      /**< The identity of the message which triggers a change of animation. */
      };
   }//namespace Examples
}//namespace RWS

#endif