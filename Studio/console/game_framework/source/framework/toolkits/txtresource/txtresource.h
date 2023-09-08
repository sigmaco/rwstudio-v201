/*****************************************************************************
*
* File :     txtresource.h
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

#ifndef _TEXTRESOURCE_H_
#define _TEXTRESOURCE_H_

//////////////////////////////////////////////////////////////////
//
// Renderware Studio Includes
//
#include "framework/core/resourcemanager/cresourcehandler.h"

namespace RWS
{
   /**
   *
   * \ingroup TxtResource
   *
   *  Resource handler for text file resources.
   *
   */
   class CTextResource: public CResourceHandler
   {
      public:

         virtual void *Load(const RwChar *psName, const RwChar *psType, const RwChar *psResourcePath, RwStream* pStream, RwUInt32 uiStreamSize,RwUInt32 &uiResourceSize);

         virtual RwBool UnLoad(const RwChar *pStrType, void *pResource);

         virtual RwBool IsHandled(const RwChar *psType);

         virtual void Update(const void *pResData,
            const void *pData,
            const RwChar *pStrType) {}
   };
}

#endif