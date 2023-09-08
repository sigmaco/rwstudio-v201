/*****************************************************************************
*
* File :     SkinSafe.h
*
* Abstract : Helper functions for making RpSkins 'safe' w.r.t animation input.
*
*            Skins normally crash if they're rendered without having had
*            animation data applied so there are functions that can use the
*            default pose of the skin to 'fake' appropriate input.
*
*            Also provides helpers for verifying the compatibility of
*            animation input.
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

#ifndef __SKINSAFE_H__
#define __SKINSAFE_H__


//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>
#include "rphanim.h"
#include "rpskin.h"

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "RpHAnim.lib")
#pragma comment (lib, "RpSkin.lib")
#endif

namespace RWS
{
   bool HierarchyCanAcceptDefaultPose( const RpHAnimHierarchy& hierarchy, const RpSkin& skin );

   // RWS_PRE( HierarchyCanAcceptDefaultPose( hierarchy, skin ) )
   void ApplyDefaultPoseToHierarchy( RpHAnimHierarchy& hierarchy, const RpSkin& skin );

   bool HierarchyStructureIsValid( const RpHAnimHierarchy& hierarchy );

   // RWS_PRE( HierarchyStructureIsValid(hierarchy) )
   bool HierarchyIsAttachable( const RpHAnimHierarchy& hierarchy, const RwFrame& rootFrame );

   bool HierarchyIsCompatibleWithSkin( const RpHAnimHierarchy& hierarchy, const RpSkin& skin );
   
   bool AnimationIsCompatibleWithHierarchy( const RpHAnimAnimation& anim, const RpHAnimHierarchy& hierarchy );
}


#endif
