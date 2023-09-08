/*****************************************************************************
*
* File :     AtomicHelper.cpp
*
* Abstract : Helper functions for use with RpAtomics
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

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/core/macros/debugmacros.h"
#include "framehelper.h"

namespace RWS
{
   namespace AtomicHelper
   {
      /**
      *  \ingroup AtomicHelper
      *
      *  This is used to safely destroy an RpAtomic, usually called by either the
      *  destructor of CAtomicPtr or by the function overload
      *  RwBool RpAtomicDestroy( RWS::CAtomicPtr &rCAtomicPtr ).
      *
      *  \li Detaches any child frames from the frame of the pAtomic.
      *  \li Deletes the frame.
      *  \li Removes the pAtomic from the world.
      *  \li Deletes the pAtomic.
      *
      *  \param pAtomic Pointer to an RpAtomic object.
      *
      *  \return Returns TRUE if successful. 
      */
      RwBool Destroy(RpAtomic* pAtomic)
      {
         RWS_FUNCTION("RWS::AtomicHelper::Destroy");

         RWS_PRE(pAtomic);
   
         RwFrame* pFrame = RpAtomicGetFrame(pAtomic);
         if (pFrame)
         {
            // Clean up any frame hierarchy attached to the pAtomic (the assumption is
            // that any child frames are managed by other objects, so it is sufficient
            // for us to simply detach them and let them 'float off')
            //
            RwFrameForAllChildren(pFrame, FrameHelper::RemoveChildFrame,0);

            // If this frame is itself attached to another, detach it
            //
            if (RwFrameGetParent(pFrame)) RwFrameRemoveChild(pFrame);

            RpAtomicSetFrame(pAtomic, 0);
            FrameHelper::FrameDestroy(pFrame);
         }

         // If this pAtomic is also part of a clump then need do remove it from the clump
         //
         //
         RpClump *pClump = RpAtomicGetClump(pAtomic);

         if (pClump)
         {
            RpClumpRemoveAtomic(pClump, pAtomic);
         }
   
         // Remove pAtomic from world
         RpWorld* pWorld = RpAtomicGetWorld (pAtomic);
         if (pWorld) RpWorldRemoveAtomic(pWorld, pAtomic);
   
         // RenderWare destroy pAtomic
         RwBool ret = RpAtomicDestroy(pAtomic);

         RWS_RETURN(ret);
      }


      /**
      *
      *  \ingroup AtomicHelper
      *
      *  Sets the pAtomic's visibility, see RenderWare Graphics rpATOMICRENDER.
      *
      *  \note The pAtomic remains attached to world.
      *
      *  \param pAtomic Reference to an RenderWare Graphics RpAtomic object.
      *  \param isVisible Boolean flag, if true then the pAtomic is visible and will be rendered, otherwise
      *         the pAtomic is not rendered.
      *
      */
      void SetIsVisible( RpAtomic& pAtomic, bool isVisible )
      {
         RWS_FUNCTION("RWS::AtomicHelper::SetIsVisible");

         RwUInt32 flags = RpAtomicGetFlags(&pAtomic);

         if (isVisible) flags |= static_cast<RwUInt32>(rpATOMICRENDER);
         else flags &= ~ static_cast<RwUInt32>(rpATOMICRENDER);

         (void)RpAtomicSetFlags( &pAtomic, flags);

         RWS_RETURNVOID();
      }

      /**
      *
      *  \ingroup AtomicHelper
      *
      *  Sets the pAtomic's collide ability flag i.e. enable/disable collision tests with this pAtomic
      *
      *  \param pAtomic Reference to an RenderWare Graphics RpAtomic object.
      *  \param canCollide Boolean flag, if true then the atomic can collide, otherwise
      *         the atomic cannot collide.
      *
      */
      void SetCanCollide( RpAtomic& pAtomic, bool canCollide )
      {
         RWS_FUNCTION("RWS::AtomicHelper::SetCanCollide");

         RwUInt32 flags = RpAtomicGetFlags(&pAtomic);

         if (canCollide) flags |= static_cast<RwUInt32>(rpATOMICCOLLISIONTEST);
         else flags &= ~static_cast<RwUInt32>(rpATOMICCOLLISIONTEST);

         (void)RpAtomicSetFlags( &pAtomic, flags);

         RWS_RETURNVOID();
      }

      /**
      *
      * \ingroup AtomicHelper
      *
      * Handles system commands for a RenderWare Graphics RpAtomic object i.e. 
      * CMD_LoadMatrix, CMD_SetSolidFlag, CMD_SetInvisibleFlag, CMD_SetCollisionFlag
      *
      *  \param rAtomic Reference to a RenderWare Graphics RpAtomic object.
      *  \param rAttr Reference to a CAttributePacket containing the attribute data.
      *
      */
      void HandleSystemCommands(RpAtomic &rAtomic, const CAttributePacket& rAttr)
      {
         RWS_FUNCTION("RWS::AtomicHelper::HandleSystemCommands");

         CAttributeCommandIterator attrIt(rAttr, RWS_CLASSID_OF(CSystemCommands));
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CSystemCommands::CMD_LoadMatrix:
               {
                  RwFrame* pFrame = RpAtomicGetFrame(&rAtomic);
                  if (pFrame) CSystemCommands::UpdateFrame(*pFrame, *attrIt);
               }
               break;
            case CSystemCommands::CMD_SetSolidFlag:
               {
                  RwUInt32 flag;
                  attrIt->GetCommandData(flag);
                  RpAtomicCollisionProperties::SetIsSolid(rAtomic, flag?true:false );
               }
               break;
            case CSystemCommands::CMD_SetInvisibleFlag:
               {
                  RwUInt32 flag;
                  attrIt->GetCommandData(flag);
                  SetIsVisible( rAtomic, flag?false:true);
               }
               break;
            case CSystemCommands::CMD_SetCollisionFlag:
               {
                  RwUInt32 flag;
                  attrIt->GetCommandData(flag);
                  SetCanCollide( rAtomic, flag?true:false);
               }
               break;
            }
         
            ++attrIt;
         }
         RWS_RETURNVOID();
      }
   }
}
