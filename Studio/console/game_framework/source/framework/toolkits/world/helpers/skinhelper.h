/*****************************************************************************
*
* File :     SkinHelper.h
*
* Abstract : Helper functions for dealing with RpSkin
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

#ifndef __SKINHELPER_H__
#define __SKINHELPER_H__

//////////////////////////////////////////////////////////////////
//
// RenderWare Includes
//
#include <rpskin.h>

#ifdef _MSC_VER
#pragma comment (lib, "rpskin.lib")
#endif


namespace RWS
{
   /**
   *
   *  \ingroup SkinHelper
   *
   *  Helper functions for RenderWare Graphics RpSkin object.
   *
   */
   namespace SkinHelper
   {
      /**
      *
      *  \ingroup SkinHelper
      *
      *  Prototype for RpSkin callbacks. Return NULL to prevent further
      *  callbacks being made on other RpSkins.
      *
      */
      typedef RpSkin* (*RpSkinCallBack)(RpSkin* pSkin, void* pData);

      void ForAllSkins( RpClump& clump, RpSkinCallBack callback, void* pData );

      RpSkin* FindFirstSkin(RpClump& clump);
   }
}

#endif