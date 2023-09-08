/*****************************************************************************
 *
 * File :     CCameraSpline.h
 *
 * Abstract : Declaration of CCameraSpline class/behavior.
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
 * Copyright (c) 2003 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/

#ifndef __CCAMERASPLINE_H__
#define __CCAMERASPLINE_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/mainloop/logic.h"
#include "framework/mainloop/render.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace Examples
   {
      /**
      *
      *  \ingroup Mod_Examples
      *
      *  This class implements a generic camera that follows a spline.
      *
      */
      class CCameraSpline: public CSystemCommands, public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(CCameraSpline);
         RWS_DECLARE_CLASSID(CCameraSpline);
         RWS_CATEGORY("Examples");
         RWS_DESCRIPTION("Camera Spline", "Example of a camera that follows a spline");

         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);

         RWS_BEGIN_COMMANDS
            RWS_SEPARATOR("Messages", 1)

               RWS_MESSAGE(CMD_Set_m_RxEnableCameraEvent,
                           "Enable camera",
                           "Enables this camera",
                           RECEIVE,
                           0,
                           0)

               RWS_MESSAGE( CMD_Set_m_TxEnableCameraEvent,
                           "Camera active",
                           "Send this event when camera becomes active",
                           TRANSMIT,
                           0,
                           0)

               RWS_MESSAGE( CMD_Set_m_RxDisableCameraEvent,
                           "Disable camera",
                           "Disable this camera",
                           RECEIVE,
                           0,
                           0)

               RWS_MESSAGE(CMD_Set_m_InRender,
                           "Render msg (input)",
                           "Incoming rendering message, notes camera settings in data",
                           RECEIVE,
                           RwCamera*,
                           0)
      
               RWS_ATTRIBUTE(CMD_priority, "Render Priority", "Specify the render priority",
                             SLIDER,
                             RwUInt32,
                             RANGE(0, 65535, 65535))

               RWS_MESSAGE(CMD_Set_m_OutRender,
                           "Render msg (output)",
                           "Outgoing rendering message, same as world / incoming, but with updated position and view",
                           TRANSMIT,
                           RwCamera*,
                           0)

               RWS_ATTRIBUTE(CMD_Speed,
                             "Speed",
                             "Speed which the camera moves down the spline.",
                             SLIDER,
                             RwReal,
                             RANGE(0.0, 0.5, 10.0))

               RWS_ATTRIBUTE(CMD_Move_Camera,
                             "Move Camera",
                             "If checked the camera will move along the spline, if unchecked the movement will be paused.",
                             BOOLEAN,
                             RwUInt32,
                             DEFAULT(1))

               RWS_ATTRIBUTE(CMD_Loop,
                             "Loop Camera Movement",
                             "If checked the camera movement will loop once the end of the spline is reached.",
                             BOOLEAN,
                             RwUInt32,
                             DEFAULT(1))

               RWS_MESSAGE(CMD_Set_Next_Control_Point,
                           "Next Control Point",
                           "When this message is received, we move to the next control point on the spline.",
                           RECEIVE,
                           0,
                           0)

               RWS_SEPARATOR("Workspace Sync Camera Request", 1)

               RWS_ATTRIBUTE(CMD_Set_m_respondtocamerarequest, 
                           "Enable Respond to Workspace Sync Camera Request",
                           "Enable this camera responding to the workspace sync camera request.",
                           BOOLEAN,
                           RwUInt32,
                           DEFAULT(1))


         RWS_END_COMMANDS;

         CCameraSpline(const CAttributePacket& attr);
         ~CCameraSpline(void);

         void AddResource(const RWSGUID *pResourceID);
         void HandleSystemCommands(const CAttributePacket &attr);

         void Proc_m_RxEnableCameraEvent(void);
         void Proc_m_RxDisableCameraEvent(void);

#ifdef RWS_DESIGN
         void Proc_iMsg_SetEnableDirectorsCamera(void);
         void Proc_iMsg_SetDisableDirectorsCamera(void);

         RwBool m_respondtocamerarequest;               /**< Design Mode only, enable/disable respond to
                                                             workspace sync camera request */

         RwFrame *m_cameraFrame;                        /**< Design Mode only, player camera RwFrame */
#endif

         RpSpline *m_pSpline;             /**< Pointer to the RenderWare Spline. */

         RwBool m_bDirectorsCam;          /**< Flag indicates that the directors camera
                                               is active. */

         RwReal m_rViewWindow;            /**< Value calculated based on current view angle. */

         RwInt32 m_iCurrentControlPoint;  /**< Stores an index to the control point
                                               that the camera is currently positioned after. */

         RwReal m_rControlPointRatio;     /**< Value between 0 and 1 corresponding to the ratio
                                               of the number of control points. */

         RwReal m_rFrameTimeDelta;        /**< Delta value controlling camera movement from the
                                               current control point to the next control point. */
         RwReal m_rSpeed;                 /**< Speed to move the camera at from the previous point
                                               on the spline. */

         RwBool m_bMoving;                /**< Flag to indicate if the camera is moving. */
         RwBool m_bLoop;                  /**< Flag to store whether the camera should return to
                                               the start of the spline and keep moving once it
                                               reaches the end. */

         RwBool m_bThisCameraActive;      /**< Flag indicates that this camera is current
                                               active, is overridden by 
                                               CConsoleEnvironment::m_bDirectorsCameraIsActive */

         RwUInt32 m_uiOrthoCount;         /**< Counter to store the number of frames passed since
                                               the last time RwMatrixOrthoNormalize was called.
                                               This is an expensive operation and so should not be
                                               called every frame. */

         CEventId m_RxEnableCameraEvent;  /**< Event received to enable this camera */ 
         CEventId m_TxEnableCameraEvent;  /**< Event sent when this camera is enabled */
         CEventId m_RxDisableCameraEvent; /**< Event received to disable this camera */
         CEventId m_RenderInMsg;          /**< Event for incoming render message. */
         RwUInt16 m_RenderInPriority;     /**< Input render message's priority value, 0 = last. */
         CEventId m_RenderOutMsg;         /**< Event for outgoing render message. */
      };
   }//namespace Examples
}//namespace RWS

#endif
