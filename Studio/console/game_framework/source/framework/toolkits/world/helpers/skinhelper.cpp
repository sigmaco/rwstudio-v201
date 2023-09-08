/*****************************************************************************
*
* File :     SkinHelper.cpp
*
* Abstract : Helper functions for dealing with RpClump
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
// Renderware Studio Includes
//
#include "skinhelper.h"
#include "framework/core/macros/debugmacros.h"

namespace RWS
{
   namespace SkinHelper
   {
      // Storage for RpSkin callback data
      struct SkinCallbackData
      {
         SkinCallbackData(RpSkinCallBack callback, void* pData)
            : m_callback(callback), m_pData(pData) {}

         RpSkinCallBack m_callback;
         void*          m_pData;
      };

      /**
      *
      *  \ingroup SkinHelper
      *
      *  Applies a callback to any skin in the atomic
      *
      */
      RpAtomic* ForAllSkinsCallback(RpAtomic* pAtomic, void* pData)
      {
         RWS_FUNCTION("RWS::SkinHelper::ForAllSkinsCallback");

         RWS_PRE(pAtomic);
         RWS_PRE(pData);

         RpSkin* pSkin = RpSkinAtomicGetSkin(pAtomic);
         if(pSkin)
         {
            SkinCallbackData& cbData = *static_cast<SkinCallbackData*>(pData);
            if(0 == cbData.m_callback(pSkin,cbData.m_pData)) pAtomic = 0;
         }

         RWS_RETURN(pAtomic);
      }

      /**
      *
      *  \ingroup SkinHelper
      *
      *  Applies the given callback function to all the skins in the clump
      *
      *  \param clump Reference to a RpClump object.
      *  \param callback \see RpSkinCallBack.
      *  \param pData void pointer to custom data, passed to the callback function.
      *
      */
      void ForAllSkins( RpClump& clump, RpSkinCallBack callback, void* pData )
      {
         RWS_FUNCTION("RWS::SkinHelper::ForAllSkins");

         SkinCallbackData cbData(callback,pData);
         RpClumpForAllAtomics(&clump,ForAllSkinsCallback,&cbData);

         RWS_RETURNVOID();
      }

      RpSkin* FindFirstSkinCallback(RpSkin* pSkin, void* pData);

      /**
      *
      *  \ingroup SkinHelper
      *
      *  Finds the first skin in the clump. Returns NULL if no such skin is found.
      *
      *  \param clump Reference to a RpClump object.
      *
      *  \returns Returns pointer to RpSkin object, otherwise 0.
      */
      RpSkin* FindFirstSkin(RpClump& clump)
      {
         RWS_FUNCTION("RWS::SkinHelper::FindFirstSkin");

         RpSkin* pSkin = 0;
         ForAllSkins(clump,FindFirstSkinCallback,&pSkin);

         RWS_RETURN (pSkin);
      }

      /**
      *
      *  \ingroup SkinHelper
      *  
      *  Stops when the first skin is found
      *
      */
      RpSkin* FindFirstSkinCallback(RpSkin* pSkin, void* pData)
      {
         RWS_FUNCTION("RWS_::SkinHelper::FindFirstSkinCallback")

         RWS_PRE(pSkin);
         RWS_PRE(pData);

         *static_cast<RpSkin**>(pData) = pSkin;

         RWS_RETURN(0);
      }
   }
}