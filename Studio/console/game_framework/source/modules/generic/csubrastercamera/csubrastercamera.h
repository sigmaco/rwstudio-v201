/*****************************************************************************
*
* File :     CSubRasterCamera.h
*
* Notes :
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

#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/toolkits/world/smartptrs/ccameraptr.h"
#include "framework/mainloop/logic.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"


namespace RWS
{
   namespace Generic
   {
      /**
      *
      *  \ingroup Mod_Generic
      *
      *  Behavior class, Can be used to define a sub-raster camera. This is useful for screen in screen or
      *  split screen effects.
      *
      *  A typical usage would be to set the [Receive Render Camera Event] to iMsgDoRender, and the [Transmit Render Camera Event]
      *  to some arbitrary event name i.e. [RenderSubRasters]. You then need a rendering behavior that will respond to the [RenderSubRasters]
      *  event e.g. FPSRender.
      *
      */

      class CSubRasterCamera : public CSystemCommands, public CAttributeHandler , public CEventHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(CSubRasterCamera);
         RWS_DECLARE_CLASSID(CSubRasterCamera);
         RWS_CATEGORY("Generic");
         RWS_DESCRIPTION("Specify A SubRaster Camera", "received Camera is used to update the Transmit camera, the Transmit camera specifies a sub-raster of the main display raster.");
         
         CSubRasterCamera(const CAttributePacket& attr);
         ~CSubRasterCamera(void);
         
         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);
         
         RWS_BEGIN_COMMANDS
            RWS_MESSAGE  (CMD_In,  "Receive Render Camera Event","When this event is received the transmit render event is sent with the RwCamera owned by this behavior.",RECEIVE , RwCamera*,"iMsgDoRender")

            RWS_ATTRIBUTE(CMD_priority,  "Render Priority",   "Specify the render priority",   SLIDER,  RwUInt32,   RANGE(0,65535,65535))

            RWS_MESSAGE  (CMD_Out, "Transmit Render Camera Event","When this event is received the transmit render event is sent with the RwCamera owned by this behavior.",TRANSMIT , RwCamera*,0)

            RWS_ATTRIBUTE( CMD_Left,      "Left - Pos","Set left raster position", SLIDER, RwReal, RANGE(0,0.25,1))
            RWS_ATTRIBUTE( CMD_Right,     "Right - Pos","Set right raster position", SLIDER, RwReal, RANGE(0,0.75,1))

            RWS_ATTRIBUTE( CMD_Top,       "Top - Pos","Set top raster position", SLIDER, RwReal, RANGE(0,0.25,1))
            RWS_ATTRIBUTE( CMD_Bottom,    "Bottom - Pos","Set bottom raster position", SLIDER, RwReal, RANGE(0,0.75,1))

         RWS_END_COMMANDS;
            
      protected:

         void CreateCamera(void);
         
         CCameraPtr m_pCamera;         /**< Sub-Raster Camera, smart pointer for a RwCamera.*/
         
         CEventId m_Render_In;         /**< Receive Render Event.*/
         CEventId m_Render_Out;        /**< Send Render Event.*/

         RwReal m_top;                 /**< Top of screen specified as fraction of full screen.*/
         RwReal m_bottom;              /**< Bottom of screen specified as fraction of full screen.*/
         RwReal m_left;                /**< Left of screen specified as fraction of full screen.*/
         RwReal m_right;               /**< Right of screen specified as fraction of full screen.*/

         RwUInt32 m_link_priority;     /**< Current Link Priority, Used to determine rendering order.*/
      };
   }//namespace CSubRasterCamera
}//namespace RWS