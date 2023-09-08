/*****************************************************************************
*
* File :     WorldHelper.h
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

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rwcore.h>
#include <rpworld.h>

#ifdef _MSC_VER
#pragma comment (lib, "rwcore.lib")
#pragma comment (lib, "rpworld.lib")
#endif

namespace RWS
{
   /**
   *
   *  \ingroup WorldHelper
   *
   *  Helper functions for RenderWare Graphics RpWorld object.
   *
   */
   namespace WorldHelper
   {
      void MoveAllObjects (RpWorld *_src_RpWorld, RpWorld *_dst_RpWorld);
   }
}//namespace