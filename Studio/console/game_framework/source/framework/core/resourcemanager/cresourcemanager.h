/*****************************************************************************
*
* File :     CResourceManager.h
*
* Abstract : The CResourceManager and associated classes provide a way of managing
* the data resources for a game level. Typically this will be graphics, audio and animation
* data but could be just about anything.
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
* Copyright (c) 2002 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#ifndef _CRESOURCEMANAGER_H_
#define _CRESOURCEMANAGER_H_

#include "CResource.h"

namespace RWS
{
   /**
   *
   *  \ingroup Resource
   *
   *  \page ResourceOverview Overview
   *
   *  The CResourceManager and associated classes provide a way of managing
   *  the data resources for a game level. Typically this will be graphics,
   *  audio and animation data but could be just about anything.
   *
   */

  /**
   *
   *  \ingroup Resource
   *
   *  The ResourceManager class provides functionality for managing assets. An asset
   *  is a block of memory typically graphics, audio, or animation data, which may be
   *  accessed by many clients.
   *
   */
   class CResourceManager
   {
   public:
      
      static void Add(  const RwChar *_pResName,
                        const RWSGUID *_pResId,
                        const RwChar *_pResType,
                        const void *_pResData,
                        RwUInt32 _ResDataSize);
      
      static const void *Remove(const void *_pResData);
      
      static const void *FindById(  const RWSGUID *_pResId,
                                    RwChar const **_pResType = 0,
                                    RwUInt32 *_ResDataSize = 0,
                                    RwChar const **_pResName = 0);
      
      static const void *FindByName(const RwChar * const _pResName,
                                    RWSGUID *_pResId = 0,
                                    RwChar const **_pResType = 0,
                                    RwUInt32 *_ResDataSize = 0);
      
      static void Lock(const void *ptr);
      
      static void UnLock(const void *ptr);
      
      static void *Load(const RwChar *pStrName,
                        const RWSGUID *pResId,
                        const RwChar *pStrType,
                        const RwChar *pStrFName,
                        const RwChar *pStrLoc);
      
      static void *UpLoad( const RwChar *pStrName,
                           const RWSGUID *pResId,
                           const RwChar *pStrType,
                           const RwChar *pStrLoc,
                           RwStream *pStream,
                           RwUInt32 nEmbDataSize);

      static bool Purge(void);

      static bool PurgeAll(void);
      
#ifdef _DEBUG
      static void Debug_ListAvailableResources(void);
#endif

      static void Open(void);
      static void Close(void);

      static void RegisterStreamChunkHandlers(void);

      static void UnRegisterStreamChunkHandlers(void);

      static void Update(const RwChar *pStrType,
                         const void *pResData,
                         const void *pData);
   protected:

      static bool PurgeItem(CResourceIterator &Ittr);
   };
}//namespace RWS
#endif
