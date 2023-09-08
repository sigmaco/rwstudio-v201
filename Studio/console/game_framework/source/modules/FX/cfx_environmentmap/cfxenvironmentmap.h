/*****************************************************************************
 *
 * File :     CFXEnvironmentMap.h
 *
 * Abstract : A behavior that allows control over RenderWare materials
 *            that contain environment maps
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

#ifndef __CFXENVIRONMENTMAP_H__
#define __CFXENVIRONMENTMAP_H__

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

#include "cfxbaseenvironmentmap.h"
namespace RWS
{

   namespace FX
   {
      /**
      *
      *  \ingroup Mod_FX
      *
      *  CFXEnvironmentMap is a behavior that can be used to alter the appearance of environment maps on assets
      *  that has been associated with the behavior.
      *  The appearance of the environment map is changed by altering the data source used to generate the environment
      *  map frame
      *  The different source types have the following effects
      *  SOURCE_NONE       - No source data is supplied so the environment map frame is set to identity.
      *  SOURCE_CAMERA     - The environment map frame is set to the current camera frame.
      *  SOURCE_ANIMATED   - The environment map frames matrix is multiplied by the rotation matrix specified by m_RotationMatrix each time iMsgDoRender is received
      *  SOURCE_USER       - The environment map frame is set the rotation matrix described by m_RotationMatrix
      *  
      *
      */
      class CFXEnvironmentMap: public CFXBaseEnvironmentMap, public CSystemCommands, public CAttributeHandler, public CEventHandler

      {
      public:

         RWS_MAKENEWCLASS(CFXEnvironmentMap);
         RWS_DECLARE_CLASSID(CFXEnvironmentMap);
         RWS_CATEGORY("FX");
         RWS_DESCRIPTION("Environment Map", "Environment map effect behavior");

         RWS_BEGIN_COMMANDS

            RWS_MESSAGE  (CMD_UpdateEvent,            "Update Event","Update event used to trigger the update of the animation for the environment map", RECEIVE, RwCamera*, "iMsgDoRender" )
            RWS_ATTRIBUTE(CMD_Set_m_Source,           "Source",      "Set source",   LIST,    RwUInt32,   LIST("NONE|@CAMERA|ANIMATED|USER"))

            RWS_SEPARATOR("User", 0)
            RWS_ATTRIBUTE(CMD_Set_m_UserRotation_x,   "X Rotation",  "Amount of rotation in x axis",   SLIDER,  RwReal,RANGE(0, 0, 360))
            RWS_ATTRIBUTE(CMD_Set_m_UserRotation_y,   "Y Rotation",  "Amount of rotation in y axis",   SLIDER,  RwReal,RANGE(0, 0, 360))
            RWS_ATTRIBUTE(CMD_Set_m_UserRotation_z,   "Z Rotation",  "Amount of rotation in z axis",   SLIDER,  RwReal,RANGE(0, 0, 360))

            RWS_SEPARATOR("Animated", 0)

            RWS_ATTRIBUTE(CMD_Set_m_AnimRotation_x,   "X Rotation",  "Amount of rotation in x axis",   SLIDER,  RwReal,RANGE(-90, 0, 90))
            RWS_ATTRIBUTE(CMD_Set_m_AnimRotation_y,   "Y Rotation",  "Amount of rotation in y axis",   SLIDER,  RwReal,RANGE(-90, 0, 90))
            RWS_ATTRIBUTE(CMD_Set_m_AnimRotation_z,   "Z Rotation",  "Amount of rotation in z axis",   SLIDER,  RwReal,RANGE(-90, 0, 90))

            RWS_SEPARATOR("Scale", 0)

            RWS_ATTRIBUTE(CMD_Set_m_Scale,            "Scale",       "Amount of scale",                SLIDER,  RwReal,RANGE(-2.0, 1.0, 2.0))
            RWS_ATTRIBUTE(CMD_Set_m_OffsetX,          "Offset X",    "Amount of offset in x",          SLIDER,  RwReal,RANGE(-2.0, 1.0, 2.0))
            RWS_ATTRIBUTE(CMD_Set_m_OffsetY,          "Offset Y",    "Amount of offset in y",          SLIDER,  RwReal,RANGE(-2.0, 1.0, 2.0))
                                                                                                                              
         RWS_END_COMMANDS;

         CFXEnvironmentMap                      (const CAttributePacket& rAttr);
         ~CFXEnvironmentMap                     (void);
         virtual void         HandleEvents      (CMsg &pMsg);
         virtual void         HandleAttributes  (const CAttributePacket& attr);
         void                 UpdateEnvFrame    (void);


      protected:

         enum SOURCE_TYPES
         {
            SOURCE_NONE = 0,                    /**< Sets the environment map frame to identity. */
            SOURCE_CAMERA,                      /**< Sets the environment map frame to the camera frame. */
            SOURCE_ANIMATED,                    /**< Set the environment map frame to be rotated based on attributes set in the workspace. */
            SOURCE_USER                         /**< Allows the user to specify a set of x,y,z rotations for the environment map. */
         };

         SOURCE_TYPES   m_Source;               /**< Specifies the source data used to construct the environment map frames matrix. */
         RwV3d          m_UserRotation;         /**< Rotation vector used to generate m_RotationMatrix when in user mode. */
         RwV3d          m_AnimRotation;         /**< Rotation vector used to generate m_RotationMatrix when in animation mode. */

         RwReal         m_Scale;                /**< The matrix used to generate the environment map texture coordinates is ortho-normalized on the PS2, therefore the scale value will have no effect on the PS2*/

         RwFrame       *m_pEnvFrame;             /**< The frame used to orientate the environment map. */
         RwMatrix      *m_pRotationMatrix;       /**< The matrix that describes the amount to rotate the environment map frame by each game frame. */
         RwUInt8        m_OrthoCount;           /**< Counter used to keep track of when the environment map frame needs orthonormalizing. */
         CEventId       m_UpdateEvent;          /**< Event ID of the event used to update the animation for the environment map */
      };

   } // namespace FX
      
} // namespace RWS

#endif
