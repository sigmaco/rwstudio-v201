/*****************************************************************************
*
* File :     LightHelper.h
*
* Abstract : Helper functions for use with RpLights
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

#include "precomp.h"
#include "lighthelper.h"
#include "framework/core/macros/debugmacros.h"
#include "framehelper.h"

namespace RWS
{
   namespace LightHelper
   {
      /**
      *
      * \ingroup LightHelper
      *
      *  Destroy is used to safely destroy an RpLight, usually called by either the
      *  destructor of CLightPtr or by the function overload RwBool RpLightDestroy( RWS::CLightPtr &rCLightPtr ).
      *
      *  \li Detaches any child frames from the frame of the light.
      *  \li Deletes the frame.
      *  \li Removes the light from the world.
      *  \li Deletes the light.
      *
      *  \param pLight Pointer to an RpLight object.
      *
      *  \returns Returns TRUE if successful. 
      */
      RwBool Destroy(RpLight* pLight)
      {
         RWS_FUNCTION("RWS::CLightPtr::Destroy");
   
         RWS_PRE(pLight);
         
         RwFrame* pFrame = RpLightGetFrame(pLight);
         if (pFrame)
         {
            // Clean up any frame hierarchy attached to the light (the assumption is
            // that any child frames are managed by other objects, so it is sufficient
            // for us to simply detach them and let them 'float off')
            //
            RwFrameForAllChildren(pFrame, FrameHelper::RemoveChildFrame, 0);
            
            // If this frame is itself attached to another, detach it
            //
            if (RwFrameGetParent(pFrame)) RwFrameRemoveChild(pFrame);
            
            RpLightSetFrame(pLight, 0);
            FrameHelper::FrameDestroy(pFrame);
         }
         
         // Remove light from world
         RpWorld* pWorld = RpLightGetWorld (pLight);
         if (pWorld) RpWorldRemoveLight(pWorld, pLight);
         
         // RenderWare destroy light
         RwBool ret = RpLightDestroy(pLight);
         
         RWS_RETURN(ret);
      }

      /**
      *
      * \ingroup LightHelper
      *
      * Handles system commands for a RenderWare Graphics RpLight object i.e. 
      * CMD_LoadMatrix.
      *
      *  \param rLight Reference to an RenderWare Graphics RpLight object.
      *  \param rAttr Reference to a CAttributePacket containing the attribute data.
      *
      */
      void HandleSystemCommands(RpLight& rLight, const CAttributePacket& rAttr)
      {
         RWS_FUNCTION ("RWS::LightHelper::HandleSystemCommands");

         CAttributeCommandIterator attrIt(rAttr, RWS_CLASSID_OF(CSystemCommands));
         while (!attrIt.IsFinished())
         {
            if (attrIt->GetCommandId() == CSystemCommands::CMD_LoadMatrix)
            {
               RwFrame* pFrame = RpLightGetFrame(&rLight);
               if (pFrame) CSystemCommands::UpdateFrame(*pFrame, *attrIt);
            }
         
            ++attrIt;
         }

         RWS_RETURNVOID();
      }
   }
}
