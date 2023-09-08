/*****************************************************************************
*
* File :     AnimHelper.h
*
* Abstract : Inline helper functions for dealing with RpHAnim
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

#ifndef __ANIMHELPER_H__
#define __ANIMHELPER_H__

#include <rwcore.h>
#include <rpworld.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rtfsyst.lib")
#pragma comment (lib, "rpworld.lib")
#pragma comment (lib, "rtcmpkey.lib")
#endif

struct RpHAnimHierarchy;

namespace RWS
{
   RwFrame* FindHAnimHierarchyCallback(RwFrame* pFrame, void* pData);
   RpAtomic* AttachHAnimHierarchyCallback(RpAtomic* pAtomic, void* pData);

   /**
   *
   *  \ingroup Animation
   *
   *  Searches recursively for the first RpHAnimHierarchy attached to 
   *  an RwFrame. Returns NULL if no RpHAnimHierarchy is found.
   *
   */
   inline RpHAnimHierarchy* FindHAnimHierarchy(RwFrame& root)
   {
      RpHAnimHierarchy* pResult = 0;
      FindHAnimHierarchyCallback(&root,&pResult);
      return pResult;
   }

   /**
   *
   *  \ingroup Animation
   *
   *  Searches recursively for the first RpHAnimHierarchy attached to
   *  a clump's RwFrame hierarchy. Returns NULL if no RpHAnimHierarchy
   *  is found.
   *
   */
   inline RpHAnimHierarchy* FindHAnimHierarchy(RpClump& clump)
   {
      RwFrame* pClumpFrame = RpClumpGetFrame(&clump);
      return pClumpFrame ? FindHAnimHierarchy(*pClumpFrame) : 0;
   }

   /**
   *
   *  \ingroup Animation
   *
   *  Attaches an RpHAnimHierarchy to every atomic in a clump
   *
   */
   inline void AttachHAnimHierarchy(RpClump& clump, RpHAnimHierarchy& hierarchy)
   {
      RpClumpForAllAtomics(&clump,AttachHAnimHierarchyCallback,&hierarchy);
   }
}

#endif