/*****************************************************************************
*
* File :     MemoryHandler.h
*
* Abstract : Memory handling functions for RenderWare Studio.
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
* Copyright (c) 2001 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

#ifndef _RWSMEMORY_H_
#define _RWSMEMORY_H_

/**
*
* \ingroup MemHandler
* \page overview Overview
*
* The memory system within RenderWare Studio combines the standard memory handler (global new), RenderWare's memory
* handler and its own memory handler into one location. All allocations are fed through the CMemHandler class,
* which can be made to log the allocation / deallocation and give usage and leak information. This information
* is most complete if the RWS_NEW and RWS_OP_NEW macros are used. When the memory handler is disabled, all
* allocations will still occur, but logging and additional memory usage (by the handler) are disabled.
*
* Other functionality of the memory handler class can enabled / disabled. The class provides memory leak checking,
* memory boundary over/under run checking, basic/pre-file/pre-line memory usage information. All can be enabled or
* disabled.
*
* The memory class in turn feeds it's requests into a series of BaseMem... functions. These are called for
* all handled memory allocations. The BaseMem... functions can be user replaced. The base memory functions are
* also passed in a type. There are 4 basic types, RenderWare, standard (global operator new), RenderWare Studio
* and user. The value is simply a numerical value, so additional user types can be added. With the standard code
* the values are not used, but they are provided to allow multiple heap memory systems to be implemented.
*
* Note that some platforms may use the standard library allocation functions (malloc, free, realloc and calloc)
* within their libraries. These cannot currently be allocated / tracked by this system.
*
* Also, this system will only be implemented for RenderWare allocations if the RenderWare Studio startup 'Open'
* function is called with it's memory handler parameter set to zero (NULL). It is necessary to have the
* 'RWSMemory.h' file included to use the functionality - this is included in the precompiled header file of all
* platforms and so should not require direct inclusion.
*
* The memory system will be disabled if the 'RWS_DISABLE_MEMORY_CHECKING' definition is made on the compiler command line. By
* default, this is defined for the release and cd-rom project builds.
*
* There is now functionality to track the user types passed into RWS_NEW_TYPE and RWS_OP_NEW_TYPE. The
* RWS_MEMORY_USER_TYPE_METRICS and RWS_MEMORY_USER_PEAK_METRICS will report the user type on file/line metrics and
* memory usage per user type.
*
* Memory can be cleared to a predefined value when it is allocated and to a different value just before being
* freed, to aid with checking if pointers use valid memory. The RWS_MEMORY_INIT_VALUE is used to switch this on
* and off.
*
* Memory can also be check to see if the guard boundaries have been damaged before the memory is freed using the
* CheckAllocatedBounds function. The older CheckAndReport function, which reports errors which occurred during the
* free operations, can still be used.
*
* \see RWS::Startup::Open
* \see CMemHandler
* \see CMemHandler::CheckAndReport
* \see CMemHandler::CheckAllocatedBounds
* \see RWS_NEW
* \see RWS_NEW_TYPE
* \see RWS_OP_NEW
* \see RWS_OP_NEW_TYPE
* \see BaseMemInit
* \see BaseMemClose
* \see BaseMemAlloc
* \see BaseMemFree
* \see BaseMemReAlloc
* \see BaseMemCalloc
* \see RWS_MEMORY_LOG          
* \see RWS_MEMORY_INC_MEM_SYS  
* \see RWS_MEMORY_LEAK_CHECK   
* \see RWS_MEMORY_REPORT_STD   
* \see RWS_MEMORY_BOUNDS_CHECK 
* \see RWS_MEMORY_BASIC_METRICS
* \see RWS_MEMORY_ASSERT_ON_ERROR
* \see RWS_MEMORY_FILE_METRICS 
* \see RWS_MEMORY_LINE_METRICS 
* \see RWS_MEMORY_TYPE_RW
* \see RWS_MEMORY_TYPE_STD
* \see RWS_MEMORY_TYPE_RWS
* \see RWS_MEMORY_TYPE_USER
* \see RWS_MEMORY_USER_TYPE_METRICS
* \see RWS_MEMORY_USER_PEAK_METRICS
* \see RWS_MEMORY_INIT_VALUE
*
*/

/* Replacement for (and call via) RwMalloc, RwFree, RwCalloc and RwRealloc. Documented in .cpp file... */

void *RWSMalloc(size_t size, RwUInt32 hint);
void RWSFree(void *pMem);
void *RWSReAlloc(void *pMem, size_t newSize, RwUInt32 hint);
void *RWSCalloc(size_t num, size_t size, RwUInt32 hint);

/********************************************
 *                                          *
 * If 'C' file, then turn off C++ features. *
 *                                          *
 ********************************************/

#if defined(__cplusplus) || defined(DOXYGEN)

/**
*
* \ingroup MemHandler
*
* Memory type used only when the memory type was not stored, for example if file only metrics are used.
*
*/

const RwUInt32 RWS_MEMORY_TYPE_UNKNOWN = 0xFFFFFFFF;

/**
*
* \ingroup MemHandler
*
* Memory type passed to the base memory functions for RenderWare allocations, i.e. all those via the
* RwMalloc, RwCalloc and RwRealloc functions.
*
* \see CMemHandler::GetUserTypeName
*
*/

const RwUInt32 RWS_MEMORY_TYPE_RW = 0;

/**
*
* \ingroup MemHandler
*
* Memory type passed to the base memory functions for standard memory allocations, i.e. all those via
* the global operator new.
*
* \see CMemHandler::GetUserTypeName
*
*/

const RwUInt32 RWS_MEMORY_TYPE_STD = 1;

/**
*
* \ingroup MemHandler
*
* Memory type passed to the base memory functions for standard memory allocations, i.e. all those via
* the global operator new. This is the value used by all 'RWS_NEW' and 'RWS_OP_NEW' macros.
*
* \see RWS_NEW
* \see RWS_OP_NEW
* \see CMemHandler::GetUserTypeName
*
*/

const RwUInt32 RWS_MEMORY_TYPE_RWS = 2;

/**
*
* \ingroup MemHandler
*
* Memory type value used as a base for user values. This should always be used, as the value is not
* guaranteed not to change. Values less than this are 'standard' types (RenderWare, global new,
* RenderWare Studio, etc) anything equal to or above is a user type. The value can be passed to the
* base memory functions via use of the 'RWS_NEW_TYPE' and 'RWS_OP_NEW_TYPE' macros. These user types
* can be named via an overridden version of the BaseMemGetName function.
*
* \see BaseMemAlloc
* \see RWS_NEW_TYPE
* \see RWS_OP_NEW_TYPE
* \see BaseMemGetName
* \see CMemHandler::GetUserTypeName
*
*/

const RwUInt32 RWS_MEMORY_TYPE_USER = 3;

// Specialized memory functions, needed for verbose information passed by the RWS_NEW, RWS_OP_NEW,
// RWS_NEW_TYPE and RWS_OP_NEW_TYPE macros.

#ifdef RWS_DISABLE_MEMORY_CHECKING
   void *operator new(size_t size, RwUInt32 type);
   void *operator new[](size_t size, RwUInt32 type);
   void operator delete(void *pMem, RwUInt32 type);
   void operator delete[](void *pMem, RwUInt32 type);
#else
   void *operator new(size_t size, const char *file, int line, RwUInt32 type);
   void *operator new[](size_t size, const char *file, int line, RwUInt32 type);
   void operator delete(void *pMem, const char *file, int line, RwUInt32 type);
   void operator delete[](void *pMem, const char *file, int line, RwUInt32 type);
#endif

// Standard memory functions, needed for VC libraries, etc.

void *operator new(size_t size);
void *operator new[](size_t size);
void operator delete(void *pMem);
void operator delete[](void *pMem);

/**
*
* \ingroup MemHandler
*
* Macro to replace 'operator new'. Use of it will increase the amount of information held by the memory handler.
* That is file and line numbers, etc, will be included.
*
* For example the code...
*
* \code
 
  RwUInt32 *pData;
  pData = static_cast<RwUInt32 *>(RwMalloc(sizeof(RwUInt32) * 500));

  ...

  RwFree(pData);
 
  \endcode
*
* ...would be converted to...
*
* \code
 
  RwUInt32 *pData;
  pData = static_cast<RwUInt32 *>(RWS_OP_NEW(sizeof(RwUInt32) * 500));

  ...

  operator delete(pData);
 
  \endcode
*
* When deallocating, call the standard C++ "operator delete" function. A direct call is needed as a destructor
* should not be called on raw memory. If you do wish to call the destructor then use the normal C++ "delete"
* function instead.
*
* \see RWS_NEW
* \see RWS_OP_NEW_TYPE
*
*/

// NOTE: Keep LONG names for size & type so help in compiler is more useful.

#ifdef RWS_DISABLE_MEMORY_CHECKING
#define RWS_OP_NEW(size) operator new(size, RWS_MEMORY_TYPE_RWS)
#else
#define RWS_OP_NEW(size) operator new(size, __FILE__, __LINE__, RWS_MEMORY_TYPE_RWS)
#endif

/**
*
* \ingroup MemHandler
*
* Macro to replace 'new'. Use of it will increase the amount of information held by the memory handler.
* That is file and line numbers, etc, will be included.
*
* For example the code...
*
* \code
 
  RwUInt32 *pData;
  pData = new RwUInt32[1000];
 
  \endcode
*
* ...would be converted to...
*
* \code
 
  RwUInt32 *pData;
  pData = RWS_NEW RwUInt32[1000];
 
  \endcode
*
* When deallocating, call the standard C++ delete or delete[] functions.
*
* \see RWS_OP_NEW
* \see RWS_NEW_TYPE
*
*/

#ifdef RWS_DISABLE_MEMORY_CHECKING
#define RWS_NEW new(RWS_MEMORY_TYPE_RWS)
#else
#define RWS_NEW new(__FILE__, __LINE__, RWS_MEMORY_TYPE_RWS)
#endif

/**
*
* \ingroup MemHandler
*
* Macro to replace 'operator new'. Use of it will increase the amount of information held by the memory handler.
* That is file and line numbers, etc, will be included. This version also allows a type ident to be specified,
* Either a user value or one of the preset values (RWS_MEMORY_TYPE_...).
*
* For example the code...
*
* \code
 
  RwUInt32 *pData;
  pData = static_cast<RwUInt32 *>(RwMalloc(sizeof(RwUInt32) * 500));

  ...

  RwFree(pData);
 
  \endcode
*
* ...would be converted to...
*
* \code
 
  const RwUInt32 MY_MEM_TYPE_GFX = RWS_MEMORY_TYPE_USER + 10;

  RwUInt32 *pData;
  pData = static_cast<RwUInt32 *>(RWS_OP_NEW_TYPE(sizeof(RwUInt32) * 500, MY_MEM_TYPE_GFX));

  ...

  operator delete(pData);
 
  \endcode
*
* When deallocating, call the standard C++ "operator delete" function. A direct call is needed as a destructor
* should not be called on raw memory. If you do wish to call the destructor then use the normal C++ "delete"
* function instead.
*
* \see RWS_NEW_TYPE
* \see RWS_OP_NEW
* \see RWS_MEMORY_TYPE_RW
* \see RWS_MEMORY_TYPE_STD
* \see RWS_MEMORY_TYPE_RWS
* \see RWS_MEMORY_TYPE_USER
*
*/

// NOTE: Keep LONG names for size & type so help in compiler is more useful.

#ifdef RWS_DISABLE_MEMORY_CHECKING
#define RWS_OP_NEW_TYPE(size, type) operator new(size, type)
#else
#define RWS_OP_NEW_TYPE(size, type) operator new(size, __FILE__, __LINE__, type)
#endif

/**
*
* \ingroup MemHandler
*
* Macro to replace 'new'. Use of it will increase the amount of information held by the memory handler.
* That is file and line numbers, etc, will be included. This version also allows a type ident to be
* specified. Either a user value or one of the preset values (RWS_MEMORY_TYPE_...).
*
* For example the code...
*
* \code
 
  RwUInt32 *pData;
  pData = new RwUInt32[1000];
 
  \endcode
*
* ...would be converted to...
*
* \code
 
  const RwUInt32 MY_MEM_TYPE_SFX = RWS_MEMORY_TYPE_USER + 15;

  RwUInt32 *pData;
  pData = RWS_NEW_TYPE(MY_MEM_TYPE_SFX) RwUInt32[1000];
 
  \endcode
*
* When deallocating, call the standard C++ delete or delete[] functions.
*
* \see RWS_NEW
* \see RWS_OP_NEW_TYPE
* \see RWS_MEMORY_TYPE_RW
* \see RWS_MEMORY_TYPE_STD
* \see RWS_MEMORY_TYPE_RWS
* \see RWS_MEMORY_TYPE_USER
*
*/

// NOTE: Keep LONG names for size & type so help in compiler is more useful.

#ifdef RWS_DISABLE_MEMORY_CHECKING
#define RWS_NEW_TYPE(type) new(type)
#else
#define RWS_NEW_TYPE(type) new(__FILE__, __LINE__, type)
#endif

// Access function which call function within the memory handler class. Needed because otherwise the control
// switch macros, etc, would have to be included in this file and changing them would result in a MAJOR
// RECOMPILE. This way the recompile can kept to just the memory handler .cpp file and a bit of linking.
// Documented in the .cpp file...

void MemHandlerStartBlock(const char *pName = 0);
RwUInt32 MemHandlerCheckBounds(RwUInt32 = 0);
RwUInt32 MemHandlerClearErrLog(RwUInt32 = 0);

#else

void MemHandlerStartBlock(const char *pName);
RwUInt32 MemHandlerCheckBounds(RwUInt32);
RwUInt32 MemHandlerClearErrLog(RwUInt32);

#endif      /* __cplusplus || DOXYGEN */

void MemHandlerEndBlock(void);
void MemHandlerCheckAndReport(void);

#ifdef RWS_DISABLE_MEMORY_CHECKING
void *MemHandlerNewEx(size_t size, RwUInt32 type);
void *MemHandlerNewHintEx(size_t size, RwUInt32 type, RwMemoryHintDuration hint);
#else
void *MemHandlerNewEx(size_t size, const char *file, int line, RwUInt32 type);
void *MemHandlerNewHintEx(size_t size, const char *file, int line, RwUInt32 type, RwMemoryHintDuration hint);
#endif

void *MemHandlerNew(size_t size);
void *MemHandlerNewHint(size_t size, RwMemoryHintDuration hint);
void MemHandlerDel(void *pMem);

#endif      /* _RWSMEMORY_H_ */
