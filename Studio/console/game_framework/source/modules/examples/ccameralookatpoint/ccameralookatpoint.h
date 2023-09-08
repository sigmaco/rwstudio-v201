/*****************************************************************************
 *
 * File :     CCameralookatpoint.h
 *
 * Abstract : Declaration of aptly named "CCameraLookAtPoint" class/behavior.
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

#ifndef __CCAMERALOOKATPOINT_H__
#define __CCAMERALOOKATPOINT_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/mainloop/logic.h"
#include "framework/mainloop/render.h"
#include "framework/toolkits/world/smartptrs/catomicptr.h"
#include "framework/toolkits/world/smartptrs/ccameraptr.h"
#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace Examples
   {
      /**
      *
      *  \ingroup Mod_Examples
      *
      *  This class implements a generic camera that follows another object. This behavior is
      *  common to 3rd person or driving games. The object is followed along the floor of the
      *  world (xz space) and the camera moves along the y axis to maintain an aligned view of the object
      *  being followed. The preferred angle, height, distance, etc can be set, along with the
      *  minimum, maximum and smoothing values for these parameters. The linked to object must
      *  transmit a message containing a RwFrame* for the camera to be able to follow it. The
      *  camera can be used with sub-raster camera and other camera systems due to its ability
      *  to receive and transmit a (non-destructively modified) RwCamera* object.
      *
      */

      class CCameraLookAtPoint: public CSystemCommands, public CEventHandler, public CAttributeHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(CCameraLookAtPoint);
         RWS_DECLARE_CLASSID(CCameraLookAtPoint);
         RWS_CATEGORY("Examples");
         RWS_DESCRIPTION("Camera Look At Point", "Example of a 3rd Person camera which follows and looks at a point");

         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);

         // Command associated with this class that HandleAttributes can modify
         //
         RWS_BEGIN_COMMANDS

            RWS_SEPARATOR("Messages", 1)

            RWS_MESSAGE( CMD_Set_m_Msg_Rx_Pos,
                     "Position update",
                     "Position of entity that this camera will follow.",
                     RECEIVE,
                     RwFrame*,
                     0)

            RWS_MESSAGE( CMD_Set_m_RxEnableCameraEvent,
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

            RWS_SEPARATOR("Distance Attributes", 0)

            RWS_ATTRIBUTE(CMD_m_max_distance,
                     "Max distance",
                     "Max distance from target entity",
                     SLIDER,
                     RwReal,
                     RANGE(0, 100, 1000))

            RWS_ATTRIBUTE(CMD_m_pref_distance,
                     "Preferred distance",
                     "Preferred distance from target entity",
                     SLIDER,
                     RwReal,
                     RANGE(0, 75, 1000))

            RWS_ATTRIBUTE(CMD_m_min_distance,
                     "Min distance",
                     "Min distance from target entity",
                     SLIDER,
                     RwReal,
                     RANGE(0, 25, 1000))

            RWS_ATTRIBUTE(CMD_m_smooth_distance,
                     "Distance smoothing",
                     "Distance smoothing factor",
                     SLIDER,
                     RwReal,
                     RANGE(0, 0.1, 1))

            RWS_SEPARATOR("Elevation Attributes", 0)
         
            RWS_ATTRIBUTE(CMD_m_max_elevation,
                     "Max elevation",
                     "Max elevation from entity",
                     SLIDER,
                     RwReal,
                     RANGE(-100, 10, 100))

            RWS_ATTRIBUTE(CMD_m_pref_elevation,
                     "Preferred elevation",
                     "Preferred elevation from entity",
                     SLIDER,
                     RwReal,
                     RANGE(-100, 5, 100))

            RWS_ATTRIBUTE(CMD_m_min_elevation,
                     "Min elevation",
                     "Min elevation from entity",
                     SLIDER,
                     RwReal,
                     RANGE(-100, -10, 100))

            RWS_ATTRIBUTE(CMD_m_smooth_elevation,
                     "Elevation smoothing",
                     "Elevation smoothing factor",
                     SLIDER,
                     RwReal,
                     RANGE(0, 0.1, 1))

            RWS_SEPARATOR("Angle Attributes", 0)
         
            RWS_ATTRIBUTE(CMD_m_max_angle,
                     "Max angle",
                     "Max angle relative to entity",
                     SLIDER,
                     RwReal,
                     RANGE(-360, 200, 360))

            RWS_ATTRIBUTE(CMD_m_pref_angle,
                     "Preferred angle",
                     "Preferred angle relative to entity",
                     SLIDER,
                     RwReal,
                     RANGE(-180, 180, 180))

            RWS_ATTRIBUTE(CMD_m_min_angle,
                     "Min angle",
                     "Min angle relative to entity",
                     SLIDER,
                     RwReal,
                     RANGE(-360, 160, 360))

            RWS_ATTRIBUTE(CMD_m_smooth_angle,
                     "Angle smoothing",
                     "Angle smoothing factor",
                     SLIDER, 
                     RwReal,
                     RANGE(0, 0.1, 1))

            RWS_SEPARATOR("View Attributes", 0)

            RWS_ATTRIBUTE(CMD_Set_View_Angle,
                     "Field of view",
                     "View Cone Angle",
                     SLIDER,
                     RwReal,
                     RANGE(0, 45, 180))

            RWS_SEPARATOR("Misc Attributes", 0)

            RWS_ATTRIBUTE(CMD_Set_StartsActive,
                     "Enable camera",  
                     "Enable/Disable camera", 
                     BOOLEAN, 
                     RwUInt32,
                     DEFAULT(1))

            RWS_SEPARATOR("Workspace Sync Camera Request", 1)

            RWS_ATTRIBUTE(CMD_Set_m_respondtocamerarequest, 
                     "Enable Respond to Workspace Sync Camera Request",
                     "Enable this camera responding to the workspace sync camera request.",
                     BOOLEAN,
                     RwUInt32,
                     DEFAULT(1))

         RWS_END_COMMANDS;

         CCameraLookAtPoint(const CAttributePacket& attr);
         virtual ~CCameraLookAtPoint();

      private:
         void Proc_m_Msg_Rx_Pos(const CMsg &pMsg);
         void Proc_iMsg_SetEnableDirectorsCamera(void);
         void Proc_iMsg_SetDisableDirectorsCamera(void);
         void Proc_m_RxEnableCameraEvent(void);
         void Proc_m_RxDisableCameraEvent(void);

         void HandleSystemCommands(const CAttributePacket &attr);

         bool m_this_camera_active;       /**< Flag indicates that this camera is current
                                               active, is overridden by 
                                               CConsoleEnvironment::m_bDirectorsCameraIsActive */
   
         CEventId m_Msg_Rx_Pos;           /**< Event used to specify the target the camera is 
                                               following, looking at */

         CEventId m_RxEnableCameraEvent;  /**< Event received to enable this camera */ 
         CEventId m_TxEnableCameraEvent;  /**< Event sent when this camera is enabled */
         CEventId m_RxDisableCameraEvent; /**< Event received to disable this camera */
         CEventId m_renderInMsg;          /**< Event for incoming render message. */
         RwUInt16 m_renderInPriority;     /**< Input render message's priority value, 0 = last. */
         CEventId m_renderOutMsg;         /**< Event for outgoing render message. */

         // Relative XZ Distance from target to camera
         //
         RwReal m_distance;               /**< Current distance XZ of camera from target */

         RwReal m_max_distance;           /**< Maximum allowed distance XZ of camera from target */
         RwReal m_min_distance;           /**< Minimum allowed distance XZ of camera from target */
         RwReal m_pref_distance;          /**< Preferred distance XZ of camera from target */
         RwReal m_smooth_distance;        /**< Smoothing value applied when updating the distance */

         // Relative Y Distance from target to camera
         //
         RwReal m_elevation;              /**< Current elevation Y of camera from target */

         RwReal m_max_elevation;          /**< Maximum allowed elevation Y of camera from target */
         RwReal m_min_elevation;          /**< Minimum allowed elevation Y of camera from target */
         RwReal m_pref_elevation;         /**< Preferred elevation Y of camera from target */
         RwReal m_smooth_elevation;       /**< Smoothing value applied when updating the elevation */

         // Relative angle between camera and object i.e. 0 degree = infront, 90 degrees = to the side, 180 = behind.
         //

         RwReal m_angle;                  /**< Current angle XZ of camera from target */
         RwReal m_lastAngle;              /**< Angle at previous call - used to correctly range new calculated value */
                                          
         RwReal m_max_angle;              /**< Minimum allowed angle XZ of camera from target */
         RwReal m_min_angle;              /**< Minimum allowed angle XZ of camera from target */
         RwReal m_pref_angle;             /**< Preferred angle XZ of camera from target. 0 = infront,
                                               90 = side, 180 = behind. */
         RwReal m_smooth_angle;           /**< Smoothing value applied when updating the angle */
                                          
         RwReal m_ViewWindow;             /**< Value calculated based on current view angle */
                                          
         CAtomicPtr m_pAtomic;            /**< Position of the camera. */

#ifdef RWS_DESIGN
         RwBool m_respondtocamerarequest;               /**< Design Mode only, enable/disable respond to
                                                             workspace sync camera request */
         RwFrame *m_cameraFrame;                        /**< Design Mode only, player camera RwFrame */
#endif
      };
   }//namespace Examples
}//namespace RWS

#endif
