/*****************************************************************************
*
* File :     FPSSkyDome.cpp
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
#include "precomp.h"

// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

#include "fpsskydome.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/helpers/camerahelper.h"


namespace RWS
{
   namespace FPS
   {
      RWS_REGISTERCLASS(FPSSkyDome);
      RWS_IMPLEMENT_CLASSID(FPSSkyDome);

      /**
      *
      *  Handle events for FPSSkyDome, overrides m_StartRender event handler to position the camera at the 
      *  origin of the world.
      *
      *  \param pMsg Reference to a CMsg object.
      *
      */
      void FPSSkyDome::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSSkyDome::HandleEvents");

         if (pMsg.Id == m_StartRender)
         {
            // Modify the camera's frame so the it is positioned at the origin.
            //
            RwCamera *pRwCamera = reinterpret_cast<RwCamera*>(pMsg.pData);

            RWS_PRE(pRwCamera);

            RwFrame *pFrame = RwCameraGetFrame(pRwCamera);

            RWS_PRE(pFrame);

            RwMatrix *pMatrix = RwFrameGetMatrix(pFrame);

            RWS_PRE(pMatrix);

            RwV3d *pPos = RwMatrixGetPos(pMatrix);

            RWS_PRE(pPos);

            // Save Camera's current position
            //
            RwV3d OldPos = *pPos;

            // Set new position.
            //
            pPos->x = 0.0f;
            pPos->y = 0.0f;
            pPos->z = 0.0f;

            // Don't call RwMatrixUpdate as flags are unchanged, but
            // ensure any linked objects are updated.
            //
            RwFrameUpdateObjects(pFrame);

            DoRender(pRwCamera);

            // End Camera Event
            //
            CMsg Msg(m_EndRender, pMsg.pData);

            SendMsg(Msg);

            // Reset camera position
            //
            *pPos = OldPos;

            // Don't call RwMatrixUpdate as flags are unchanged, but
            // ensure any linked objects are updated.
            //
            RwFrameUpdateObjects(pFrame);
         }
         else
         {
            FPSRender::HandleEvents(pMsg);
         }

         RWS_RETURNVOID();
      }
   }
}
