/*****************************************************************************
*
* File :      CSeqCamera.h
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

#ifndef C_SEQ_CAMERA_HEADER
#define C_SEQ_CAMERA_HEADER

#include "cseqpos.h"

namespace RWS
{
   namespace SEQUENCE
   {
      class CSeqCamera : public CSeqPos
      {
      public:

         RWS_MAKENEWCLASS(CSeqCamera);
         RWS_DECLARE_CLASSID(CSeqCamera);
         RWS_CATEGORY("Sequence");
         RWS_DESCRIPTION("Sequencable camera",
            "Sequencable behaviour for controlling a camera position.");

         RWS_BEGIN_COMMANDS            
            RWS_SEPARATOR("Camera", 0)
               RWS_MESSAGE(MSG_RENDER_IN,
                  "Render in",
                  "Incoming rendering message",
                  RECEIVE,
                  RwCamera*,
                  0)
               RWS_ATTRIBUTE(CMD_PRIORITY,
                  "Rendering priority",
                  "The priority of the incoming render message (0 = last)",
                  SLIDER,
                  RwUInt32,
                  RANGE(0, 65535, 65535))
               RWS_MESSAGE(MSG_RENDER_OUT,
                  "Render out",
                  "Outgoing render message, camera is modified IF behaviour is enabled",
                  TRANSMIT,
                  RwCamera*,
                  0)
               RWS_ATTRIBUTE(CMD_ENABLED,
                  "Start enabled",
                  "Sets if camera is initially enabled or not. State modified by enable & disable messages",
                  BOOLEAN,
                  RwUInt32,
                  DEFAULT(1))
               RWS_MESSAGE(MSG_ENABLE,
                  "Enable",
                  "Enables the camera, will modify incoming camera and send it out via 'Render Out' msg",
                  RECEIVE,
                  0,
                  0)
               RWS_MESSAGE(MSG_DISABLE,
                  "Disable",
                  "Disables the camera, will no longer send it out modified camera object",
                  RECEIVE,
                  0,
                  0)
               RWS_ATTRIBUTE(CMD_WS_RESPOND,
                  "Respond to workspace",
                  "Respond to the workspace's requests to 'Sync Camera'",
                  BOOLEAN,
                  RwUInt32,
                  DEFAULT(1))
               RWS_ATTRIBUTE(CMD_FOV,
                  "Field-of-view",
                  "The field-of-view of the camera in degrees.",
                  SLIDER,
                  RwReal,
                  RANGE(0.1, 46, 120))
         RWS_END_COMMANDS;

         RWS_DS_TABLE_START

            RWS_DS_START_CMD_SECTION
               RWS_DS_CMD(MSG_RENDER_IN,
                  RWS_CMD_DATA_BLANK,
                  RWS_CMD_FUNC(CSeqCamera, UpdateRenderInMsg))
               RWS_DS_CMD(CMD_PRIORITY,
                  RWS_CMD_DATA_BLANK,
                  RWS_CMD_FUNC(CSeqCamera, UpdateRenderPriority))
               RWS_DS_CMD(MSG_RENDER_OUT,
                  RWS_CMD_DATA_BLANK,
                  RWS_CMD_FUNC(CSeqCamera, UpdateRenderOutMsg))

               // NOTE: CMD_ENABLED is not an 'on the fly' settable value, so no direct-set.

               RWS_DS_CMD(MSG_ENABLE,
                  RWS_CMD_DATA_BLANK,
                  RWS_CMD_FUNC(CSeqCamera, UpdateEnableMsg))
               RWS_DS_CMD(MSG_DISABLE,
                  RWS_CMD_DATA_BLANK,
                  RWS_CMD_FUNC(CSeqCamera, UpdateDisableMsg))

               // NOTE: CMD_WS_RESPOND is a design only function, so has no direct-set.

               RWS_DS_CMD(CMD_FOV,
                  RWS_CMD_DATA_BLANK,
                  RWS_CMD_FUNC(CSeqCamera, UpdateFOV))
            RWS_DS_END_CMD_SECTION(CSeqCamera)

         RWS_DS_TABLE_END

         // NOTE: There is no post-change, 'lazy update' function here, as the base class
         //       implements a virtual 'update' function instead. This is needed so that
         //       updates due to events will also be picked up.

         CSeqCamera(const CAttributePacket &attr);
         ~CSeqCamera(void);

         virtual void HandleEvents(CMsg &msg);
         virtual void HandleAttributes(const CAttributePacket &attr);

      private:

         // 'Direct-set' callback functions...

         /**
         *
         * Changes the linked message name for the render-in message. The
         * name must be a valid string.
         *
         */

         static void UpdateRenderInMsg(void *pThis, const void *pData)
         {
            RWS_ASSERT(pData, "Invalid message name.");
            CSeqCamera *pSeqCamera = static_cast<CSeqCamera *>(pThis);
            const char *pMsgName = static_cast<const char *>(pData);
            ReplaceLinkedMsg(*pSeqCamera, pSeqCamera->m_inRenderMsg, pMsgName,
               "RwCamera*", pSeqCamera->m_renderPriority);
         }

         /**
         *
         * Changes the link priority on the incoming render message.
         *
         */

         static void UpdateRenderPriority(void *pThis, const void *pData)
         {
            CSeqCamera *pSeqCamera = static_cast<CSeqCamera *>(pThis);
            const RwUInt16 *pPriority = static_cast<const RwUInt16 *>(pData);

            pSeqCamera->m_renderPriority = *pPriority;
            SetLinkPriority(*pSeqCamera, pSeqCamera->m_inRenderMsg, "RwCamera*",
               pSeqCamera->m_renderPriority);
         }

         /**
         *
         * Changes the linked message name for the render-out message. The
         * name must be a valid string.
         *
         */

         static void UpdateRenderOutMsg(void *pThis, const void *pData)
         {
            RWS_ASSERT(pData, "Invalid message name.");
            CSeqCamera *pSeqCamera = static_cast<CSeqCamera *>(pThis);
            const char *pMsgName = static_cast<const char *>(pData);
            ReplaceLinkedMsg(*pSeqCamera, pSeqCamera->m_outRenderMsg, pMsgName, "RwCamera*");
         }

         /**
         *
         * Changes the linked message name for the enable camera message. The
         * name must be a valid string.
         *
         */

         static void UpdateEnableMsg(void *pThis, const void *pData)
         {
            RWS_ASSERT(pData, "Invalid message name.");
            CSeqCamera *pSeqCamera = static_cast<CSeqCamera *>(pThis);
            const char *pMsgName = static_cast<const char *>(pData);
            ReplaceLinkedMsg(*pSeqCamera, pSeqCamera->m_enableMsg, pMsgName, 0);
         }

         /**
         *
         * Changes the linked message name for the disable camera message. The
         * name must be a valid string.
         *
         */

         static void UpdateDisableMsg(void *pThis, const void *pData)
         {
            RWS_ASSERT(pData, "Invalid message name.");
            CSeqCamera *pSeqCamera = static_cast<CSeqCamera *>(pThis);
            const char *pMsgName = static_cast<const char *>(pData);
            ReplaceLinkedMsg(*pSeqCamera, pSeqCamera->m_disableMsg, pMsgName, 0);
         }

         /**
         *
         * Calculates the field-of-view value needed for modifying the camera on rendering.
         *
         */

         static void UpdateFOV(void *pThis, const void *pData)
         {
            RWS_ASSERT(pData, "Invalid field-of-view angle.");
            CSeqCamera *pSeqCamera = static_cast<CSeqCamera *>(pThis);
            const RwReal *pFOVAngle = static_cast<const RwReal *>(pData);
            pSeqCamera->m_fov = RwTan(RWDEG2RAD(*pFOVAngle * 0.5f));
         }

         // Internal variables.

         RwMatrix m_nonScaledMatrix;      /**< Orientation of object, but not scaled. */
         RwReal m_fov;                    /**< Field-of-view for 'CameraHelper::SetFieldOfView'. */
         CEventId m_inRenderMsg,
            m_outRenderMsg,
            m_enableMsg,
            m_disableMsg;
         RwUInt32 m_enabledFlag;          /**< Can be changed by 'direct set' so must be RwUInt32. */

#ifdef RWS_DESIGN

         RwUInt32 m_workspaceSyncFlag;    /**< Can be changed by 'direct set' so must be RwUInt32. */
#endif

         RwUInt32 m_renderPriority;
         RwBool m_startEnabled;           /**< Initial state of the enabled flag on 'iMsgStartSystem'. */

      protected:

         void HandleAttributesOnly(const CAttributePacket &attr);
         void UpdateObjectData(RwMatrix *pCopyMatrix = 0);
         virtual void Update(void);
      };
   }     // SEQUENCE
}        // RWS
#endif   // #ifndef C_SEQ_CAMERA_HEADER
