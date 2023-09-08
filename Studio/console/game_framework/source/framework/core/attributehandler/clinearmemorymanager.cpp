/*****************************************************************************
*
* File :     PlacementNew.cpp
*
* Abstract : Placement New functions for RenderWare Studio.
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
* Copyright (c) 2003 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/
#include "precomp.h"

#include <rwcore.h>
#include "new.h"
#include "../attributehandler/cclassfactory.h"
#include "../streamhandler/streamhandler.h"
#include "../streamhandler/strfunc.h"
#include "../macros/debugmacros.h"

#include "clinearmemorymanager.h"


namespace RWS
{
   
   CLinearMemoryManager::LinearMemoryPools CLinearMemoryManager::m_StoragePools;
   CLinearMemoryManager::LinearMemoryPool * CLinearMemoryManager::m_pCurrentPool = 0;

  /**
   *
   *  Receive Placement new information from RenderWare Studio Workspace.
   *
   *  \param rChunkHeader A reference to a RenderWare Gfx RwChunkHeaderInfo object.
   *
   *  \param pStream A pointer to the RenderWare Gfx Stream object.
   */
   namespace
   {
   
      void SetupPlacementNewInfo (RwChunkHeaderInfo &rChunkHeader, RwStream *pStream)
      {
         RWS_FUNCTION("RWS::NULL::SetupPlacementNewInfo");

         // Create a temporary buffer, based on the length in the chunk header
         RwChar *buffer = static_cast<char*>(::RWS_OP_NEW(rChunkHeader.length) );
         RwChar *pbuffer = buffer; // Used to walk the string

         // read the stream into the buffer
         RwStreamRead(pStream, buffer, rChunkHeader.length);

         // Read through data (first element is number of entity types to expect)
         RwInt32 count = *(RwInt32 *)pbuffer;
         // Increment the buffer pointer         
         pbuffer += sizeof(RwInt32);

         // Keep a track of the amount of memory required by the linearMemoryManager
         size_t TotalStorage = 0;

         // Loop through all the entity types
         for (int i = 0; i < count; i++)
         {            
            RwChar *tString; // What is the maximum characters for a behaviour name?
            RwInt32 stringLength;
            RwInt32 entityCount;

            // Get the null terminated string of the behaviour
            tString = pbuffer;
            // Increment the buffer pointer by the length of the string plus a null character
            stringLength = rwstrlen(tString) + 1;
            pbuffer += stringLength;

            // Apply any padding required within the string, to align the next read on an int boundary. 
            // (Mainly for the PS2 platform, but other platforms need to take the padding into account.)
            stringLength = stringLength%4;
            if (stringLength)
               pbuffer += 4 - (stringLength);

            // Next is the number of this behaviour to create space for
            entityCount = *(RwInt32 *)pbuffer;
            // Increment the buffer pointer
            pbuffer += sizeof(RwInt32);

            // After getting this data, need to correlate this with something to get
            // the size of the entity.
            size_t size = RWS::CClassFactory::GetRegisteredClassSize(tString);

            // Multiply the size of the entity by the number of entities to be created
            // Add this to the total size of the storage for the placement new system
            TotalStorage += size * entityCount;
         }
         // Allocate space for these entities.
         CLinearMemoryManager::Instance()->CreateMemoryPool(TotalStorage);

         // Delete the temporary buffer
         operator delete (buffer);

         RWS_RETURNVOID();
      }
   }
   

  /**
   *
   *  Register the Placement new handler for the placement new information stream.
   *
   */
   void CLinearMemoryManager::RegisterStreamChunkHandlers(void)
   {
      RWS_FUNCTION("RWS::CLinearMemoryManager::RegisterStreamChunkHandlers");

      // Register the chunk handler
      CStreamHandler::RegisterChunkHandler(MAKECHUNKID (rwVENDORID_CRITERIONRM,
           strFunc_PlacementNew), SetupPlacementNewInfo);

      RWS_RETURNVOID();
   }

   /**
   *  RequestMemory from the Linear Memory Manager
   *
   *  \param size Size of memory chunk required.
   *
   *  \return Returns a pointer to the memory chunk requested
   */
   void * CLinearMemoryManager::RequestMemory(size_t size)
   {
      RWS_FUNCTION("RWS::CLinearMemoryManager::RequestMemory");
      
      if (!m_pCurrentPool)
      {
         RWS_TRACE("There is no current memory pool to allocate from, allocating from the heap instead.");
         RWS_RETURN(0);
      }

      void *ret = m_pCurrentPool->m_pStorageNext;
      
      if ((m_pCurrentPool->m_pStorageNext + size ) > m_pCurrentPool->m_pStorageEnd)   
      {
         RWS_TRACE(" Warning linear memory buffer exceeds expected size");

         RWS_RETURN(0);
      }

      RWS_ASSERT((size % CClassFactory::m_AlignmentSize) == 0, "Size should always be a multiple of CClassFactory::m_AlignmentSize to ensure alignment. check the scope of the new operation ::RWS_NEW vs RWS_NEW.");

      if ((size % CClassFactory::m_AlignmentSize) != 0)
      {
         RWS_RETURN(0);
      }
      
      m_pCurrentPool->m_pStorageNext += size;
      m_pCurrentPool->m_uAllocCount++;

      RWS_TRACE(" Address " << RWS_HEX(ret));
      
      RWS_RETURN(ret);
   }
   
   /*
   *
   *  Free a memory block from the linear memory allocator.
   *
   */
   RwBool CLinearMemoryManager::FreeMemory(void * pMem)
   {
      RWS_FUNCTION("CLinearMemoryManager::FreeMemory");

      LinearMemoryPools::iterator it = m_StoragePools.begin();
      while (it != m_StoragePools.end())
      {
         if (pMem >= (*it)->m_pStorageStart
             && pMem <= (*it)->m_pStorageEnd)
         {
            (*it)->m_uAllocCount--;

            if ((*it)->m_uAllocCount == 0)
            {
               LinearMemoryPool * pPool = (*it);
               it = m_StoragePools.erase(it);
               DestroyMemoryPool(pPool);

               if (pPool == m_pCurrentPool)
               {
                  m_pCurrentPool = 0;
               }
            }

            RWS_RETURN(TRUE);
         }
         ++it;
      }

      RWS_RETURN(FALSE);
   }

}

