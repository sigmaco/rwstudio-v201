/*****************************************************************************
*
* File :     clinearmemorymanager.h
*
* Abstract : Linear memory management system for RenderWare Studio.
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

#ifndef _RWSPLACEMENTNEW_H_
#define _RWSPLACEMENTNEW_H_

#include <new>
#include <vector>

#include "..\macros\debugmacros.h"

namespace RWS
{

   class CLinearMemoryManager
   {
   private:
      
      /*
      *
      *  Represents a pool of memory allocated by the linear memory manager.
      *
      */
      struct LinearMemoryPool
      {
         RwUInt8 * m_pStorage;      // Start of the memory pool (unaligned).
         RwUInt8 * m_pStorageStart; // Start address of the pool (aligned).
         RwUInt8 * m_pStorageNext;  // Address of the next empty block in the pool.
         RwUInt8 * m_pStorageEnd;   // Address one after the end of pool.
         RwUInt32 m_uAllocCount;    // Number of blocks of memory that have been allocated from this pool.
      };

      /*
      *  Used to keep track of allocated memory pools.
      */
      typedef std::vector<LinearMemoryPool *> LinearMemoryPools;

   public:

      /**
      *
      *  Destruction routine called when executable shuts down.
      *
      */
      static void DestroyLinearMemoryManager(void)
      {
         LinearMemoryPools::iterator it = m_StoragePools.begin();
         while (it != m_StoragePools.end())
         {
            LinearMemoryPool * pPool = (*it);
            it = m_StoragePools.erase(it);
            DestroyMemoryPool(pPool);
         }
      }

      /**
      *  Returns the instance of the singleton.
      *
      *  \return Returns a pointer to the singleton instance of the LinearMemoryManager.
      */
      static CLinearMemoryManager * Instance()
      {
         static CLinearMemoryManager tCLinearMemoryManager;

         return &tCLinearMemoryManager;
      }

      /**
      *
      *  Sets the size of the LMM's memory pool.
      *
      *  \param memSize Size of memory pool to be created for the LMM.
      *
      *  \return Returns a pointer to the singleton instance of the LinearMemoryManager (LMM)
      */
      static CLinearMemoryManager * CreateMemoryPool(size_t MemSize)
      {
         LinearMemoryPool * pNewPool;

         pNewPool = ::RWS_NEW LinearMemoryPool;
         pNewPool->m_pStorage =  ::RWS_NEW RwUInt8[MemSize + CClassFactory::m_AlignmentSize];

         pNewPool->m_pStorageStart = pNewPool->m_pStorage;

         if ((reinterpret_cast<RwUInt32>(pNewPool->m_pStorageStart) % CClassFactory::m_AlignmentSize))
            pNewPool->m_pStorageStart += (CClassFactory::m_AlignmentSize - (reinterpret_cast<RwUInt32>(pNewPool->m_pStorageStart) % CClassFactory::m_AlignmentSize));
                     
         RWS_ASSERT(!((int)pNewPool->m_pStorageStart % CClassFactory::m_AlignmentSize),
                    "CLinearMemoryManager: Storage not aligned to CClassFactory::m_AlignmentSize.");

         pNewPool->m_pStorageNext = pNewPool->m_pStorageStart;
         pNewPool->m_pStorageEnd = pNewPool->m_pStorageStart + MemSize;
         pNewPool->m_uAllocCount = 0;   

         // Insert the new pool into the list of pools
         m_StoragePools.push_back(pNewPool);

         // Set the new memory pool as the currently active one.
         m_pCurrentPool = pNewPool;

         return Instance();
      }

      static void * RequestMemory(size_t size);

      static RwBool FreeMemory(void * pMem);

      static void RegisterStreamChunkHandlers(void);

   private:
      CLinearMemoryManager()
      {
         m_StoragePools.reserve(8);
      };

      /**
      *
      *  Frees up the memory used by a linear memory pool in the linear memory
      *  manager.
      *
      *  \param pPool Memory pool to destroy.
      *
      */
      static void DestroyMemoryPool(LinearMemoryPool * pPool)
      {
         RWS_ASSERT(pPool->m_uAllocCount == 0,
                    "Deleting a memory pool with " << pPool->m_uAllocCount
                    << " allocations not freed.");

         delete [] pPool->m_pStorage;
         delete pPool;
      }

      static LinearMemoryPools m_StoragePools;
      static LinearMemoryPool * m_pCurrentPool;
   };

}

#endif