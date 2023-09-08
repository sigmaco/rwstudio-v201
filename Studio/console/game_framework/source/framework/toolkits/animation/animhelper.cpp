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
#include "precomp.h"
//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "animhelper.h"
#include "skinsafe.h"
#include "framework/core/macros/debugmacros.h"

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include <rwcore.h>
#include <rphanim.h>
#include <rpskin.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpworld.lib")
#pragma comment (lib, "rpskin.lib")
#endif

namespace RWS
{
   /**
   *
   *  \ingroup Animation
   *
   *  Searches recursively for the first RpHAnimHierarchy attached to 
   *  an RwFrame.
   *
   *  \param pFrame Pointer to a RenderWare Gfx RwFrame object.
   *
   *  \param pData Pointer to additional data, specifically a pointer to a
   *  RenderWare Gfx RpHAnimHierarchy object.
   *
   *  \return Returns 0 if no hierarchy is attached, otherwise returns a pointer to a RwFrame.
   *
   */
   RwFrame* FindHAnimHierarchyCallback(RwFrame* pFrame, void* pData)
   {
      RWS_FUNCTION("RWS::FindHAnimHierarchyCallback")

      RWS_PRE(pFrame);
      RWS_PRE(pData);

      RpHAnimHierarchy*& pHierarchy = *reinterpret_cast<RpHAnimHierarchy**>(pData);
      pHierarchy = RpHAnimGetHierarchy(pFrame);
      if (0 == pHierarchy) RwFrameForAllChildren(pFrame, FindHAnimHierarchyCallback, pData);

      RWS_RETURN(0 == pHierarchy ? pFrame : 0);
   }

   /**
   *
   *  \ingroup Animation
   *
   *  Attaches an RpHAnimHierarchy to an atomic.
   *
   *  \param pAtomic Pointer to a RenderWare Gfx RpAtomic object.
   *
   *  \param pData Pointer to additional data, specifically a pointer to a
   *  RenderWare Gfx RpHAnimHierarchy object.
   *
   *  \return Returns pAtomic.
   *
   */
   RpAtomic* AttachHAnimHierarchyCallback(RpAtomic* pAtomic, void* pData)
   {
      RWS_FUNCTION("RWS::AttachHAnimHierarchyCallback")

      RWS_PRE(pAtomic);
      RWS_PRE(pData);

      RpSkin* pSkin = RpSkinAtomicGetSkin(pAtomic);
      if (pSkin)
      {
         RpHAnimHierarchy* pHierarchy = static_cast<RpHAnimHierarchy*>(pData);
         RWS_ASSERT( HierarchyIsCompatibleWithSkin(*pHierarchy, *pSkin), "Incompatible hierarchy" )
         RpSkinAtomicSetHAnimHierarchy(pAtomic, pHierarchy);
      }

      RWS_RETURN(pAtomic);
   }
}
