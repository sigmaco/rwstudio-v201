/*****************************************************************************
*
* File :     txtresource.cpp
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

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#include "framework/core/macros/debugmacros.h"
#include "txtresource.h"

namespace RWS
{
   /**
   *
   * Load a text file resource.
   *
   *  \param psName pointer to a string defining the resource name.
   *
   *  \param psType pointer to a string defining the resource type.
   *
   *  \param psResourcePath pointer to a string containing a path where files referenced
   *  by this file can be found.
   *
   *  \param pStream pointer to a RenderWare stream.
   *
   *  \param uiStreamSize size of the file associated with this resource.
   *
   *  \returns If successful returns a pointer to the loaded resource, otherwise 0.
   *
   */
   void *CTextResource::Load(const RwChar *psName, const RwChar *psType, const RwChar *psResourcePath, RwStream* pStream, RwUInt32 uiStreamSize,RwUInt32 &uiResourceSize)
   {
      RWS_FUNCTION("RWS::CTextResource::Load");
      RWS_PRE(pStream);

      void * pBuffer = 0;

      if (uiStreamSize > 0)
      {
         pBuffer = ::RWS_OP_NEW(uiStreamSize);
         RwStreamRead(pStream, pBuffer, uiStreamSize);
      }

      uiResourceSize = uiStreamSize;

      RWS_RETURN(pBuffer);
   }

   /**
   *
   * Unload text file resource.
   *
   *  \param pStrType pointer to a string defining the resource type.
   *
   *  \param pResource pointer to the resource.
   *
   *  \returns True if the resource was successfully unloaded otherwise false.
   *
   */
   RwBool CTextResource::UnLoad(const RwChar *pStrType, void *pResource)
   {
      RWS_FUNCTION("RWS::CTextResource::UnLoad");

      operator delete(pResource);

      RWS_RETURN(TRUE);
   }

   /**
   *
   *  Check if the resource is handled by this resource handler.
   *
   *  \param psType pointer to a string defining the resource type.
   *
   *  \returns TRUE if the resource is of type "TEXT" otherwise returns FALSE.
   *
   */
   RwBool CTextResource::IsHandled(const RwChar *psType)
   {
      RWS_FUNCTION("RWS::CTextResource::IsHandled");
      RWS_PRE(psType);

      if (!rwstrcmp(psType, RWSTRING("TEXT")))
      {
         RWS_RETURN(TRUE);
      }

      RWS_RETURN(FALSE);
   }

   /*
   *
   * Create an instance of the Text Resource handler, this is required in order to register the handler.
   *
   */
   namespace { CTextResource gCTextResource; }
}
