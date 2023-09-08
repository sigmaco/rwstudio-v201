/*****************************************************************************
*
* File :      CMorphAnimate.cpp
*
* Abstract :
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
// RenderWare Includes
//
#include <rpmorph.h>

#ifdef _MSC_VER
#pragma comment (lib, "rpmorph.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "cmorphanimate.h"

#include "framework/core/macros/debugmacros.h"
#include "framework/toolkits/math/maths.h"
#include "framework/toolkits/world/factory.h"

namespace RWS
{
   namespace Examples
   {
      RWS_IMPLEMENT_CLASSID(CMorphAnimate);
      RWS_REGISTERCLASS(CMorphAnimate);
      
      namespace 
      {
         /*
         *  Initialize each interpolator callback
         */
         RwReal InterpolatorCallback(RpAtomic *atomic, RwReal position)
         {
            RwInt32 curIpIndex, newIpIndex, NumInterpolators;
            RwReal newPosition, scale;
            
            scale = RpInterpolatorGetScale(RpAtomicGetInterpolator(atomic));
            newPosition = position - scale;
            
            RpGeometry* pGeometry = RpAtomicGetGeometry (atomic);
            NumInterpolators = (RpGeometryGetNumMorphTargets (pGeometry) - 1) * 2;
            
            curIpIndex = RpMorphAtomicGetCurrentInterpolator(atomic);
            
            newIpIndex = (curIpIndex + 1) % NumInterpolators;
            
            RpMorphAtomicSetCurrentInterpolator(atomic, newIpIndex);
            
            return newPosition;
         }
         
         /*
         *  Initialize the interpolators for each atomic
         */
         RpAtomic * AtomicInitializeInterpolators(RpAtomic *atomic, void* unused)
         {
            RpGeometry *geometry;
            
            geometry = RpAtomicGetGeometry(atomic);
            
            if ( geometry )
            {
               RwInt32 NumMorphTargets = RpGeometryGetNumMorphTargets(geometry);
               
               if ( NumMorphTargets > 1 )
               {
                  RwInt32 i;
                  
                  /*
                  * Create interpolators...
                  */
                  RpMorphGeometryCreateInterpolators(geometry, (NumMorphTargets * 2)-1);
                  
                  /*
                  * Link all morph targets...
                  */
                  for ( i = 0; i < (NumMorphTargets - 1); i++ )
                  {
                     RpMorphGeometrySetInterpolator(geometry, i, i, i + 1, 1.0f);
                  }
                  /*
                  * Link final morph target to fisrt creating continuos loop...
                  */
                  RpMorphGeometrySetInterpolator(geometry, i, i, 0, 1.0f);            
                  
                  RpMorphAtomicSetCurrentInterpolator(atomic, 0);
                  
                  RpMorphGeometrySetCallBack(geometry, InterpolatorCallback);
               }
            }
            
            return atomic;
         }
         
         /*
         *  Initialize interpolators
         */
         void ClumpInitializeInterpolators(RpClump *clump)
         {
            RpClumpForAllAtomics(clump, AtomicInitializeInterpolators, 0);
         }
         
         /*
         *  Callback used to update the morph time of each atomic.
         */
         RpAtomic * AtomicMorphAddTime (RpAtomic * atomic, void *data)
         {
            RwReal *rDelta = static_cast<RwReal*>(data);
            
            RpMorphAtomicAddTime(atomic, *rDelta);
            
            return atomic;
         }
      }
      
      /**
      *
      *  Construct CMorphAnimate object. Links to messages and setups atomic.
      *
      *  \attr standard attribute data.
      *
      */
      CMorphAnimate::CMorphAnimate(const CAttributePacket& attr)
         : InitCEventHandler(&m_pClump),
         m_rDelta(0.1f)
      {
         RWS_FUNCTION("RWS::Examples::CMorphAnimate::CMorphAnimate");
         
         m_pClump = CreateClumpInWorldFromResource(attr, this);
         
         ClumpInitializeInterpolators(m_pClump);
         
         LinkMsg(iMsgRunningTick);
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Destroy CMorphAnimate object. Unlinks from messages and cleans up atomic (via smart pointer's destructor).
      *
      */
      CMorphAnimate::~CMorphAnimate(void)
      {
         RWS_FUNCTION("RWS::Examples::CMorphAnimate::~CMorphAnimate");
         
         UnLinkMsg(iMsgRunningTick);
         
         RWS_RETURNVOID();
      }
      
      /**
      *
      *  Handle events, updates the morph as the control message is received.
      *  
      *  \param pMsg a standard message packet.
      *
      */
      void CMorphAnimate::HandleEvents(CMsg &pMsg)
      {
         RWS_FUNCTION("RWS::Examples::CMorphAnimate::HandleEvents");
         
         if (pMsg.Id == iMsgRunningTick)
         {
            RpClumpForAllAtomics(m_pClump, AtomicMorphAddTime, &m_rDelta);
         }
         
         RWS_RETURNVOID();
      }
      
      
      /**
      *
      *  Handle attributes, updates the delta value.
      *
      *  \param rAttr standard attribute data packet.
      *  
      */
      void CMorphAnimate::HandleAttributes(const CAttributePacket& rAttr)
      {
         RWS_FUNCTION("RWS::Examples::CMorphAnimate::HandleAttributes");
         
         m_pClump.HandleSystemCommands(rAttr);
         
         CAttributeCommandIterator attrIt(rAttr, RWS_CLASSID_OF(CMorphAnimate));
         
         while (!attrIt.IsFinished())
         {
            if (attrIt->GetCommandId() == CMD_rDelta)
            {
               attrIt->GetCommandData( m_rDelta);
            }
            ++attrIt;
         }
         
         RWS_RETURNVOID();
      }
   }//namespace Examples
}//namespace RWS
