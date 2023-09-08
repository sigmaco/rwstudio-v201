/****************************************************************************
*
* File :     SeqCtrlDataResource.h
*
* Abstract : Handles loading of sequence control data resources.
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

#ifndef SEQUENCE_DATA_RES_HANDLER
#define SEQUENCE_DATA_RES_HANDLER

//////////////////////////////////////////////////////////////////
//
// Renderware Studio Includes
//

#include "framework/core/resourcemanager/cresourcehandler.h"
#include "framework/core/macros/debugmacros.h"
#include "cseqctrldata.h"

using namespace RWS::SEQUENCE;
namespace RWS
{
   /**
   *
   * Defines the resource string used for sequence control data resources.
   *
   */

   const RwChar SEQUENCE_DATA_RESOURCE_STRING[] = RWSTRING("rwID_SEQUENCE");

   /**
   *
   *  \ingroup RwGfxResource
   *
   *  Resource handler for handling Renderware resources.
   *
   */
   class CSeqCtrlDataResource : public CResourceHandler
   {
   public:

      CSeqCtrlDataResource(void);
      virtual ~CSeqCtrlDataResource(void);

      virtual void Open(void);
      virtual void Close(void);
      virtual RwBool IsHandled(const RwChar *psType);
      virtual void *Load(const RwChar *psName, const RwChar *psType,
         const RwChar *psResourcePath, RwStream* pStream, RwUInt32 uiStreamSize,
         RwUInt32 &uiResourceSize);
      virtual RwBool UnLoad(const RwChar *pStrType, void *pResource);

      virtual void Update(const void *pResData,
                          const void *pData,
                          const RwChar *pStrType);

      static CSeqCtrlData *Find(const RwChar *psName);

   private:

      static CSeqCtrlDataNode *sm_pDataRoot;

      static void Empty(void);
   };
}        // namespace RWS
#endif   // #ifndef SEQUENCE_DATA_RES_HANDLER
