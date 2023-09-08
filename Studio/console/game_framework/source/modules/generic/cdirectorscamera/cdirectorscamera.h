/*****************************************************************************
*
* File :     CDirectorsCamera.cpp
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

#ifdef RWS_DESIGN

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
      * \ingroup Mod_Generic
      *
      * Moving the camera in the workspace modifies the global camera, if the rendering behavior
      * isn't using the global camera then this behavior can be used to pipe the global camera to the
      * rendering behavior when the directors camera is enabled.
      *
      */
      class CDirectorsCamera: public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
         public:
            RWS_MAKENEWCLASS(CDirectorsCamera);
            RWS_DECLARE_CLASSID(CDirectorsCamera);
            RWS_CATEGORY("Generic");
            RWS_DESCRIPTION("Directors Camera", "Pipes the global camera (Directors Camera) to a render event");
         
            virtual void HandleEvents(CMsg &pMsg);
            virtual void HandleAttributes(const CAttributePacket& attr);
            
            CDirectorsCamera(const CAttributePacket& rAttr);
            ~CDirectorsCamera();

            RWS_BEGIN_COMMANDS

               RWS_MESSAGE  (CMD_RenderOut, "Transmit Render Camera Event","When the iMsgDoRenderDirectorsCamera event is received then this event is sent.",TRANSMIT , RwCamera*,"iMsgDoRender")

            RWS_END_COMMANDS;

         protected:

            CEventId m_Render_Out;        /**< Send Render Event.*/
      };
   }//namespace CDirectorsCamera
}//namespace RWS

#endif