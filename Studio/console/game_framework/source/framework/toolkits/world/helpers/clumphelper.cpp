/*****************************************************************************
*
* File :     ClumpHelper.cpp
*
* Abstract : Helper functions for use with RpClump
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
#include "clumphelper.h"
#include "atomichelper.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/core/attributehandler/csystemcommands.h"
#include "framework/core/eventhandler/ceventhandler.h"

namespace RWS
{
   namespace ClumpHelper
   {
#ifndef NDEBUG
      namespace
      {
         bool bOpenFlag = false;
      }
#endif

      CEventId iMsgPreRpClumpDestroy;


      /**
      *
      *  Open ClumpHelper, this must be called to initialize ClumpHelper,
      *  as it registers the iMsgPreRpClumpDestroy event.
      *
      */
      void Open()
      {
         RWS_FUNCTION("RWS::ClumpHelper::Open");

#ifndef NDEBUG
         bOpenFlag = true;
#endif

         CEventHandler::RegisterMsg ( iMsgPreRpClumpDestroy, iMsgPreRpClumpDestroyStr,"RwFrame*");

         RWS_RETURNVOID();
      }


      /**
      *
      *  Close ClumpHelper, this must be called to shutdown ClumpHelper,
      *  as it unregisters the iMsgPreRpClumpDestroy event.
      */
      void Close()
      {
         RWS_FUNCTION("RWS::ClumpHelper::Open");

#ifndef NDEBUG
         bOpenFlag = false;
#endif

         CEventHandler::UnRegisterMsg ( iMsgPreRpClumpDestroy ); 

         RWS_RETURNVOID();
      }


      /**
      *  \ingroup ClumpHelper
      *
      *  This is used to safely destroy an RpClump, usually called by either the
      *  destructor of CClumpPtr or by the function overload RwBool RpClumpDestroy( RWS::CClumpPtr &rCClumpPtr )
      *
      * \li Deletes the frame associated with the clump, deconstruct the 
      * frame hierarchy and deletes any frames belonging to the clump.
      * \li Deletes all atomics within the clump
      * \li Removes the clump from the world.
      * \li Deletes the clump.
      *
      *  \param pClump Pointer to an RpClump object.
      *
      *  \returns Returns TRUE if successful. 
      */
      RwBool Destroy(RpClump* pClump)
      {
         RWS_FUNCTION("RWS::ClumpHelper::Destroy");

         RWS_ASSERT(bOpenFlag, "RWS::ClumpHelper::Open hasn't been called");

         RWS_PRE(pClump);

         /*
         *
         *  Game code may have modified the frame hierarchy of the clump, there is no way to determine
         *  at this point if something has been added to the hierarchy that wasn't part of the original clump,
         *  if RpClumpDestroy is allowed to proceed before removing any additions to the hierarchy the added
         *  frames will also be destroyed, this event is sent to enable behaviors to check if they are attached
         *  to this hierarchy and remove themselves.
         *
         */
         RwFrame* pFrame = RpClumpGetFrame(pClump);

         CMsg msg(iMsgPreRpClumpDestroy, pFrame);

         SendMsg(msg);

         if (pFrame)
         {
            // If the clump is attached to any parent frame detach it
            //
            if (RwFrameGetParent(pFrame)) RwFrameRemoveChild(pFrame);
         }

         // Remove clump from world, if necessary
         RpWorld* pWorld = RpClumpGetWorld(pClump);
         if (pWorld) RpWorldRemoveClump(pWorld, pClump);
   
         RwBool ret = RpClumpDestroy(pClump);

         RWS_RETURN(ret);
      }

      /*
      *
      *  \ingroup ClumpHelper
      *
      *  Callback used to set the visibility of the atomic, called by SetIsVisible( RpClump& clump, bool isVisible )
      *
      *  \param pAtomic Pointer to a RenderWare Graphics RpAtomic object.
      *  \data pData Enable/Disable visibility, \see SetIsVisible.
      */
      RpAtomic* SetIsVisibleCallback(RpAtomic* pAtomic, void* pData)
      {
         RWS_FUNCTION("RWS::ClumpHelper::SetIsVisibleCallback");

         RWS_PRE(pAtomic);
         AtomicHelper::SetIsVisible(*pAtomic, 0 != pData);

         RWS_RETURN(pAtomic);
      }

      /**
      *
      *  \ingroup ClumpHelper
      *
      *  Sets the visibility of all the atomics in the clump.
      *
      *  \param clump Reference to a RenderWare Graphics RpClump object.
      *  \param isVisible New state of visibility flag, true is visible.
      *
      */
      void SetIsVisible( RpClump& clump, bool isVisible )
      {
         RWS_FUNCTION("RWS::ClumpHelper::SetIsVisible");

         RpClumpForAllAtomics(&clump, SetIsVisibleCallback, isVisible ? reinterpret_cast<void*>(1) : 0);

         RWS_RETURNVOID();
      }

      /*
      *
      *  \ingroup ClumpHelper
      *
      *  Sets the collide ability of the atomics within a clump, i.e. enable/disable collision test on this atomic
      *
      *  \param pAtomic Pointer to a RenderWare Graphics RpAtomic object.
      *  \param pData Enable/Disable collisions flag, \see SetCanCollide.
      */
      RpAtomic* SetCanCollideCallback(RpAtomic* pAtomic, void* pData)
      {
         RWS_FUNCTION("RWS::ClumpHelper::SetIsVisibleCallback");

         RWS_PRE(pAtomic);
         AtomicHelper::SetCanCollide(*pAtomic, 0 != pData);

         RWS_RETURN(pAtomic);
      }

      /**
      *
      *  \ingroup ClumpHelper
      *
      *  Sets whether the atomics within the clump can collide or not
      *
      *  \param clump Reference to a RenderWare Graphics RpClump object.
      *  \param canCollide New state of can collide flag, true implies object will collide.
      */

      void SetCanCollide(RpClump& clump, bool canCollide)
      {
         RWS_FUNCTION("RWS::ClumpHelper::SetCanCollide");

         RpClumpForAllAtomics(&clump, SetCanCollideCallback, canCollide ? reinterpret_cast<void*>(1) : 0);

         RWS_RETURNVOID();
      }

      /*
      *
      *  \ingroup ClumpHelper
      *
      *  Sets the event handler for the atomic
      *
      *  \param pAtomic Reference to a RenderWare Graphics RpAtomic object.
      */
      RpAtomic* SetEventHandlerCallback(RpAtomic* pAtomic, void* pData)
      {
         RWS_FUNCTION("RWS::ClumpHelper::SetEventHandlerCallback");

         RWS_PRE(pAtomic);
         RpAtomicToEventHandler::SetEventHandler(*pAtomic, static_cast<CEventHandler*>(pData));

         RWS_RETURN(pAtomic);
      }

      /**
      *
      *  \ingroup ClumpHelper
      *
      *  Sets the event handler for all the atomics in the clump
      *
      *  \param clump Reference to a RenderWare Graphics RpClump object.
      *  \param pEventHandler Pointer to the CEventHandler associated with this clump.
      */
      void SetEventHandler( RpClump& clump, CEventHandler* pEventHandler )
      {
         RWS_FUNCTION("RWS::ClumpHelper::SetEventHandler");

         RpClumpForAllAtomics(&clump, SetEventHandlerCallback, pEventHandler);

         RWS_RETURNVOID();
      }

      /**
      *
      *  \ingroup ClumpHelper
      *
      *  Similar to RpClumpRender, renders the atomics within a clump, if the atomic is within the view frustrum,
      *
      *  \param pClump Pointer to a RenderWare Graphics RpClump object.
      */
      void RenderAllVisibleAtomics(RpClump * pClump)
      {
         RWS_FUNCTION("RWS::ClumpHelper::RenderAllVisibleAtomics");
         
         RwLLLink           *cur, *end;
         
         RWS_PRE(pClump);
         
         cur = rwLinkListGetFirstLLLink(&pClump->atomicList);
         end = rwLinkListGetTerminator(&pClump->atomicList);
         
         while (cur != end)
         {
            RpAtomic           *apAtom = rwLLLinkGetData(cur, RpAtomic, inClumpLink);
            
            if (rwObjectTestFlags(apAtom, rpATOMICRENDER))
            {
               const RwSphere     *atomicBoundingSphere;
               
               atomicBoundingSphere = RpAtomicGetWorldBoundingSphere(apAtom);
               
               RWS_PRE(atomicBoundingSphere);
               
               if (RwCameraFrustumTestSphere((RwCamera *) RWSRCGLOBAL(curCamera),
                  atomicBoundingSphere) != rwSPHEREOUTSIDE)
               {
                  RpAtomicRender(apAtom);
               }
            }
            
            cur = rwLLLinkGetNext(cur);
         }
         
         RWS_RETURNVOID();
      }

      /**
      *
      *  \ingroup ClumpHelper
      *
      *  Handles system commands for a RenderWare Graphics RpClump object i.e. 
      *  CMD_LoadMatrix, CMD_SetSolidFlag, CMD_SetInvisibleFlag, CMD_SetCollisionFlag
      *
      *  \param rClump Reference to a RenderWare Graphics RpClump object.
      *  \param rAttr Reference to a CAttributePacket containing the attribute data.
      *
      */
      void HandleSystemCommands(RpClump& rClump, const CAttributePacket& rAttr)
      {
         RWS_FUNCTION("RWS::ClumpHelper::HandleSystemCommands");

         CAttributeCommandIterator attrIt(rAttr, RWS_CLASSID_OF(CSystemCommands));
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CSystemCommands::CMD_LoadMatrix:
               {
                  RwFrame* pFrame = RpClumpGetFrame(&rClump);
                  if (pFrame) CSystemCommands::UpdateFrame(*pFrame, *attrIt);
               }
               break;
            case CSystemCommands::CMD_SetSolidFlag:
               {
                  RwUInt32 flag;
                  attrIt->GetCommandData(flag);
                  RpAtomicCollisionProperties::SetIsSolid(rClump, flag?true:false );
               }
               break;
            case CSystemCommands::CMD_SetInvisibleFlag:
               {
                  RwUInt32 flag;
                  attrIt->GetCommandData(flag);
                  SetIsVisible( rClump, flag?false:true);
               }
               break;
            case CSystemCommands::CMD_SetCollisionFlag:
               {
                  RwUInt32 flag;
                  attrIt->GetCommandData(flag);
                  SetCanCollide( rClump, flag?true:false);
               }
               break;
            }
         
            ++attrIt;
         }
         RWS_RETURNVOID();
      }
   }
}
