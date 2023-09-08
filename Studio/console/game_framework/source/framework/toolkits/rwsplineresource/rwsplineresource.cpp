/*****************************************************************************
*
* File :     rwsplineresource.cpp
*
* Abstract : Resource Manager for RenderWare Splines.
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
#endif

//#define RWS_CALLSTACKENABLE
//#define RWS_TRACEENABLE

#include <rpspline.h>

#ifdef _MSC_VER
#pragma comment (lib, "rpspline.lib")
#endif

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/macros/debugmacros.h"
#include "rwsplineresource.h"

namespace RWS
{
   /**
   *
   * Load a RenderWare Spline file resource.
   *
   *  \param psName pointer to a string defining the resource name.
   *
   *  \param psType pointer to a string defining the resource type.
   *
   *  \param psResourcePath pointer to a string containing a path where files
   *  referenced by this file can be found.
   *
   *  \param pStream pointer to a RenderWare stream.
   *
   *  \param uiStreamSize size of the file associated with this resource.
   *
   *  \return If successful returns a pointer to the loaded resource,
   *  otherwise 0.
   *
   */
   void *CRwSplineResource::Load(const RwChar *psName,
                                 const RwChar *psType,
                                 const RwChar *psResourcePath,
                                 RwStream* pStream,
                                 RwUInt32 uiStreamSize,
                                 RwUInt32 &uiResourceSize)
   {
      RWS_FUNCTION("RWS::CRwSplineResource::Load");
      RWS_PRE(pStream);

      void * pBuffer = 0;

      if (uiStreamSize > 0)
      {
         if (psResourcePath)
         {
            RwImageSetPath(psResourcePath);
         }

         RwChunkHeaderInfo _RwChunkHeaderInfo;

         RwStreamReadChunkHeaderInfo(pStream, &_RwChunkHeaderInfo);

         switch (_RwChunkHeaderInfo.type)
         {
            case rwID_SPLINE:
               pBuffer = RpSplineStreamRead(pStream);
               break;
         }
      }
      else
      {
         pBuffer = ::RWS_NEW RwInt32;
         
         *reinterpret_cast<RwInt32 *>(pBuffer) = -1;

         RWS_WARNING(uiStreamSize > 0, psName << " : Spline asset has no data");
      }

      uiResourceSize = uiStreamSize;

      RWS_RETURN(pBuffer);
   }

   /**
   *
   * Unload RenderWare Spline file resource.
   *
   *  \param pStrType pointer to a string defining the resource type.
   *
   *  \param pResource pointer to the resource.
   *
   *  \return True if the resource was successfully unloaded otherwise false.
   *
   */
   RwBool CRwSplineResource::UnLoad(const RwChar *pStrType, void *pResource)
   {
      RWS_FUNCTION("RWS::CRwSplineResource::UnLoad");

      RwBool ret = FALSE;

      if (*reinterpret_cast<RwInt32 *>(pResource) == -1)
      {
         delete(reinterpret_cast<RwInt32 *>(pResource));
         ret = TRUE;
      }
      else
      {
         if (rwstrcmp(pStrType, RWSTRING("rwID_SPLINE")) == 0)
         {
            RpSplineDestroy(reinterpret_cast<RpSpline *>(pResource));
            ret = TRUE;
         }
      }

      RWS_RETURN(ret);
   }

   /**
   *
   *  Check if the resource is handled by this resource handler.
   *
   *  \param psType pointer to a string defining the resource type.
   *
   *  \return TRUE if the resource is of type "SPLINE" otherwise returns FALSE.
   *
   */
   RwBool CRwSplineResource::IsHandled(const RwChar *psType)
   {
      RWS_FUNCTION("RWS::CRwSplineResource::IsHandled");
      RWS_PRE(psType);

      if (!rwstrcmp(psType, RWSTRING("rwID_SPLINE")))
      {
         RWS_RETURN(TRUE);
      }

      RWS_RETURN(FALSE);
   }

   /*
   *
   * Create an instance of the RenderWare Slpine Resource handler,
   * this is required in order to register the handler.
   *
   */
   namespace { CRwSplineResource gCRwSplineResource; }
}
