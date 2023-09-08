/*****************************************************************************
 *
 * File :     CFXMotionBlur.h
 *
 * Abstract : A behavior that can be used to blend the previous frame with the current
 *            frame to produce various motion blur effects.
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
 * Copyright (c) 2000 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/

#ifndef __CFXMOTIONBLUR_H__
#define __CFXMOTIONBLUR_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include "rwcore.h"
#include "rpworld.h"


#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpworld.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//

#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"

#include "framework/core/attributehandler/allocation policies/linearallocationpolicy.h"

#if (defined (DOLPHIN))
#include "gamecube/cfxgamecubemotionblur.h"
#endif

#if (defined (_XBOX))
#include "xbox/cfxxboxmotionblur.h"
#endif

#if (defined (SKY))
#include "sky/cfxskymotionblur.h"
#endif

namespace RWS
{

   namespace FX
   {

      /**
      *
      *  \ingroup Mod_FX
      *
      *  CFXMotionBlur is a behavior that can be used to blend in the last frame rendered with the
      *  current frame.  An attribute is provided which can affect the amount the previous frame is
      *  combined with the current frame. There are also attributes that can transform the orientation
      *  of the previous frame when it is combined with the current frame. Offset, Scaling and Rotation
      *  can be applied allowing for a number of interesting visual effects.
      *  Wherever possible the front buffer is used as a source texture to blend between frames, however
      *  hardware such as the PC and game cube require the allocation of a separate off screen texture to
      *  store the current frame.
      *
      */
      class CFXMotionBlur: public CSystemCommands, public CAttributeHandler,public CEventHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(CFXMotionBlur);
         RWS_DECLARE_CLASSID(CFXMotionBlur);
         RWS_CATEGORY("FX");
         RWS_DESCRIPTION("Motion Blur", "Motion blur behavior");

         RWS_BEGIN_COMMANDS


            RWS_MESSAGE  ( CMD_Start_Render,
                           "Start Render Event",
                           "Applies motion blur to the entire display",
                           RECEIVE,
                           RwCamera*,
                           "iMsgDoPostRender" )

            RWS_ATTRIBUTE( CMD_Priority,
                           "Render Priority",
                           "Specify the render priority",
                           SLIDER,
                           RwUInt32,
                           RANGE(0, 32768, 65535) )

            RWS_ATTRIBUTE( CMD_Set_m_BlurAmount,
                           "Blur amount",
                           "Blur Amount value",
                           SLIDER,
                           RwReal,
                           RANGE(0, 0.5, 1.0) )

            RWS_ATTRIBUTE( CMD_Set_m_OffsetX,
                           "Offset X",
                           "Offset X value",
                           SLIDER,
                           RwReal,
                           RANGE(-1.0,  0.0, 1.0) )

            RWS_ATTRIBUTE( CMD_Set_m_OffsetY,
                           "Offset Y",
                           "Offset Y value",
                           SLIDER,
                           RwReal,
                           RANGE(-1.0,  0.0, 1.0) )

            RWS_ATTRIBUTE( CMD_Set_m_ScaleX,
                           "Scale X Axis",
                           "Scale X axis value",
                           SLIDER,
                           RwReal,
                           RANGE(-5.0, 1.0, 5.0) )
            RWS_ATTRIBUTE( CMD_Set_m_ScaleY,
                           "Scale Y Axis",
                           "Scale Y axis value",
                            SLIDER,
                            RwReal,
                            RANGE(-5.0, 1.0, 5.0) )
            RWS_ATTRIBUTE( CMD_Set_m_Scale,
                           "Scale",
                           "Scale value",
                           SLIDER,
                           RwReal,
                           RANGE(-5.0, 1.0, 5.0) )

            RWS_ATTRIBUTE( CMD_Set_m_Angle,
                           "Angle",
                           "Angle value",
                           SLIDER,
                           RwReal,
                           RANGE(-180, 0.0, 180) )
         RWS_END_COMMANDS;

         CFXMotionBlur(const CAttributePacket& rAttr);
         ~CFXMotionBlur();
         virtual void         HandleEvents         (CMsg &pMsg);
         virtual void         HandleAttributes     (const CAttributePacket& attr);

#if (defined (DOLPHIN))
         CFXGameCubeMotionBlur   m_Target;         /**< GameCube implementation of the motion blur.*/
#endif

#if (defined (_XBOX))
         CFXXBOXMotionBlur       m_Target;         /**< XBox implementation of the motion blur.*/
#endif

#if (defined (SKY))
         CFXSkyMotionBlur        m_Target;         /**< Sky implementation of the motion blur.*/
#endif

      protected:

         CEventId                m_StartRender;   /**< Motion blur event.*/
         RwUInt32                m_link_priority; /**< Current Link Priority, Used to determine rendering order */

      };

   } // namespace FX
      
} // namespace RWS

#endif