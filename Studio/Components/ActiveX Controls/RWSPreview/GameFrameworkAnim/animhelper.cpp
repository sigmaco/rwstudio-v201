/*****************************************************************************
*
* File :     AnimHelper.cpp
*
* Abstract : Support for inline helper functions for dealing with RpHAnim
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

#include "stdafx.h"

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "animhelper.h"
#include "skinsafe.h"
#include "debugmacros.h"

namespace RWS
{
   /**
   *
   *  \ingroup Animation
   *
   *  Searches recursively for the first RpHAnimHierarchy attached to 
   *  an RwFrame.
   *
   */
   RwFrame* FindHAnimHierarchyCallback(RwFrame* pFrame, void* pData)
   {
      RWS_FUNCTION("RWS::FindHAnimHierarchyCallback")

      RWS_PRE(pFrame);
      RWS_PRE(pData);

      RpHAnimHierarchy*& pHierarchy = *reinterpret_cast<RpHAnimHierarchy**>(pData);
      pHierarchy = RpHAnimGetHierarchy(pFrame);
      if(0 == pHierarchy) RwFrameForAllChildren(pFrame,FindHAnimHierarchyCallback,pData);

      RWS_RETURN(0 == pHierarchy ? pFrame : 0);
   }

   /**
   *
   *  \ingroup Animation
   *
   *  Attaches an RpHAnimHierarchy to an atomic.
   *
   */
   RpAtomic* AttachHAnimHierarchyCallback(RpAtomic* pAtomic, void* pData)
   {
      RWS_FUNCTION("RWS::AttachHAnimHierarchyCallback")

      RWS_PRE(pAtomic);
      RWS_PRE(pData);

      RpSkin* pSkin = RpSkinAtomicGetSkin(pAtomic);
      if(pSkin)
      {
         RpHAnimHierarchy* pHierarchy = static_cast<RpHAnimHierarchy*>(pData);
         RWS_ASSERT( HierarchyIsCompatibleWithSkin(*pHierarchy,*pSkin), "Incompatible hierarchy" )
         RpSkinAtomicSetHAnimHierarchy(pAtomic,pHierarchy);
      }

      RWS_RETURN(pAtomic);
   }
}
