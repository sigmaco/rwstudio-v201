/*****************************************************************************
*
* File :     FPSSkyDome.h
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
* Copyright (c) 2002 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#ifndef __FPS_SKYDOME_H__
#define __FPS_SKYDOME_H__

#include "..\fpsrenderer\fpsrender.h"

namespace RWS
{
   namespace FPS
   {
      /**
      *
      *  \ingroup Mod_FPS
      *
      *  Render the scene as if the camera was at the origin of the world. This is a common technique for rendering
      *  a scene that is very far away from the viewer i.e. the sky.
      *
      */
      class FPSSkyDome: public FPSRender
      {
         public:

            RWS_MAKENEWCLASS(FPSSkyDome);
            RWS_DECLARE_CLASSID(FPSSkyDome);
            RWS_CATEGORY("FPS");
            RWS_DESCRIPTION("Sky Dome Rendering Behavior", "All the functionality of FPSRender, but always rendered as if camera is at origin.");

            FPSSkyDome(const CAttributePacket& rAttr) : FPSRender(rAttr) {}
            ~FPSSkyDome(void) {}

            virtual void HandleEvents(CMsg &pMsg);
      };
   }
}

#endif