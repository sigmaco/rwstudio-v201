/****************************************************************************
*
* File :      CSeqCamera.cpp
*
* Abstract :  Sequencable camera behaviour.
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

// Left to its own devices, the MW linker will dead strip the registrar ctors, so...

#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off     // Turn off exceptions due to bug in MW Dwarf
#endif                     // Generator for PS2, present in CW 3.04

/////////////////////////////
//
// RenderWare Studio Includes
//

#include "framework/core/resourcemanager/cresourcemanager.h"
#include "framework/core/netstream/nettools.h"
#include "framework/toolkits/world/helpers/camerahelper.h"
#include "framework/toolkits/world/factory.h"
#include "framework/toolkits/math/maths.h"
#include "framework/mainloop/render.h"
#include "framework/mainloop/mainloop.h"

#include "cseqcamera.h"

namespace RWS
{
   namespace SEQUENCE
   {
      /**
      *
      * CSeqCamera class functions.
      *
      */

      RWS_IMPLEMENT_CLASSID(CSeqCamera);
      RWS_REGISTERCLASS(CSeqCamera);
      RWS_DS_REGISTER_START(CSeqCamera)
         RWS_DS_REGISTER_BASE(CSeqCamera, CSeqPos)
      RWS_DS_REGISTER_END

      /**
      *
      * Constructor - setup base class, etc.
      *
      */

      CSeqCamera::CSeqCamera(const CAttributePacket &attr) :
         CSeqPos(attr)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCamera::CSeqCamera");

         RwMatrixSetIdentity(&m_nonScaledMatrix);
         LinkMsg(iMsgStartSystem);

#ifdef RWS_DESIGN
         
         LinkMsg(iMsgRequestCurrentCameraMatrix, "RwUInt32 InstanceID");
#endif

         RWS_RETURNVOID();
      }

      /**
      *
      * Destructor - unlink from messages, etc.
      *
      */
         
      CSeqCamera::~CSeqCamera(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCamera::~CSeqCamera");

         // Unlink & unregister messages...

         UnLinkMsg(m_inRenderMsg);
         UnRegisterMsg(m_inRenderMsg);
         UnLinkMsg(m_outRenderMsg);
         UnRegisterMsg(m_outRenderMsg);
         UnLinkMsg(m_enableMsg);
         UnRegisterMsg(m_enableMsg);
         UnLinkMsg(m_disableMsg);
         UnRegisterMsg(m_disableMsg);

         UnLinkMsg(iMsgStartSystem);

#ifdef RWS_DESIGN
         
         UnLinkMsg(iMsgRequestCurrentCameraMatrix);
#endif

         RWS_RETURNVOID();
      }

      /**
      *
      * Standard RenderWare Studio behaviour's event handler.
      *
      */

      void CSeqCamera::HandleEvents(CMsg &msg)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCamera::HandleEvents");

         // Process events of the base class.

         CSeqPos::HandleEvents(msg);

         // Start up...

         if (msg.Id == iMsgStartSystem)
         {
            UnLinkMsg(iMsgStartSystem);
            m_enabledFlag = m_startEnabled;
         }

         // Incoming rendering message (only process if enabled AND there is at least
         // on listening behaviour).

         if (msg.Id == m_inRenderMsg && msg.pData && m_enabledFlag &&
            m_outRenderMsg.Get_registered_count())
         {
            RwCamera *pOutCam;
            RwFrame *pOutFrame;
            RwMatrix *pDestMatrix;

            // Clone incoming camera, so that the data in the original message is not altered.

            pOutCam = RwCameraClone(static_cast<RwCamera *>(msg.pData));
            RWS_ASSERT(pOutCam, "Failed to clone incoming camera data.");

            // Create frame for new camera object...

            pOutFrame = RwFrameCreate();
            RWS_ASSERT(pOutFrame, "Failed to create frame for camera.");

            // Copy in the frame from this atomic and set the field-of-view, use the
            // matrix which has NO scaling applied to it.

            pDestMatrix = RwFrameGetMatrix(pOutFrame);
            RWS_ASSERT(pDestMatrix, "No destination matrix.");

            RwMatrixCopy(pDestMatrix, &m_nonScaledMatrix);

            // Attach matrix to camera (don't want to share frame with incoming, cloned, camera).

            RwCameraSetFrame(pOutCam, pOutFrame);
            CameraHelper::SetFieldOfView(m_fov, pOutCam);

            // Send message...

            CMsg outMsg(m_outRenderMsg, pOutCam);
            SendMsg(outMsg);

            // Tidy up.

            RwCameraSetFrame(pOutCam, 0);
            RwFrameDestroy(pOutFrame);
            RwCameraDestroy(pOutCam);
         }

         // Incoming enable message.

         if (msg.Id == m_enableMsg)
         {
            m_enabledFlag = TRUE;
         }

         // Incoming disable message.

         if (msg.Id == m_disableMsg)
         {
            m_enabledFlag = FALSE;
         }

         // 'Sync camera' request from the workspace.

#ifdef RWS_DESIGN

         if (msg.Id == iMsgRequestCurrentCameraMatrix && m_workspaceSyncFlag)
         {
            RwFrame *pFrame;

            // Send the atomic's frame.

            pFrame = RpAtomicGetFrame(m_pAtomic.ptr());
            RWS_ASSERT(pFrame, "Atomic has no frame.");
            NetTools::SendTransform(pFrame);
         }
#endif

         RWS_RETURNVOID();
      }

      /**
      *
      * Standard RenderWare Studio behaviour's attribute handler.
      *
      */

      void CSeqCamera::HandleAttributes(const CAttributePacket &attr)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCamera::HandleAttributes");

         // Allow base class to handle it's attributes.

         CSeqPos::HandleAttributesOnly(attr);

         // Process this behaviour's attributes.

         HandleAttributesOnly(attr);

         if (m_updated)
         {
            // Do NOT call virtual one here, as this function should NOT be called by
            // the derived class's own HandleAttributes. Instead it should call the
            // 'HandleAttributesOnly' function, then do it's own update, then call it's
            // own update function (which should also call this class's UpDateObjectData
            // function).

            UpdateObjectData();
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Only handles the attribute update for this behaviour. Does NOT do any base
      * classes, etc.
      *
      */

      void CSeqCamera::HandleAttributesOnly(const CAttributePacket &attr)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCamera::HandleAttributesOnly");

         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CSeqCamera));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               // Messages...

            case MSG_RENDER_IN:

               UpdateRenderInMsg(this, attrIt->GetAs_void_ptr());
               break;

            case MSG_RENDER_OUT:

               UpdateRenderOutMsg(this, attrIt->GetAs_void_ptr());
               break;

            case MSG_ENABLE:

               UpdateEnableMsg(this, attrIt->GetAs_void_ptr());
               break;

            case MSG_DISABLE:

               UpdateDisableMsg(this, attrIt->GetAs_void_ptr());
               break;

               // Commands...

            case CMD_PRIORITY:

               UpdateRenderPriority(this, attrIt->GetAs_void_ptr());
               break;

            case CMD_ENABLED:

               // No direct-set on this.

               m_startEnabled = (attrIt->GetAs_RwUInt32() != 0);
               break;

            case CMD_WS_RESPOND:

#ifdef RWS_DESIGN

               // No direct-set on this.

               m_workspaceSyncFlag = attrIt->GetAs_RwUInt32();
#endif
               break;

            case CMD_FOV:

               UpdateFOV(this, attrIt->GetAs_void_ptr());
               break;
            }

            ++attrIt;
         }

         RWS_RETURNVOID();
      }

      /**
      *
      * Overridden update function, updates this class and the base class.
      *
      */

      void CSeqCamera::Update(void)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCamera::Update");
         
         // Just call this functions update.

         UpdateObjectData();

         RWS_RETURNVOID();
      }

      /**
      *
      * Updates the object data and optionally copies the unscaled matrix to the
      * passed in matrix.
      *
      */

      void CSeqCamera::UpdateObjectData(RwMatrix *pCopyMatrix)
      {
         RWS_FUNCTION("RWS::SEQUENCE::CSeqCamera::UpdateObjectData");

         // Call base update, but need matrix without any scaling, so it can be used
         // to position the camera during rendering.

         CSeqPos::UpdateObjectData(&m_nonScaledMatrix);

         // Copy this matrix to one passed in?

         if (pCopyMatrix)
         {
            RwMatrixCopy(pCopyMatrix, &m_nonScaledMatrix);
         }

         RWS_RETURNVOID();
      }
   }  // SEQUENCE
}     // RWS
