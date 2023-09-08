/*****************************************************************************
*
* File :     dynamicallocationpolicy.h
*
* Abstract : Allows an entity to use the plain vanilla Dynamic memory management system for it's allocation.
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

#ifndef _RWSDYNAMICALLOCATIONPOLICY_H_
#define _RWSDYNAMICALLOCATIONPOLICY_H_


namespace RWS
{

   class DynamicAllocationPolicy
   {
      public:
      static RwBool UseLinearMemory() {return false;}

      // Needed for RWS_NEW macro expansion of new operator

#ifdef RWS_DISABLE_MEMORY_CHECKING

      void * operator new(size_t size, const RwUInt32 Type)
      {
         return MemHandlerNewEx(size, Type);
      }

      void * operator new[](size_t size, const RwUInt32 Type)
      {
         return MemHandlerNewEx(size, Type);
      }

#else

      void * operator new(size_t size, const RwChar * File, const RwInt32 Line, const RwUInt32 Type)
      {
         return MemHandlerNewEx(size, File, Line, Type);
      }

      void * operator new[](size_t size, const RwChar * File, const RwInt32 Line, const RwUInt32 Type)
      {
         return MemHandlerNewEx(size, File, Line, Type);
      }

#endif
      
      void *operator new(size_t size)
      {
         return MemHandlerNew(size);
      }
      
      void *operator new[](size_t size)
      {
         return MemHandlerNew(size);
      }

#ifdef RWS_DISABLE_MEMORY_CHECKING

      void operator delete(void * pObj, const RwUInt32 Type)
      {
         if (pObj)
         {
            MemHandlerDel(pObj);
         }
      };
         
      void operator delete[](void * pObj, const RwUInt32 Type)
      {
         if (pObj)
         {
            MemHandlerDel(pObj);
         }
      };

#else

      void operator delete(void * pObj, const RwChar * File, const RwInt32 Line, const RwUInt32 Type)
      {
         if (pObj)
         {
            MemHandlerDel(pObj);
         }
      };
         
      void operator delete[](void * pObj, const RwChar * File, const RwInt32 Line, const RwUInt32 Type)
      {
         if (pObj)
         {
            MemHandlerDel(pObj);
         }
      };

#endif

      void operator delete(void *pObj)
      {
         if (pObj)
         {
            MemHandlerDel(pObj);
         }
      };

      void operator delete[](void *pObj)
      {
         if (pObj)
         {
            MemHandlerDel(pObj);
         }
      };
   };
}

#endif
