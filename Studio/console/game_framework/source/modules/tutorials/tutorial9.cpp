/*****************************************************************************
*
* File :      Tutorial9.cpp
*
* Abstract :  Creating a behavior without using the CAtomicPtr object.
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
#include "precomp.h"
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "tutorial9.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/world/helpers/framehelper.h"

namespace RWS
{
   namespace Tutorial
   {
      RWS_IMPLEMENT_CLASSID(CTutorial9);
      RWS_REGISTERCLASS(CTutorial9);

      /**
      *
      *  Create CTutorial9 object.  Gets behaviors atomic by extracting information the behavior needs from the 
      *  attributes used to construct the behavior.
      *
      *  \param attr the standard attribute data.
      *
      */
      CTutorial9::CTutorial9(const CAttributePacket& attr) : InitCEventHandler(0)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial9::CTutorial9");

         m_pAtomic = CreateAtomicInWorldFromResource(attr, this);

         RWS_ASSERT(m_pAtomic, "Failed to create atomic");

#ifdef RWS_EVENTVISUALIZATION
         CEventHandler::Set_m_pCEventVisualization(this);   // Provide CEventVisualization interface
#endif

         RWS_RETURNVOID();
      }   

      /**
      *
      *  Destroy CTutorial9 object
      *
      */
      CTutorial9::~CTutorial9(void)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial9::~CTutorial9");

         // Safely destroy the Atomic
         if (m_pAtomic)
         {
            RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic);
            if (pFrame)
            {
             // Clean up any frame hierarchy attached to the atomic (the assumption is
             // that any child frames are managed by other objects, so it is sufficient
             // for us to simply detach them and let them 'float off').
             //
             RwFrameForAllChildren(pFrame, FrameHelper::RemoveChildFrame, 0);

             // If this frame is itself attached to another, detach it.
             //
             if (RwFrameGetParent(pFrame)) RwFrameRemoveChild(pFrame);

             RpAtomicSetFrame(m_pAtomic, 0);
             FrameHelper::FrameDestroy(pFrame);
            }

            // Remove atomic from world.
            RpWorld* pWorld = RpAtomicGetWorld (m_pAtomic);
            if (pWorld) RpWorldRemoveAtomic(pWorld, m_pAtomic);

            // RenderWare destroy atomic.
            RpAtomicDestroy(m_pAtomic);
         }

         RWS_RETURNVOID();
      }
   
      /**
      *
      *  Handle events, processes message events.
      *
      * \param pMsg standard message data.
      *
      */
      void CTutorial9::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial9::HandleEvents");

         RWS_RETURNVOID();
      }
  
      /**
      *
      *  Handle attributes, processes attribute changes.
      *
      * \param attr standard attribute packets.
      *
      */
      void CTutorial9::HandleAttributes(const CAttributePacket& attr)
      {
         RWS_FUNCTION("RWS::Tutorial::CTutorial9:HandleAttributes");

         CAttributeHandler::HandleAttributes(attr);

         // handle system commands,  e.g. if object moved from within RenderWare studio, then
         // move them on console.    
         CAttributeCommandIterator attrIt(attr, RWS_CLASSID_OF(CSystemCommands));
         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
            case CSystemCommands::CMD_LoadMatrix:
               {
                  RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic);
                  if (pFrame) CSystemCommands::UpdateFrame(*pFrame, *attrIt);
               }
               break;
            case CSystemCommands::CMD_SetSolidFlag:
               {
                  RwUInt32 flag;
                  attrIt->GetCommandData(flag);
                  RpAtomicCollisionProperties::SetIsSolid( *m_pAtomic, flag?true:false );
               }
               break;
            case CSystemCommands::CMD_SetInvisibleFlag:
               {
                  RwUInt32 flag;
                  attrIt->GetCommandData(flag);
                  AtomicHelper::SetIsVisible( *m_pAtomic, flag?false:true);
               }
               break;
            case CSystemCommands::CMD_SetCollisionFlag:
               {
                  RwUInt32 flag;
                  attrIt->GetCommandData(flag);
                  AtomicHelper::SetCanCollide( *m_pAtomic, flag?true:false);
               }
               break;
            }
         
            ++attrIt;
         } 

         RWS_RETURNVOID();
      }

#ifdef RWS_EVENTVISUALIZATION
      /**
      *
      * Mandatory CEventVisualization interface.
      *
      * \return A pointer to a RwV3d object, containing the world co-ordinates of the RpAtomic.
      *
      * \note Get3dPos is only available if RWS_EVENTVISUALIZATION is defined, this function is
      * only intended to be used during development by the event Visualization system. The smart
      * pointers can be used to provide the implementation of GetWorldPos but here it is shown
      * being provided by the behavior.
      *
      */
      RwV3d *CTutorial9::GetWorldPos(void)
      {
         RWS_FUNCTION("RWS::CTutorial9::Get3dPos");
         
         if (m_pAtomic)
         {
            RwFrame *pFrame = RpAtomicGetFrame(m_pAtomic);
            
            if (pFrame)
            {
               RwMatrix *pMatrix = RwFrameGetLTM(pFrame);
               
               RWS_RETURN(RwMatrixGetPos(pMatrix));
            }
         }
         RWS_RETURN(0);
      }
#endif

   }//namespace Tutorial
}//namespace RWS
