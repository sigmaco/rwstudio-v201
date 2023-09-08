/*****************************************************************************
 *
 * File :     FPSEscalator.cpp
 *
 * Abstract : 
 *
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
// Left to its own devices, the MW linker will dead-strip the registrar ctors, so...
#ifdef __MWERKS__
#pragma force_active on
#pragma exceptions off // Turn off exceptions due to bug in MW Dwarf Generator for PS2, present in CW 3.04
#endif

//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

#include "fpsescalator.h"
#include "framework/mainloop/logic.h"
#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/world/clevel.h"
#include "framework/toolkits/debugtools/debugtools.h"
#include "framework/toolkits/world/helpers/framehelper.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace FPS
   {
      RWS_IMPLEMENT_CLASSID(FPSEscalator);
      RWS_REGISTERCLASS(FPSEscalator);

      /**
      *
      *  Private container used to store atomic position data pair, used by the FPSEscalator class
      *  
      */
      class EscalatorStep
      {
         public:
            RpAtomic *m_pAtomic;
            RwReal m_rPosition;
      };


      /**
      *
      *  \ref InitSteps(). Initialize the escalator, creates a clump containing m_uiNumSteps atomics. 
      *  Each atomic is a step on the escalator where m_rPosition determines its position in 
      *  the sequence 0 to 1.0.
      *
      */
      void FPSEscalator::InitSteps(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSEscalator::InitSteps");

         RwFrame* _pFrame = RpClumpGetFrame( m_pClump.ptr());

         RpClumpSetFrame( m_pClump.ptr(), 0);

         RwFrameForAllChildren( _pFrame, FrameHelper::RemoveChildFrame, 0);

         // Setting a CClumpPtr to 0 deletes any existing clump
         //
         m_pClump = 0;

         if (m_pSteps) RwFree(m_pSteps);

         // Allocate the atomics etc for the clump
         //
         m_pSteps = reinterpret_cast<EscalatorStep*>( RwMalloc(sizeof(EscalatorStep)*m_uiNumSteps, rwMEMHINTDUR_GLOBAL) );

         // Create a clump as the master object, this contains the world
         // position of the escalator.
         //
         m_pClump = RpClumpCreate();

         RWS_ASSERT(m_pClump, "RwClumpCreate failed");

         RpClumpSetFrame( m_pClump.ptr(), _pFrame );

         RwUInt32 _uiIndex;
         RwReal _rPosition;

         for ( _uiIndex = 0, _rPosition = 0.0f;
               _uiIndex < m_uiNumSteps;
               _uiIndex++, _rPosition += 1.0f / static_cast<RwReal>(m_uiNumSteps))
         {
            // Create the steps as atomics by cloning the template atomic, attach a frame to each
            // atomic and each of these frames attach to the clump atomic.
            //
            m_pSteps[_uiIndex].m_pAtomic = RpAtomicClone( m_pAtomicTemplate);

            RWS_ASSERT(m_pSteps[_uiIndex].m_pAtomic, "CreateAtomicFromResource failed for step index ( "
                  << _uiIndex << " )");

            RwFrame* _pAtomicFrame = RwFrameCreate();

            RWS_ASSERT(_pAtomicFrame, "RwFrameCreate failed for step index ( " << _uiIndex << " )");

            RwFrameSetIdentity( _pAtomicFrame );

            RpAtomicSetFrame( m_pSteps[_uiIndex].m_pAtomic, _pAtomicFrame);

            // Add the atomic as a child of the clump, when the clump is deleted these are
            // also deleted.
            //
            RpClumpAddAtomic( m_pClump, m_pSteps[_uiIndex].m_pAtomic);

            RwFrameAddChild( _pFrame, _pAtomicFrame);

            m_pSteps[_uiIndex].m_rPosition = _rPosition;
         }

         // Place clump in world
         //
         CLevel::AddClump(m_pClump);

         // Determine width/height of escalator using its bounding box
         //
         RpGeometry* _geom = RpAtomicGetGeometry( m_pAtomicTemplate);

         RWS_ASSERT(_geom, "RpAtomicGetGeometry failed");

         RpMorphTarget* _morph = RpGeometryGetMorphTarget(_geom, 0);

         RWS_ASSERT(_morph, "RpGeometryGetMorphTarget failed");

         RwBBox _bbox;

         RwBBoxCalculate(&_bbox, RpMorphTargetGetVertices(_morph), RpGeometryGetNumVertices(_geom));

         m_rHeight = static_cast<RwReal>(m_uiNumSteps) * (_bbox.sup.y - _bbox.inf.y);
         m_rWidth = static_cast<RwReal>(m_uiNumSteps) * (_bbox.sup.x - _bbox.inf.x);

         // Animate the steps to create initial position
         //
         UpdateSteps();

         RWS_RETURNVOID();
      }


      /**
      *
      * \ref FPSEscalator(). Construct the FPSEscalator object
      *
      * \param rAttr reference to a CAttributePacket
      *
      * \see ~FPSEscalator 
      *
      */
      FPSEscalator::FPSEscalator(const CAttributePacket& rAttr) :
         InitCEventHandler(&m_pClump),
         m_pSteps(0),
         eState(Idle)
      {
         RWS_FUNCTION("RWS::FPS::FPSEscalator::FPSEscalator");

         m_pAtomicTemplate = CreateAtomicFromResource(rAttr, this);

         // Need to create the clump
         //
         m_pClump = RpClumpCreate();

         RWS_ASSERT(m_pClump, "RpClumpCreate failed");

         RwFrame* _pFrame = RwFrameCreate();

         RWS_ASSERT(_pFrame, "RwFrameCreate failed");

         RwFrameSetIdentity( _pFrame );

         RpClumpSetFrame( m_pClump.ptr(), _pFrame );

         RWS_RETURNVOID();
      }


      /**
      *
      * \ref ~FPSEscalator(). Destroy the FPSEscalator object.
      *
      * \see FPSEscalator
      *
      */
      FPSEscalator::~FPSEscalator()
      {
         RWS_FUNCTION("RWS::FPS::FPSEscalator::~FPSEscalator");

         // Free memory allocated to steps, note this only releases the pointers to the atomics, the actual
         // atomics are deleted when the CClumpPtr is deleted (Done in the destructor).
         //
         if (m_pSteps) RwFree(m_pSteps);

         // Release events
         //
         UnLinkMsg(iMsgRunningTick);
         UnLinkMsg(m_ToggleEvent);

         UnRegisterMsg(m_ToggleEvent);

         RWS_RETURNVOID();
      }

      /**
      *
      *  \ref HandleAttributes(). Handle attributes for FPSEscalator
      *
      *  \param rAttr reference to a CAttributePacket
      *
      */
      void FPSEscalator::HandleAttributes(const CAttributePacket& rAttr)
      {
         RWS_FUNCTION("RWS::FPS::FPSEscalator::HandleAttributes");

         CAttributeHandler::HandleAttributes(rAttr);

         CAttributeCommandIterator attrIt(rAttr, RWS_CLASSID_OF(FPSEscalator));

         while (!attrIt.IsFinished())
         {
            switch (attrIt->GetCommandId())
            {
               case CMD_limitlow:
                  attrIt->GetCommandData( m_rLimitLow);
                  break;

               case CMD_limithig:
                  attrIt->GetCommandData( m_rLimitHig);
                  break;

               case CMD_height_mod:
                  attrIt->GetCommandData( m_rHeightMod);
                  break;

               case CMD_width_mod:
                  attrIt->GetCommandData( m_rWidthMod);
                  break;

               case CMD_velocity:
                  attrIt->GetCommandData( m_rVelocity);
                  break;

               case CMD_num_steps:
                  attrIt->GetCommandData( m_uiNumSteps);
                  InitSteps();
                  break;

               case CMD_toggle:
                  ReplaceLinkedMsg(m_ToggleEvent, attrIt->GetAs_RwChar_ptr(), 0);
                  break;
            }
            ++attrIt;
         }

         // Allow the clump to handle the system commands here, so that the position of the clump
         // is set after InitSteps has been called.
         //
         m_pClump.HandleSystemCommands( rAttr);

         RWS_RETURNVOID();
      }


      /**
      *
      *  \ref UpdateSteps(). Update the position of each of the escalator steps, each step has its 
      *  own RwFrame which is a child of the clumps frame which allows the atomics to be trivially 
      *  positioned with the world transformation handled by the clumps frame.
      *
      *
      */
      void FPSEscalator::UpdateSteps(void)
      {
         RWS_FUNCTION("RWS::FPS::FPSEscalator::UpdateSteps");

         RwReal _rWidth = m_rWidth * m_rWidthMod;
         RwReal _rHeight = m_rHeight * m_rHeightMod;
         RwV3d _v;

         _v.z = 0;

         // Loop over each step update its position
         //
         for (RwUInt32 _uiIndex = 0;_uiIndex<m_uiNumSteps; _uiIndex++)
         {
            RwFrame *_pFrame = RpAtomicGetFrame( m_pSteps[_uiIndex].m_pAtomic );

            m_pSteps[_uiIndex].m_rPosition += m_rVelocity;

            // Check whether each stair has wrapped in either direction p<0 or p>1 ?
            //
            if (m_pSteps[_uiIndex].m_rPosition > 1.0f) 
            {
               m_pSteps[_uiIndex].m_rPosition -= 1.0f;

               // *1* Unlink any child frames, i.e. if a player object is on this step when it loops
               // the player object should remain where it is.
               //
               RwFrameForAllChildren( _pFrame, FrameHelper::RemoveChildFrame, 0);
            }
            else
            if (m_pSteps[_uiIndex].m_rPosition < 0.0f) 
            {
               m_pSteps[_uiIndex].m_rPosition += 1.0f;

               // (see *1*)
               //
               RwFrameForAllChildren( _pFrame, FrameHelper::RemoveChildFrame, 0);
            }

            RwReal _t =  m_pSteps[_uiIndex].m_rPosition;

            _v.x = _t * _rWidth;

            // Apply clamping to height, generates lead in/lead out steps
            //          . 1.0
            //         .
            //        ..... Limit High
            //       .
            //      .
            //  .... Limit Low
            //    .
            //   . 0.0
            //
            if (_t < m_rLimitLow) 
               _t = 0;//m_rLimitLow;
            else
               _t -= m_rLimitLow;   // Clamp the bottom step of the escalator to the root frame.

            if (_t > m_rLimitHig) 
               _t = m_rLimitHig;

            _v.y = _t * _rHeight;

            RwFrameTranslate( _pFrame, &_v, rwCOMBINEREPLACE);
         }

         RWS_RETURNVOID();
      }

      /**
      *
      *  \ref HandleEvents(). Handle FPSEscalator events, tick, start, stop
      *
      *  \param msg Standard Message Package
      *
      *  \ref iMsgRunningTick
      *
      */
      void FPSEscalator::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::FPS::FPSEscalator::HandleEvents");

         if (pMsg.Id == iMsgRunningTick)
         {
            UpdateSteps();
         }
         else
         {
            switch (eState)
            {
               case Idle:
                  if (pMsg.Id == m_ToggleEvent)
                  {
                     LinkMsg(iMsgRunningTick);
                     eState = Active;
                  }
                  break;
               case Active:
                  if (pMsg.Id == m_ToggleEvent)
                  {
                     UnLinkMsg(iMsgRunningTick);
                     eState = Idle;
                  }
                  break;
            }
         }

         RWS_RETURNVOID();
      }
   }//namespace FPS
}//namespace RWS
