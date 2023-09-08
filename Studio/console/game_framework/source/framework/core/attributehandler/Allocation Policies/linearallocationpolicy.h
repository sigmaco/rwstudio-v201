/*****************************************************************************
*
* File :     linearallocationpolicy.h
*
* Abstract : Allows an entity to use the Linear memory management system for it's allocation.
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

#ifndef _RWSLINEARALLOCATIONPOLICY_H_
#define _RWSLINEARALLOCATIONPOLICY_H_

#include "../clinearmemorymanager.h"


namespace RWS
{

   class LinearAllocationPolicy
   {
      public:

      static RwBool UseLinearMemory() {return true;}

      // Needed for RWS_NEW macro expansion of new operator      

#ifdef RWS_DISABLE_MEMORY_CHECKING

      void * operator new(size_t size, const RwUInt32 Type)
      {
         if (size % CClassFactory::m_AlignmentSize)
         {
            size += CClassFactory::m_AlignmentSize - (size % CClassFactory::m_AlignmentSize );
         }

         void *ptr = CLinearMemoryManager::Instance()->RequestMemory(size);

#ifdef RWS_DESIGN

         if (!ptr)
         {
           ptr = MemHandlerNewHintEx(size, Type, rwMEMHINTDUR_GLOBAL);
         }
#else
         RWS_ASSERT(ptr != 0,
               "PlacementNewPolicy doesn't expect to perform dynamic initialization in none design mode.");
#endif

         return ptr;
         
      }

      void * operator new[](size_t size, const RwUInt32 Type)
      {
         if (size % CClassFactory::m_AlignmentSize)
         {
            size += CClassFactory::m_AlignmentSize - (size % CClassFactory::m_AlignmentSize );
         }

         void *ptr = CLinearMemoryManager::Instance()->RequestMemory(size);

#ifdef RWS_DESIGN

         if (!ptr)
         {
           ptr = MemHandlerNewHintEx(size, Type, rwMEMHINTDUR_GLOBAL);
         }
#else
         RWS_ASSERT(ptr != 0,
               "PlacementNewPolicy doesn't expect to perform dynamic initialization in none design mode.");
#endif

         return ptr;
         
      }

#else

      void * operator new(size_t size, const RwChar * File, const RwInt32 Line, const RwUInt32 Type)
      {
         if (size % CClassFactory::m_AlignmentSize)
         {
            size += CClassFactory::m_AlignmentSize - (size % CClassFactory::m_AlignmentSize );
         }

         void *ptr = CLinearMemoryManager::Instance()->RequestMemory(size);

#ifdef RWS_DESIGN

         if (!ptr)
         {
           ptr = MemHandlerNewHintEx(size, File, Line, Type, rwMEMHINTDUR_GLOBAL);
         }
#else
         RWS_ASSERT(ptr != 0,
               "PlacementNewPolicy doesn't expect to perform dynamic initialization in none design mode.");
#endif

         return ptr;
         
      }

      void * operator new[](size_t size, const RwChar * File, const RwInt32 Line, const RwUInt32 Type)
      {
         if (size % CClassFactory::m_AlignmentSize)
         {
            size += CClassFactory::m_AlignmentSize - (size % CClassFactory::m_AlignmentSize );
         }

         void *ptr = CLinearMemoryManager::Instance()->RequestMemory(size);

#ifdef RWS_DESIGN

         if (!ptr)
         {
           ptr = MemHandlerNewHintEx(size, File, Line, Type, rwMEMHINTDUR_GLOBAL);
         }
#else
         RWS_ASSERT(ptr != 0,
               "PlacementNewPolicy doesn't expect to perform dynamic initialization in none design mode.");
#endif

         return ptr;
         
      }

#endif

      void *operator new(size_t size)
      {
         // Check the passed in size will align the next allocation on the boundary alignment.
         if (size % CClassFactory::m_AlignmentSize)
         {
            size += CClassFactory::m_AlignmentSize - (size % CClassFactory::m_AlignmentSize );
         }

         void *ptr = CLinearMemoryManager::Instance()->RequestMemory(size);

#ifdef RWS_DESIGN

         if (!ptr)
         {
           ptr = MemHandlerNewHint(size, rwMEMHINTDUR_GLOBAL);
         }

#else
         RWS_ASSERT(ptr != 0,
               "PlacementNewPolicy doesn't expect to perform dynamic initialization in none design mode.");
#endif

         return ptr;
      }

      void *operator new[](size_t size)
      {
         void *ptr = CLinearMemoryManager::Instance()->RequestMemory(size);

#ifdef RWS_DESIGN

         if (!ptr)
         {
           ptr = MemHandlerNewHint(size, rwMEMHINTDUR_GLOBAL);
         }
#else
         RWS_ASSERT(ptr != 0,
               "PlacementNewPolicy doesn't expect to perform dynamic initialization in none design mode.");
#endif
         return ptr;
      }
   
#ifdef RWS_DISABLE_MEMORY_CHECKING

      void operator delete(void * pObj, const RwUInt32 Type)
      {
#ifdef RWS_DESIGN
         if (pObj)
         {
            if (!CLinearMemoryManager::Instance()->FreeMemory(pObj))
            {
               MemHandlerDel(pObj);
            }
         }
#else
         if (pObj)
         {
            CLinearMemoryManager::Instance()->FreeMemory(pObj);
         }
#endif
      };

      void operator delete[](void * pObj, const RwUInt32 Type)
      {
#ifdef RWS_DESIGN
         if (pObj)
         {
            if (!CLinearMemoryManager::Instance()->FreeMemory(pObj))
            {
               MemHandlerDel(pObj);
            }
         }
#else
         if (pObj)
         {
            CLinearMemoryManager::Instance()->FreeMemory(pObj);
         }
#endif
      };

#else

      void operator delete(void * pObj, const RwChar * File, const RwInt32 Line, const RwUInt32 Type)
      {
#ifdef RWS_DESIGN
         if (pObj)
         {
            if (!CLinearMemoryManager::Instance()->FreeMemory(pObj))
            {
               MemHandlerDel(pObj);
            }
         }
#else
         if (pObj)
         {
            CLinearMemoryManager::Instance()->FreeMemory(pObj);
         }
#endif
      };

      void operator delete[](void * pObj, const RwChar * File, const RwInt32 Line, const RwUInt32 Type)
      {
#ifdef RWS_DESIGN
         if (pObj)
         {
            if (!CLinearMemoryManager::Instance()->FreeMemory(pObj))
            {
               MemHandlerDel(pObj);
            }
         }
#else
         if (pObj)
         {
            CLinearMemoryManager::Instance()->FreeMemory(pObj);
         }
#endif
      };

#endif

      void operator delete(void *pObj)
      {
#ifdef RWS_DESIGN
         if (pObj)
         {
            if (!CLinearMemoryManager::Instance()->FreeMemory(pObj))
            {
               MemHandlerDel(pObj);
            }
         }
#else
         if (pObj)
         {
            CLinearMemoryManager::Instance()->FreeMemory(pObj);
         }
#endif
      };

      void operator delete[](void *pObj)
      {
#ifdef RWS_DESIGN
         if (pObj)
         {
            if (!CLinearMemoryManager::Instance()->FreeMemory(pObj))
            {
               MemHandlerDel(pObj);
            }
         }
#else
         if (pObj)
         {
            CLinearMemoryManager::Instance()->FreeMemory(pObj);
         }
#endif
      };
   };

}

#endif
