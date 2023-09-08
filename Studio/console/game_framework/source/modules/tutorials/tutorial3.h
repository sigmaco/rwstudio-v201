/*****************************************************************************
*
* File :       Tutorial3.h
*
* Abstract :  Editing behavior attributes.
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
#include "framework/mainloop/logic.h"

#include "framework/core/attributehandler/allocation policies/LinearAllocationPolicy.h"

namespace RWS
{
   namespace Tutorial
   {
      /**
      *
      *  \ingroup Mod_Tutorials
      *
      *  Rotate an atomic object within a behavior, using RenderWare studio
      *  to control the amount of rotation.
      *
      *  \see CAttributeHandler
      *  \see CEventHandler
      *  \see CSystemCommands
      *  \see CAtomicPtr
      *
      */
      class CTutorial3 : public CSystemCommands, public CAttributeHandler , public CEventHandler, public LinearAllocationPolicy
      {
      public:
         RWS_MAKENEWCLASS(CTutorial3);
         RWS_DECLARE_CLASSID(CTutorial3);
         RWS_CATEGORY("Tutorial");
         RWS_DESCRIPTION("Tutorial3", "Tutorial 3");

         // The attribute definition for this behavior allows you to alter the amount the object is moved in each axis.
         //
         RWS_BEGIN_COMMANDS
            RWS_ATTRIBUTE( CMD_rot_x, "X Rotation", "Specify the x axis rotation", SLIDER, RwReal, RANGE(0,0,360))
            RWS_ATTRIBUTE( CMD_rot_y, "Y Rotation", "Specify the y axis rotation", SLIDER, RwReal, RANGE(0,180,360))
            RWS_ATTRIBUTE( CMD_rot_z, "Z Rotation", "Specify the z axis rotation", SLIDER, RwReal, RANGE(0,0,360))
         RWS_END_COMMANDS;

         CTutorial3(const CAttributePacket& attr);
         ~CTutorial3(void);

         virtual void HandleEvents(CMsg &pMsg);
         virtual void HandleAttributes(const CAttributePacket& attr);

      protected:
         RwMatrix *m_mat;        /**< Matrix used for rotating object. */
         CAtomicPtr m_pAtomic;   /**< Behavior's Atomic. */
         RwReal m_rot[3];        /**< RwReal array for storing rotation values. */
      };

   }//namespace Tutorial
}//namespace RWS