/*****************************************************************************
*
* File :     resourcemanagerstrfunc.cpp
*
* Abstract : Implements chunk handlers provided by this module.
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
#include "cresourcemanager.h"
#include "../macros/debugmacros.h"
#include "../streamhandler/streamhandler.h"
#include "../streamhandler/strfunc.h"

namespace RWS
{
   namespace
   {
      /*
      *
      *  Delete a resource from the resource manager.
      *
      *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
      *
      *  \param pStream A pointer to the RenderWare Gfx Stream object.
      */
      void UnLoadAsset (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
      {
         RWS_FUNCTION("RWS::NULL::UnLoadAsset");
      
         RwChar *buffer = static_cast<char*>(::RWS_OP_NEW(rChunkHeader.length) );
      
         RwStreamRead(pStream, buffer, rChunkHeader.length);
      
         // Asset Id
         //
         RWSGUID * pResourceId = reinterpret_cast<RWSGUID *>(buffer);
      
         const RwChar *pResType;
         const void *pResData;
      
         if ((pResData = CResourceManager::FindById(pResourceId, &pResType)))
         {
            // This asset has already been loaded.
            // Remove the resource using the Resource data found.
         
            CResourceManager::Remove(const_cast<void*>(pResData));
         }
      
         operator delete (buffer);
      
         RWS_RETURNVOID();
      }
   
      /**
      *
      *  Load the specified resource into the resource manager.
      *
      *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
      *
      *  \param pStream A pointer to the RenderWare Gfx Stream object.
      */
      void LoadAsset (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
      {
         RWS_FUNCTION("RWS::NULL::LoadAsset");
      
         RwChar *buffer = static_cast<char*>(::RWS_OP_NEW(rChunkHeader.length) );
      
         RwStreamRead(pStream, buffer, rChunkHeader.length);
      
         // Asset Name
         //
         RwUInt32 * pPaddedSize = reinterpret_cast<RwUInt32 *>(buffer);
         RwChar * pStrName = reinterpret_cast<RwChar *>(pPaddedSize + 1);

         // Asset Id
         //
         RWSGUID * pResourceId = reinterpret_cast<RWSGUID *>(pStrName + (*pPaddedSize));

         // Asset Type
         //
         pPaddedSize = reinterpret_cast<RwUInt32 *>(pResourceId + 1);
         RwChar * pStrType = reinterpret_cast<RwChar *>(pPaddedSize + 1);
      
         // Asset File Name
         //
         pPaddedSize = reinterpret_cast<RwUInt32 *>(pStrType + (*pPaddedSize));
         RwChar * pStrFName = reinterpret_cast<RwChar *>(pPaddedSize + 1);
      
         // Asset Additional Files Location
         //
         pPaddedSize = reinterpret_cast<RwUInt32 *>(pStrFName + (*pPaddedSize));
         RwChar * pStrLoc = reinterpret_cast<RwChar *>(pPaddedSize + 1);
      
         // Global flag
         //
         RwBool * pbGlobal = reinterpret_cast<RwBool *>(pStrLoc + (*pPaddedSize));
         
         RWS_TRACE("[" << pStrName << "][" << pResourceId << "]["
            << pStrType << "][" << pStrFName << "][" << pStrLoc << "]");
      
         // Check if this resource has already been loaded 
         //
         if (CResourceManager::FindById(pResourceId, 0))
         {
            RWS_TRACE("WARNING:: "<< pResourceId <<" Resource Appears Multiple Times In Stream.");
         }
         else
         {
            void * pResource = CResourceManager::Load(pStrName,
                                                      pResourceId,
                                                      pStrType,
                                                      pStrFName,
                                                      pStrLoc);

            if ((*pbGlobal))
            {
               CResourceManager::Lock(pResource);
            }
         }
      
         operator delete (buffer);
      
         RWS_RETURNVOID();
      }
   
      /**
      *
      *  UpLoad the specified file to the resource manager.
      *
      *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
      *
      *  \param pStream A pointer to the RenderWare Gfx Stream object.
      */
      void LoadEmbeddedAsset (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
      {
         RWS_FUNCTION("RWS::NULL::LoadEmbeddedAsset");
      
         // read the size of the header
         RwUInt32 nEmbHeaderSize = 0;
         RwStreamRead (pStream, &nEmbHeaderSize, sizeof (RwUInt32));
      
         // allocate and read the header
         RwChar *buffer = static_cast<char*>(::RWS_OP_NEW(nEmbHeaderSize) );
         RwStreamRead (pStream, buffer, nEmbHeaderSize);
      
         // read the size of the embedded data
         RwUInt32 nEmbDataSize = 0;
         RwStreamRead (pStream, &nEmbDataSize, sizeof (RwUInt32));

         RWS_TRACE("nEmbDataSize: "<<nEmbDataSize);
 
         // Asset Name
         //
         RwUInt32 * pPaddedSize = reinterpret_cast<RwUInt32 *>(buffer);
         RwChar * pStrName = reinterpret_cast<RwChar *>(pPaddedSize + 1);

         // Asset Id
         //
         RWSGUID * pResourceId = reinterpret_cast<RWSGUID *>(pStrName + (*pPaddedSize));

         // Asset Type
         //
         pPaddedSize = reinterpret_cast<RwUInt32 *>(pResourceId + 1);
         RwChar * pStrType = reinterpret_cast<RwChar *>(pPaddedSize + 1);
      
         // Asset File Name
         //
         pPaddedSize = reinterpret_cast<RwUInt32 *>(pStrType + (*pPaddedSize));
         RwChar * pStrFName = reinterpret_cast<RwChar *>(pPaddedSize + 1);
      
         // Asset Additional Files Location
         //
         pPaddedSize = reinterpret_cast<RwUInt32 *>(pStrFName + (*pPaddedSize));
         RwChar * pStrLoc = reinterpret_cast<RwChar *>(pPaddedSize + 1);

         // Global flag
         //
         RwBool * pbGlobal = reinterpret_cast<RwBool *>(pStrLoc + (*pPaddedSize));
         
         RWS_TRACE("[" << pStrName << "][" << pResourceId << "]["
            << pStrType << "][" << pStrFName << "][" << pStrLoc << "]");

         RWS_WARNING(nEmbDataSize > 0, pStrName << " : Embedded resource size is 0 bytes. Id = " << pResourceId);
      
         // Check if this resource has already been loaded 
         //
         if (CResourceManager::FindById(pResourceId, 0))
         {
            RWS_TRACE("WARNING:: "<< pResourceId <<" Resource Appears Multiple Times In Stream.");
         
            // skip data
            RwStreamSkip (pStream, nEmbDataSize);
         }
         else
         {
            void * pResource = CResourceManager::UpLoad (pStrName,
                                                         pResourceId,
                                                         pStrType,
                                                         pStrLoc,
                                                         pStream,
                                                         nEmbDataSize);

            if ((*pbGlobal))
            {
               CResourceManager::Lock(pResource);
            }
         }
      
         // skip any extra padding
         int nLen = rChunkHeader.length - 
            nEmbDataSize - nEmbHeaderSize - 2 * sizeof (RwUInt32);
         if (nLen > 0)
            RwStreamSkip (pStream, nLen);
      
         operator delete (buffer);
      
         RWS_RETURNVOID();
      }


      /**
      *  
      *  Update the specified resource.
      *  
      *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
      *
      *  \param pStream A pointer to the RenderWare Gfx Stream object.
      * 
      */
      void UpdateAsset(RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
      {
         RWS_FUNCTION("RWS::NULL::UpdateAsset");

         RwChar *buffer = static_cast<char*>(::RWS_OP_NEW(rChunkHeader.length) );

         RwStreamRead(pStream, buffer, rChunkHeader.length);

         RwInt32 offset = 0;

         // Asset Id
         //
         RWSGUID *pResourceId = reinterpret_cast<RWSGUID *>(buffer);
         offset += sizeof(pResourceId) * 4;
         // Data
         //
         void *pData = reinterpret_cast<void *>(buffer + offset);

         const RwChar *pResType;
         const void *pResData;

         if ((pResData = CResourceManager::FindById(pResourceId, &pResType)))
         {
            // Found corresponding resource for the given resource ID
            CResourceManager::Update(const_cast<RwChar *>(pResType),
                                     const_cast<void *>(pResData),
                                     pData);

         }

         operator delete (buffer);

         RWS_RETURNVOID();
      }
   } 

   /**
   *
   *  Register the chunk handlers implemented by CResourceManager, \see strfunc_func \see CStreamHandler.#
   *
   *  CResourceManager implements the following chunk handlers, \ref strfunc_UnLoadAsset,
   *  \ref strfunc_LoadAsset, \ref strfunc_LoadEmbeddedAsset.
   *
   */
   void CResourceManager::RegisterStreamChunkHandlers(void)
   {
      CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
         strfunc_UnLoadAsset), UnLoadAsset);

      CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
         strfunc_LoadAsset), LoadAsset);

      CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
         strfunc_LoadEmbeddedAsset), LoadEmbeddedAsset);

      CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
         strfunc_UpdateAsset), UpdateAsset);
   }

   /**
   *
   *  Unregister the chunk handlers implemented by CResourceManager, \see strfunc_func \see CStreamHandler.
   *
   */
   void CResourceManager::UnRegisterStreamChunkHandlers(void)
   {
      CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_UnLoadAsset));
      CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_LoadAsset));
      CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_LoadEmbeddedAsset));
      CStreamHandler::UnRegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM, strfunc_UpdateAsset));
   }
}
