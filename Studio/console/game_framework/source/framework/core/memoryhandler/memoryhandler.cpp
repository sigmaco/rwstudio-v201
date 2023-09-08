/*****************************************************************************
*
* File :     MemoryHandler.cpp
*
* Abstract : Memory handling and logging classes and functions.
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

#include "precomp.h"
#include <cstring>
#include <cstdlib>
#include <cctype>
#include "memoryprofile.h"

#ifdef DOLPHIN
// The GCN places the 'strncpy' etc into a 'std' namespace, which is
// non-standard! So, use the namespace to 'std::' does not have to be
// added - doing so would stop compilation on the other platforms.
using namespace std;
#include "framework\startup\gcn\dlmem.h"
#endif

// Having the call stack tracing enabled in this module causes a nasty recursion
//  with the ostream used to output debug messages. E.g. when operator new() gets called,
//  the overloaded version in here outputs some debug messages, which on the first
//  call constructs the ostream - which calls operator new() again, etc...
//
#ifdef RWS_CALLSTACKENABLE
#undef RWS_CALLSTACKENABLE
#endif

#ifdef RWS_TRACEENABLE
#undef RWS_TRACEENABLE
#endif


/**********************************************************************/
/**********************************************************************/
/*                                                                    */
/* PLEASE DON'T MODIFY THIS CODE UNLESS YOU KNOW WHAT YOU ARE DOING ! */
/*                                                                    */
/**********************************************************************/
/**********************************************************************/

#if DOXYGEN

/**
*
* \ingroup MemHandler
*
* Master switch for the memory manager, enables (1) or disables (0) the entire logging and leak checking
* system. Memory is still allocated in both states but, with the value at 0, no additional memory is used
* by the memory handler. See the 'CMemHandler' documentation for more information on memory usages.
*
* \see CMemHandler
* \see RWS_MEMORY_INC_MEM_SYS
* \see RWS_MEMORY_LEAK_CHECK   
* \see RWS_MEMORY_REPORT_STD   
* \see RWS_MEMORY_BOUNDS_CHECK 
* \see RWS_MEMORY_BASIC_METRICS
* \see RWS_MEMORY_FILE_METRICS 
* \see RWS_MEMORY_LINE_METRICS 
* \see RWS_MEMORY_ASSERT_ON_ERROR
* \see RWS_MEMORY_USER_TYPE_METRICS
* \see RWS_MEMORY_USER_PEAK_METRICS
* \see RWS_MEMORY_INIT_VALUE
*
*/

#define RWS_MEMORY_LOG                 1  // Version used for documentation only, MUST be kept at 1.
                                          // One which effects code is below.

/**
*
* \ingroup MemHandler
*
* Enables (1) or disables (0) if the memory used by the memory manager is included in the metric information
* returned by the class. Only active if RWS_MEMORY_LOG is.
*
* \see CMemHandler
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_LEAK_CHECK   
* \see RWS_MEMORY_REPORT_STD   
* \see RWS_MEMORY_BOUNDS_CHECK 
* \see RWS_MEMORY_BASIC_METRICS
* \see RWS_MEMORY_FILE_METRICS 
* \see RWS_MEMORY_LINE_METRICS 
* \see RWS_MEMORY_ASSERT_ON_ERROR
* \see RWS_MEMORY_USER_TYPE_METRICS
* \see RWS_MEMORY_USER_PEAK_METRICS
* \see RWS_MEMORY_INIT_VALUE
*
*/

#define RWS_MEMORY_INC_MEM_SYS         1  // Version used for documentation only, MUST be kept at 1.
                                          // One which effects code is below.

/**
*
* \ingroup MemHandler
*
* Enables (1) or disables (0) the memory leak checking is performed by the memory handler. Only active
* if RWS_MEMORY_LOG is.
*
* \see CMemHandler
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_INC_MEM_SYS
* \see RWS_MEMORY_REPORT_STD   
* \see RWS_MEMORY_BOUNDS_CHECK 
* \see RWS_MEMORY_BASIC_METRICS
* \see RWS_MEMORY_FILE_METRICS 
* \see RWS_MEMORY_LINE_METRICS 
* \see RWS_MEMORY_ASSERT_ON_ERROR
* \see RWS_MEMORY_USER_TYPE_METRICS
* \see RWS_MEMORY_USER_PEAK_METRICS
* \see RWS_MEMORY_INIT_VALUE
*
*/

#define RWS_MEMORY_LEAK_CHECK          1  // Version used for documentation only, MUST be kept at 1.
                                          // One which effects code is below.

/**
*
* \ingroup MemHandler
*
* Enables (1) or disables (0) the memory leak checking reports leaks allocate by the standard (global operator
* new) allocator. Only active if both RWS_MEMORY_LOG and RWS_MEMORY_LEAK_CHECK are enabled. This is useful on
* the PC as Window memory allocations are within this type.
*
* \see CMemHandler
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_INC_MEM_SYS
* \see RWS_MEMORY_LEAK_CHECK
* \see RWS_MEMORY_BOUNDS_CHECK 
* \see RWS_MEMORY_BASIC_METRICS
* \see RWS_MEMORY_FILE_METRICS 
* \see RWS_MEMORY_LINE_METRICS 
* \see RWS_MEMORY_ASSERT_ON_ERROR
* \see RWS_MEMORY_USER_TYPE_METRICS
* \see RWS_MEMORY_USER_PEAK_METRICS
* \see RWS_MEMORY_INIT_VALUE
*
*/

#define RWS_MEMORY_REPORT_STD          1  // Version used for documentation only, MUST be kept at 1.
                                          // One which effects code is below.

/**
*
* \ingroup MemHandler
*
* Enables (1) or disables (0) the memory boundary checking. This will check for under and overrun on the memory
* supplied to the game. Any problems will be logged ready for later reporting. Only active if both RWS_MEMORY_LOG
* and RWS_MEMORY_LEAK_CHECK are enabled. The latter is needed for the error logging.
*
* \see CMemHandler
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_INC_MEM_SYS
* \see RWS_MEMORY_LEAK_CHECK
* \see RWS_MEMORY_REPORT_STD
* \see RWS_MEMORY_BASIC_METRICS
* \see RWS_MEMORY_FILE_METRICS 
* \see RWS_MEMORY_LINE_METRICS 
* \see RWS_MEMORY_USER_TYPE_METRICS
* \see RWS_MEMORY_USER_PEAK_METRICS
* \see RWS_MEMORY_ASSERT_ON_ERROR
* \see RWS_MEMORY_INIT_VALUE
*
*/

#define RWS_MEMORY_BOUNDS_CHECK        1  // Version used for documentation only, MUST be kept at 1.
                                          // One which effects code is below.

/**
*
* \ingroup MemHandler
*
* Enables (1) or disables (0) the memory handler's basic metric logging. This stores the total amount of memory
* currently allocated, the peak amount and a rolling total / number of allocation calls. It also stores these
* values for each type of allocation (RenderWare, global new and RenderWare Studio's RWS_NEW, etc). Only active
* if RWS_MEMORY_LOG is enabled.
*
* \see CMemHandler
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_INC_MEM_SYS
* \see RWS_MEMORY_LEAK_CHECK
* \see RWS_MEMORY_REPORT_STD
* \see RWS_MEMORY_BOUNDS_CHECK
* \see RWS_MEMORY_FILE_METRICS 
* \see RWS_MEMORY_LINE_METRICS 
* \see RWS_MEMORY_ASSERT_ON_ERROR
* \see RWS_MEMORY_USER_TYPE_METRICS
* \see RWS_MEMORY_USER_PEAK_METRICS
* \see RWS_MEMORY_INIT_VALUE
*
*/

#define RWS_MEMORY_BASIC_METRICS       1  // Version used for documentation only, MUST be kept at 1.
                                          // One which effects code is below.

/**
*
* \ingroup MemHandler
*
* Enables (1) or disables (0) the memory handler's per file metrics. These store the total amount of memory
* currently allocated, the peak amount and a rolling total / number of allocation calls PER code or header file for
* each type (RenderWare, global new and RenderWare Studio's RWS_NEW, etc). Only active if RWS_MEMORY_LOG is enabled.
*
* \see CMemHandler
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_INC_MEM_SYS
* \see RWS_MEMORY_LEAK_CHECK
* \see RWS_MEMORY_REPORT_STD
* \see RWS_MEMORY_BOUNDS_CHECK
* \see RWS_MEMORY_BASIC_METRICS 
* \see RWS_MEMORY_LINE_METRICS 
* \see RWS_MEMORY_ASSERT_ON_ERROR
* \see RWS_MEMORY_USER_TYPE_METRICS
* \see RWS_MEMORY_USER_PEAK_METRICS
* \see RWS_MEMORY_INIT_VALUE
*
*/

#define RWS_MEMORY_FILE_METRICS        1  // Version used for documentation only, MUST be kept at 1.
                                          // One which effects code is below.

/**
*
* \ingroup MemHandler
*
* Enables (1) or disables (0) the memory handler's per line and file metrics. These store the total amount of memory
* currently allocated, the peak amount and a rolling total / number of allocation calls PER line PER code or header
* file. The type (RenderWare, global new and RenderWare Studio's RWS_NEW, etc) is also noted. Only active if
* RWS_MEMORY_LOG is enabled.
*
* \see CMemHandler
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_INC_MEM_SYS
* \see RWS_MEMORY_LEAK_CHECK
* \see RWS_MEMORY_REPORT_STD
* \see RWS_MEMORY_BOUNDS_CHECK
* \see RWS_MEMORY_BASIC_METRICS 
* \see RWS_MEMORY_FILE_METRICS 
* \see RWS_MEMORY_ASSERT_ON_ERROR
* \see RWS_MEMORY_USER_TYPE_METRICS
* \see RWS_MEMORY_USER_PEAK_METRICS
* \see RWS_MEMORY_INIT_VALUE
*
*/

#define RWS_MEMORY_LINE_METRICS     1     // Version used for documentation only, MUST be kept at 1.
                                          // One which effects code is below.
/**
*
* \ingroup MemHandler
*
* Enables (1) or disables (0) the memory handler's per user type metrics. These store the total amount of memory
* currently allocated and a rolling total / number of allocation calls PER user type PER line PER code or header
* file. Only active if RWS_MEMORY_LINE_METRICS is enabled. The type is the value passed in to RWS_NEW_TYPE or
* RWS_OP_NEW_TYPE and should be offset from the RWS_MEMORY_TYPE_USER value. There are also values reserved for
* the memory handler / RenderWare system.
*
* Also (separate to this) causes the logging of the user type within each memory allocation and subsequent
* reporting of this in the CheckAndReport function on memory leaks, boundary checks, etc.
*
* \see CMemHandler
* \see CMemHandler::CheckAndReport
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_INC_MEM_SYS
* \see RWS_MEMORY_LEAK_CHECK
* \see RWS_MEMORY_REPORT_STD
* \see RWS_MEMORY_BOUNDS_CHECK
* \see RWS_MEMORY_BASIC_METRICS 
* \see RWS_MEMORY_ASSERT_ON_ERROR
* \see RWS_MEMORY_LINE_METRICS
* \see RWS_MEMORY_USER_PEAK_METRICS
* \see RWS_MEMORY_INIT_VALUE
*
*/

#define RWS_MEMORY_USER_TYPE_METRICS     1   // Version used for documentation only, MUST be kept at 1.
                                             // One which effects code is below.
/**
*
* \ingroup MemHandler
*
* Enables (1) or disables (0) the memory handler's per user type metrics for peak values. These store the total
* amount of memory currently allocated, peak amount and a rolling total / number of allocation calls PER user 
* type. Only active if RWS_MEMORY_LOG is enabled. The type is the value passed in to RWS_NEW_TYPE or
* RWS_OP_NEW_TYPE and should be offset from the RWS_MEMORY_TYPE_USER value. There are also values reserved for
* the memory handler / RenderWare system.
*
* \see CMemHandler
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_INC_MEM_SYS
* \see RWS_MEMORY_LEAK_CHECK
* \see RWS_MEMORY_REPORT_STD
* \see RWS_MEMORY_BOUNDS_CHECK
* \see RWS_MEMORY_BASIC_METRICS 
* \see RWS_MEMORY_ASSERT_ON_ERROR
* \see RWS_MEMORY_LINE_METRICS
* \see RWS_MEMORY_USER_TYPE_METRICS
* \see RWS_MEMORY_INIT_VALUE
*
*/

#define RWS_MEMORY_USER_PEAK_METRICS     1   // Version used for documentation only, MUST be kept at 1.
                                             // One which effects code is below.
/**
*
* \ingroup MemHandler
*
* Enables (1) or disables (0) the memory handler's initialization of memory block's values. This will
* occur during an allocation (except one via a call via Calloc), when the allocated memory will be set
* to RWS_MEMORY_ALLOC_INIT - appears as 'INIT' in a memory display window. A second initialization will
* occur just before the memory is freed (all cases) when it will be set to RWS_MEMORY_FREE_INIT -
* appears as 'FREE' in a memory display window. There is a substantial overhead with this process and
* so it should only be turned on if absolutely necessary. Only works if RWS_MEMORY_LOG is also 1.
*
* There are some issue with using this functionality and the ReAlloc functions. The initialization of
* memory which is freed will fail in certain situations with this function.
*
* Also, some systems (ie the PC) my overwrite the RWS_MEMORY_FREE_INIT value with their own memory
* handling values.
*
* \see CMemHandler
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_INC_MEM_SYS
* \see RWS_MEMORY_LEAK_CHECK
* \see RWS_MEMORY_REPORT_STD
* \see RWS_MEMORY_BOUNDS_CHECK
* \see RWS_MEMORY_BASIC_METRICS 
* \see RWS_MEMORY_ASSERT_ON_ERROR
* \see RWS_MEMORY_LINE_METRICS
* \see RWS_MEMORY_USER_PEAK_METRICS
* \see RWS_MEMORY_USER_TYPE_METRICS
* \see CMemHandler::ReAlloc
*
*/

#define RWS_MEMORY_INIT_VALUE            1   // Version used for documentation only, MUST be kept at 1.
                                             // One which effects code is below.
/**
*
* \ingroup MemHandler
*
* Enables (1) or disables (0) the assert on error behavior of the memory handler. Only active if
* RWS_MEMORY_LOG and RWS_MEMORY_LEAK_CHECK / RWS_MEMORY_BOUNDS_CHECK are enabled.
*
* \see CMemHandler
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_LEAK_CHECK
* \see RWS_MEMORY_BOUNDS_CHECK
*
*/

#define RWS_MEMORY_ASSERT_ON_ERROR     1     // Version used for documentation only, MUST be kept at 1.
                                             // One which effects code is below.
#else

#ifdef RWS_DISABLE_MEMORY_CHECKING
#define RWS_MEMORY_LOG                 0     // DO NOT CHANGE, USED TO DISABLE SYSTEM ON RELEASE BUILD, ETC.
#else

//********************************************************************************
//
// VALUES USED FOR ACTUAL CODE - change these to change the states of CMemHandler.
//
//********************************************************************************

#define RWS_MEMORY_LOG                 1     // Used for build, change to change CMemHandler's states.

#endif

#define RWS_MEMORY_INC_MEM_SYS         0     // Used for build, change to change CMemHandler's states.
#define RWS_MEMORY_LEAK_CHECK          1     // Used for build, change to change CMemHandler's states.
#define RWS_MEMORY_REPORT_STD          0     // Used for build, change to change CMemHandler's states.
#define RWS_MEMORY_BOUNDS_CHECK        1     // Used for build, change to change CMemHandler's states.
#define RWS_MEMORY_BASIC_METRICS       1     // Used for build, change to change CMemHandler's states.
#define RWS_MEMORY_FILE_METRICS        1     // Used for build, change to change CMemHandler's states.
#define RWS_MEMORY_LINE_METRICS        1     // Used for build, change to change CMemHandler's states.
#define RWS_MEMORY_USER_TYPE_METRICS   1     // Used for build, change to change CMemHandler's states.
#define RWS_MEMORY_USER_PEAK_METRICS   1     // Used for build, change to change CMemHandler's states.
#define RWS_MEMORY_INIT_VALUE          0     // Used for build, change to change CMemHandler's states.
#define RWS_MEMORY_ASSERT_ON_ERROR     1     // Used for build, change to change CMemHandler's states.

#endif

/**
*
* \ingroup MemHandler
*
* A constant used to indicate that no line number exists. Used with versions of operator new and operator new[]
* which take file and line number information. This data is fed into the memory handler.
*
* \see CMemHandler
*
*/

const RwUInt32 MH_NO_LINE = 0xFFFFFFFF;

/**
*
* \ingroup MemHandler
*
* The number of dwords used for the bounds check guards on memory allocations. Only used if RWS_MEMORY_LOG and
* RWS_MEMORY_BOUNDS_CHECK are enabled. The minimum value is 1. The large this value the more memory will be
* use on each memory allocation. It should be kept at 1 unless it is necessary to increase it. Only reason for
* increase would be to track down over/underrun problems.
*
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_BOUNDS_CHECK
*
*/

const RwUInt32 RWS_MEMORY_NUM_BOUNDS         = 1;

/**
*
* \ingroup MemHandler
*
* The maximum number of memory allocation / error logs available for the memory handler. This is allocated as a
* static array if the leak checking is enabled. Each time a memory block is allocated, a log entry is used. When
* it is deleted, the log entry is cleared - UNLESS the bounds checking is enabled and a boundary violation is found.
* In this case the log is used to hold the boundary error information and is not cleared.
*
* \see CMemHandler
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_LEAK_CHECK
* \see RWS_MEMORY_BOUNDS_CHECK
*
*/

const RwUInt32 RWS_MEMORY_MAX_MEM_LOGS       = 13000;

/**
*
* \ingroup MemHandler
*
* The maximum number of file metric items. When an allocation is made, the lists is searched for an entry for the
* filename and memory type (RenderWare, global new, RenderWare Studio). If found, then it's updated, otherwise a
* new entry is made. These entries are only stored if RWS_MEMORY_FILE_METRICS is enabled and
* RWS_MEMORY_LINE_METRICS is disabled. The latter overrides the file storage, being file & line storage.
*
* \see CMemHandler
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_FILE_METRICS
* \see RWS_MEMORY_LINE_METRICS
* \see RWS_MEMORY_MAX_LINE_METRICS
*
*/

const RwUInt32 RWS_MEMORY_MAX_FILE_METRICS   = 500;

/**
*
* \ingroup MemHandler
*
* The maximum number of file / line metric items. When an allocation is made, the lists is searched for an entry
* for the filename and line number. The type is ignored as any given file/line can only allocate from one type
* (RenderWare, global new or RenderWare Studio) so it will always be the same type. If found, then it's updated,
* otherwise a new entry is made. These entries are only stored if RWS_MEMORY_LINE_METRICS is enabled.
*
* \see CMemHandler
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_FILE_METRICS
* \see RWS_MEMORY_LINE_METRICS
* \see RWS_MEMORY_MAX_FILE_METRICS
* \see RWS_MEMORY_MAX_USER_PEAK_METRICS
*
*/

const RwUInt32 RWS_MEMORY_MAX_LINE_METRICS   = 50000;

/**
*
* \ingroup MemHandler
*
* The maximum number of file / line metric items. When an allocation is made, the lists is searched for an entry
* for the filename and line number. The type is ignored as any given file/line can only allocate from one type
* (RenderWare, global new or RenderWare Studio) so it will always be the same type. If found, then it's updated,
* otherwise a new entry is made. These entries are only stored if RWS_MEMORY_LINE_METRICS is enabled.
*
* \see CMemHandler
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_LINE_METRICS
* \see RWS_MEMORY_MAX_LINE_METRICS
*
*/

const RwUInt32 RWS_MEMORY_MAX_USER_PEAK_METRICS   = 50;

/**
*
* \ingroup MemHandler
*
* This is the 32 bit value used for the guard block check at the beginning of the user memory allocation. It
* is only inserted if the RWS_MEMORY_BOUNDS_CHECK is enabled.
*
* \see CMemHandler
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_LEAK_CHECK
* \see RWS_MEMORY_BOUNDS_CHECK
* \see RWS_MEMORY_TAIL_GUARD
*
*/

#ifdef DOLPHIN
const RwUInt32 RWS_MEMORY_HEAD_GUARD = 0x48454144;       // "HEAD" in character format memory dump (Big endian).
#else
const RwUInt32 RWS_MEMORY_HEAD_GUARD = 0x44414548;       // "HEAD" in character format memory dump.
#endif

/**
*
* \ingroup MemHandler
*
* This is the 32 bit value used for the guard block check at the end of the user memory allocation. It
* is only inserted if the RWS_MEMORY_BOUNDS_CHECK is enabled.
*
* \see CMemHandler
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_LEAK_CHECK
* \see RWS_MEMORY_BOUNDS_CHECK
* \see RWS_MEMORY_HEAD_GUARD
*
*/
#ifdef DOLPHIN
const RwUInt32 RWS_MEMORY_TAIL_GUARD = 0x5441494C;       // "TAIL" in character format memory dump (Big endian).
#else
const RwUInt32 RWS_MEMORY_TAIL_GUARD = 0x4C494154;       // "TAIL" in character format memory dump.
#endif

/**
*
* \ingroup MemHandler
*
* The 32bit value that allocated memory is set to if RWS_MEMORY_INIT_VALUE is set to 1. This
* initialization does not occur with memory that is gained via the Calloc functionality, which
* is set to zero as the call requires.
*
* \see CMemHandler
* \see CMemHandler::Alloc
* \see RWS_MEMORY_INIT_VALUE
* \see RWS_MEMORY_FREE_INIT
*
*/

#ifdef DOLPHIN
const RwUInt32 RWS_MEMORY_ALLOC_INIT = 0x494E4954;       // "INIT" in character format memory dump (Big endian).
#else
const RwUInt32 RWS_MEMORY_ALLOC_INIT = 0x54494E49;       // "INIT" in character format memory dump.
#endif

/**
*
* \ingroup MemHandler
*
* The 32bit value that memory is set to if RWS_MEMORY_INIT_VALUE is set to 1 when it is freed.
* All frees (including them from a Calloc call) will result in this value being written into
* the memory just before it is handled back to the platform's memory system.
*
* \see CMemHandler
* \see CMemHandler::Alloc
* \see RWS_MEMORY_INIT_VALUE
* \see RWS_MEMORY_ALLOC_INIT
*
*/

#ifdef DOLPHIN
const RwUInt32 RWS_MEMORY_FREE_INIT = 0x46524545;       // "FREE" in character format memory dump (Big endian).
#else
const RwUInt32 RWS_MEMORY_FREE_INIT = 0x45455246;       // "FREE" in character format memory dump.
#endif

/**
*
* \ingroup MemHandler
*
* Indicates the number of bits to shift up the type data within the basic 32bit value stored in the memory allocation
* systems per allocation header. The remaining bits are used to store the blocks size. With a shift value of 28, this
* allows a maximum, in any one allocation, size of (2 to the power 28) - 1 = 268,435,455 bytes. While still allowing
* a nibble for the types (16 different ones). Currently only 3 are used, RenderWare, global new, RenderWare Studio.
*
* \see CMemHandler
* \see RWS_MEMORY_TYPE_MASK
*
*/

const RwUInt32 RWS_MEMORY_TYPE_SHIFT = 28;

/**
*
* \ingroup MemHandler
*
* A mask used to detect over sized blocks, check type bits, etc. All bits used for the type in the header's basic
* 32bit block, must be set to 1.
*
* \see RWS_MEMORY_TYPE_SHIFT
* \see CMemHandler
*
*/

const RwUInt32 RWS_MEMORY_TYPE_MASK = ~((1 << RWS_MEMORY_TYPE_SHIFT) - 1);

/**
*
* \ingroup MemHandler
*
* The maximum number of nested levels allowed by the memory handler. The 'zero' level is the global level, i.e. all
* memory allocations.
*
* \see CMemHandler
* \see CMemHandler::StartBlock
* \see CMemHandler::EndBlock
*
*/

const RwUInt8 RWS_MEMORY_NEST_LEVELS    = 100;

/**
*
* \ingroup MemHandler
*
* The maximum name length for any sub-level in the memory handler.
*
* \see CMemHandler
* \see CMemHandler::StartBlock
*
*/

const RwUInt32 RWS_MEMORY_NEST_NAME_LEN = 40;            // Includes the '\0' on the end of the string.

/**
*
* \ingroup MemHandler
*
* The buffer size used for data output by the memory handlers reporting (to debug console) functions.
*
* \see CMemHandler
* \see CMemHandler::CheckAndReport
*
*/

const RwUInt32 RWS_MEMORY_MAX_MSG_LEN = 8092;

/**
*
* \ingroup MemHandler
*
* The alignment necessary for each platform. PlayStation 2 and XBox require 16 bytes, PC and GameCube require 4.
* The memory handler's header (if being used) will be rounded up to the nearest alignment needed.
*
* \see CMemHandler
* \see CMemHandler::AddAlignment
* \see CMemHandler::RemoveAlignment
*
*/

#ifdef SKY
const RwUInt32 RWS_MEMORY_ALIGNMENT = 16;
#elif _XBOX
const RwUInt32 RWS_MEMORY_ALIGNMENT = 16;
#else
const RwUInt32 RWS_MEMORY_ALIGNMENT = 4;
#endif

// Do includes. These are added after the constants and macros, as some of the
// includes are based on the state of the macros / constants. In particular the
// tracing must be enabled before including the debug macro header file.

#include "memoryhandler.h"

#if RWS_MEMORY_LOG
#define RWS_TRACEENABLE
#endif

#include "../macros/debugmacros.h"

/**
*
* \ingroup MemHandler
*
* A constant used as a bit flag within the CMemHandler::MemLogItem error data field. This is used to indicate an
* underrun in the memory returned from the memory handler. That is the code to which the memory was returned has
* written to memory BEFORE the start of the block.
*
* \see CMemHandler
* \see CMemHandler::MemLogItem
*
*/

const RwUInt32 MH_MLI_UNDERRUN   = 0x00000001;

/**
*
* \ingroup MemHandler
*
* A constant used as a bit flag within the CMemHandler::MemLogItem error data field. This is used to indicate an
* overrun in the memory returned from the memory handler. That is the code to which the memory was returned has
* written to memory AFTER the end of the block.
*
* \see CMemHandler
* \see CMemHandler::MemLogItem
*
*/

const RwUInt32 MH_MLI_OVERRUN    = 0x00000002;


// Base memory allocation and free functions. To override these define 'NON_DEF_MEM_HANDLER' and write your own
// version in another file. They will then be linked in instead. These functions should not be called directly
// as they will by-pass the memory leak, bounds and metric code.
//
// NOTE: these function are at the head of the file as I cannot prototype them and have function in the SAME
//       file. If this is done, Doxygen will NOT add the functions to the documentation! Doxygen will only
//       support having a name in a file ONCE.

#ifndef NON_DEF_MEM_HANDLER

/**
*
* \ingroup MemHandler
*
* The base memory initializer. Standard version does nothing. Before the first memory allocation is made. Only
* called once. This can be replace (all other BaseMem.. function must also be - at the same time) by defining
* NON_DEF_MEM_HANDLER on the compiler command line, etc, and writing your own version. NOTE: This code would
* not have to be within this file.
*
* \see CMemHandler
* \see CMemHandler::CMemHandler
* \see BaseMemClose
* \see BaseMemAlloc
* \see BaseMemFree
* \see BaseMemReAlloc
* \see BaseMemCalloc
* \see BaseMemGetName
*
*/

void BaseMemInit(void)
{
   // Called on first memory allocation, via CMemHandler class.

#ifdef DOLPHIN

   RwBool Ret;

   Ret = RwsDolphinHeapInit();
   RWS_ASSERT(Ret, "FAILED to initialize memory system");

#endif

}

/**
*
* \ingroup MemHandler
*
* The base memory closer. Standard version does nothing. When the memory handler closes this function is called. Only
* called once. This can be replace (all other BaseMem.. function must also be - at the same time) by defining
* NON_DEF_MEM_HANDLER on the compiler command line, etc, and writing your own version. NOTE: This code would not have
* to be within this file.
*
* \see CMemHandler
* \see CMemHandler::~CMemHandler
* \see BaseMemInit
* \see BaseMemAlloc
* \see BaseMemFree
* \see BaseMemReAlloc
* \see BaseMemCalloc
* \see BaseMemGetName
*
*/

void BaseMemClose(void)
{
   // Call when memory handler closes.

#ifdef DOLPHIN

   RwBool Ret;

   Ret = RwsDolphinTermMemorySystem();
   RWS_ASSERT(Ret, "FAILED to close memory system");

#endif

}

/**
*
* \ingroup MemHandler
*
* The base memory allocator. Standard version calls malloc. This can be replace (all other BaseMem.. function must
* also be - at the same time) by defining NON_DEF_MEM_HANDLER on the compiler command line, etc, and writing your
* own version. NOTE: This code would not have to be within this file.
*
* \see CMemHandler
* \see CMemHandler::Alloc
* \see BaseMemInit
* \see BaseMemClose
* \see BaseMemFree
* \see BaseMemReAlloc
* \see BaseMemCalloc
* \see BaseMemGetName
*
*/

void *BaseMemAlloc(size_t size, RwUInt32 userTypeNum)
{
#ifdef DOLPHIN

   return (RwsDolphinHeapAlloc(size));
#else

   return (malloc(size));
#endif

}

/**
*
* \ingroup MemHandler
*
* The base memory deallocator. Standard version calls free. This can be replace (all other BaseMem.. function must
* also be - at the same time) by defining NON_DEF_MEM_HANDLER on the compiler command line, etc, and writing your
* own version. NOTE: This code would not have to be within this file.
*
* \see CMemHandler
* \see CMemHandler::Free
* \see BaseMemInit
* \see BaseMemClose
* \see BaseMemAlloc
* \see BaseMemReAlloc
* \see BaseMemCalloc
* \see BaseMemGetName
*
*/

void BaseMemFree(void *pMem)
{
#ifdef DOLPHIN

   RwsDolphinHeapFree(pMem);

#else

   free(pMem);

#endif
}

/**
*
* \ingroup MemHandler
*
* The base memory reallocator. Standard version calls realloc. This can be replace (all other BaseMem.. function must
* also be - at the same time) by defining NON_DEF_MEM_HANDLER on the compiler command line, etc, and writing your
* own version. NOTE: This code would not have to be within this file.
*
* \see CMemHandler
* \see CMemHandler::ReAlloc
* \see BaseMemInit
* \see BaseMemClose
* \see BaseMemAlloc
* \see BaseMemFree
* \see BaseMemCalloc
* \see BaseMemGetName
*
*/

void *BaseMemReAlloc(void *pMem, size_t size, RwUInt32 userTypeNum)
{
   // NOTE: If this function is replaced the replacement MUST FULLY IMPLEMENT realloc. That is...
   //  
   //       a) if size is zero and pMem is zero, don't crash, but do nothing.
   //       b) if size is zero and pMem is non-zero, free it.
   //       c) if size is non-zero and pMem is zero, allocate normally
   //       d) if size is non-zero and pMem is non-zero, then expand/contract/reallocate memory and copy as much
   //          as possible from old memory to new memory (smaller of old and new sizes).
   //
   // NOTE: This function IS called, so it cannot be not implemented or not fully implemented. In worst case,
   //       make the code call BaseMemFree and BaseMemAlloc as required. Function will be called by CMemHandler::
   //       ReAlloc to do all of the above 4 functions.

#ifdef DOLPHIN
   return (RwsDolphinHeapRealloc(pMem, size));

#else
   return(realloc(pMem, size));
#endif

}

/**
*
* \ingroup MemHandler
*
* The base memory clear allocator. Standard version calls calloc. This can be replace
* (all other BaseMem.. function must also be - at the same time) by defining 
* NON_DEF_MEM_HANDLER on the compiler command line, etc, and writing your
* own version. NOTE: This code would not have to be within this file.
*
* \see CMemHandler
* \see CMemHandler::Calloc
* \see BaseMemInit
* \see BaseMemClose
* \see BaseMemAlloc
* \see BaseMemFree
* \see BaseMemReAlloc
* \see BaseMemGetName
*
*/

void *BaseMemCalloc(size_t num, size_t size, RwUInt32 userTypeNum)              
{
   // NOTE: function IS used, so do implement. In worst case just call BaseMemAlloc and then a memset.
#ifdef DOLPHIN
   return(RwsDolphinHeapCalloc(num, size));
#else
   return(calloc(num, size));
#endif

}

#else

// Define macros for BaseMem... functions instead.

void BaseMemInit(void);
void BaseMemClose(void);
void *BaseMemAlloc(size_t size, RwUInt32 userTypeNum);
void BaseMemFree(void *pMem);
void *BaseMemReAlloc(void *pMem, size_t size, RwUInt32 userTypeNum);
void *BaseMemCalloc(size_t num, size_t size, RwUInt32 userTypeNum);

#endif

#if !defined(NON_DEF_MEM_HANDLER) && !defined(NON_DEF_MEM_USER_NAME_HANDLER)

/**
*
* \ingroup MemHandler
*
* The base memory user naming function. This function is called by the memory
* system when a user memory type number of RWS_MEMORY_TYPE_USER. Therefore, the
* passed in userTypeNumber will always be at least RWS_MEMORY_TYPE_USER or above.
* However, the value of RWS_MEMORY_TYPE_UNKNOWN will be handled externally. The
* function return a static string giving the types name or zero if the type is
* unknown. The default function returns 0 for all calls. This functionality can
* be overridden by writing a custom version of this function and causing the
* default version not to be compiled. To do this define
* NON_DEF_MEM_USER_NAME_HANDLER in the compiler defines section. This is separate
* from the define used to disable the other base memory functions. However, if
* they are overridden (be defining NON_DEF_MEM_HANDLER) then this function must
* also be overridden. NOTE: the code for these base memory functions does not have
* to be within this code file.
*
* \param userTypeNum The user type number requiring a name string. This will always
* be of RWS_MEMORY_TYPE_USER or above. However, RWS_MEMORY_TYPE_UNKNOWN will not
* be passed in.
* \return ZERO if the type has no name or is unknown or a pointer to a STATIC
* naming string otherwise. The caller should not attempt to free or alter this string.
* \see CMemHandler
* \see CMemHandler::Calloc
* \see BaseMemInit
* \see BaseMemClose
* \see BaseMemAlloc
* \see BaseMemFree
* \see BaseMemReAlloc
* \see BaseMemCalloc
*
*/

const char *BaseMemGetName(RwUInt32 userTypeNum)
{
   // Does not know about any user types, so return a value of zero. Overridden
   // code would test for userTypeNum's value and return a static string, ie...
   //
   // if (userTypeNum == (RWS_MEMORY_TYPE_USER + 10))
   // {
   //    return("GRAPHICS MEMORY");
   // }
   // else
   // {
   //    return(0);
   // }
   //
   // Use of a switch statement and constants for the (RWS_MEMORY_TYPE_USER + 10)
   // would make the code easier to read though, ie...
   //
   // switch (userTypeNum)
   // {
   //
   // ...
   //
   // case MY_GRAPHICS_MEMORY:
   //
   //    return("GRAPHICS MEMORY");
   //
   // ...
   //
   // default:
   //
   //    return(0);
   //
   // }

   return(0);
}

#else

// Define prototype for BaseMemGetName function instead.

const char *BaseMemGetName(RwUInt32 userTypeNum);

#endif

/**
*
* \ingroup MemHandler
*
* Class to handle the allocation and logging of memory allocation and de-allocation. Only logs if 'RWS_MEMORY_LOG'
* is enabled. Used by memory allocation functions, global operator new, RenderWare and RenderWare Studio's RWS_NEW,
* RWS_OP_NEW, etc macros. RenderWare is only included if RWS::Startup::Open has no other memory handling functions
* specified (i.e. it's memory function table pointer is zero). The system can be made to check for memory leaks,
* boundary violations and give metric information. Which data is given depends on the state of the RWS_MEMORY_...
* flags.
*
* As more features are included, the memory usage of the class will increase. The memory usage is in two forms.
* First, there can be a number of static arrays allocated...
*
* A) an array of CMemHandler::MemLogItem (RWS_MEMORY_MAX_MEM_LOGS in size) if RWS_MEMORY_LEAK_CHECK is enabled.
*
* B) an array of CMemHandler::FileMetricItem (RWS_MEMORY_MAX_FILE_METRICS in size) if RWS_MEMORY_FILE_METRIC is
* enabled, but RWS_MEMORY_LINE_METRIC is disabled.
*
* C) an array of CMemHandler::LineMetricItem (RWS_MEMORY_MAX_LINE_METRICS in size) if RWS_MEMORY_LINE_METRIC is
* enabled.
*
* Secondly, the system can place a header and trailer on each memory allocation...
*
* A) 4 bytes in header if RWS_MEMORY_LOG is enabled.
*
* B) 4 additional bytes in header if RWS_MEMORY_FILE_METRIC and/or RWS_MEMORY_LINE_METRIC is enabled.
*
* C) 4 additional bytes in header and 4 in a trailer if RWS_MEMORY_BOUNDS_CHECK is enabled.
*
* The header will be rounded up to the next alignment value. So, if the alignment was 16 bytes and RWS_MEMORY_LOG
* only was enabled, the header would go from 4 bytes and be aligned up to 16. If RWS_MEMORY_LOG,
* RWS_MEMORY_FILE_METRIC and RWS_MEMORY_BOUNDS_CHECK were all on, the header would go from 12 and be aligned up to
* 16 again (plus an additional 4 bytes would be placed on the trailer, but this is not aligned up and so would
* stay at 4 bytes in size).
*
* Some additional memory used, but it is very small (one of block of < 1Kb). Also, if RWS_MEMORY_LOG is disabled,
* then none of the above static, header or trailer memory usages will be used.
*
* For the memory system to work correctly, RWSMemory.h MUST be included in EVERY file, regardless of if it uses
* the RWS_NEW, RWS_OP_NEW, etc, macros. It is also necessary to ensure the object code from RWSMemory.cpp is
* included in the link (on some platforms it may have to be FIRST in the link). This is because the system
* replaces the base memory allocator. If a block of memory is allocated by this code, but deallocated by the
* compiler's base memory system (system alloc, free, global new), or visa-versa, then an error is likely to occur.
* If you think this is happening, you can check by disabling the memory systems extra memory usage
* (RWS_MEMORY_LOG disabled) to check.
*
* NOTE: that if you have any template classes in header file that perform memory allocation, they will also need
* to have the RWSMemory.h include. The simple way to perform this 'always include' is via a precompiled header.
*
* To make full use of the memory handler (i.e. to get line numbers and filenames) the RWS_NEW, RWS_OP_NEW,
* RWS_NEW_TYPE & RWS_OP_NEW_TYPE macros should be used. The _TYPE macros allow a user type number to be passed
* in (some allocations will force this to a predefined value). This is passed to the base memory allocators used
* by this class (BaseMemAlloc, BaseMemFree, etc) and, if you implement your own memory handler, can be used to 
* partition memory up to reduce/remove the effects of fragmentation. These user type numbers will also be
* reported on memory leaks, bounds violations and memory usage reports. The latter only if 
* RWS_MEMORY_USER_TYPE_METRICS and/or RWS_MEMORY_USER_PEAK_METRICS are used.
*
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_INC_MEM_SYS
* \see RWS_MEMORY_LEAK_CHECK   
* \see RWS_MEMORY_REPORT_STD   
* \see RWS_MEMORY_BOUNDS_CHECK 
* \see RWS_MEMORY_BASIC_METRICS
* \see RWS_MEMORY_FILE_METRICS 
* \see RWS_MEMORY_LINE_METRICS 
* \see RWS_MEMORY_USER_TYPE_METRICS 
* \see RWS_MEMORY_USER_PEAK_METRICS 
* \see RWS_NEW
* \see RWS_OP_NEW
* \see RWS_NEW_TYPE
* \see RWS_OP_NEW_TYPE
* \see RWS::Startup::Open
* \see BaseMemAlloc
* \see BaseMemFree
* \see BaseMemReAlloc
* \see BaseMemCalloc
*
*/

class CMemHandler
{
public:

   /**
   *
   * \ingroup MemHandler
   *
   * Internal typing value for CMemHandler. Used to track where the memory was allocated from. Currently there
   * can be only 16 of these as the system stores them in a nibble.
   *
   * \see RWS_MEMORY_TYPE_SHIFT
   *
   */

   enum mem_type
   {
      MHMT_STD = 0,           /**< Standard (global operator new) allocations */
      MHMT_RWS,               /**< RenderWare Studio / user allocations. Those that use RWS_NEW, RWS_OP_NEW, etc. */
      MHMT_RW,                /**< RenderWare allocations. Those that use RwMalloc, RwCalloc, RwRealloc, etc. */

      MHMT_TOTAL              /**< MUST be last, used to calculate the total number. NOT included in limiting count. */
   };

private:

   /**
   *
   * \ingroup MemHandler
   *
   * Private sub-structure of the CMemHandler class. Used to keep list of all memory allocations. This allows for
   * leak checking, but does use additional of memory. Can be enabled / disabled with RWS_MEMORY_LEAK_CHECK. The
   * items are used to log allocations. When a deallocation comes along, the matching allocation item will be
   * released. However, if bounds checking is enabled and a boundary violation is found, the item will not be
   * released, as it is used to store the boundary violation errors for later reporting. The boundary behavior is
   * controlled by RWS_MEMORY_BOUNDS_CHECK.
   *
   * The filename pointer my be zero and the line number be set to MH_NO_LINE. This occurs when a memory allocation
   * which is not via the RWS_NEW, RWS_OP_NEW, etc, macros is made. The reason for this is that memory allocations
   * via the global operator new and RwMalloc, etc, do not pass this information.
   *
   * The nesting level is used by the start and end block system within CMemHandler.
   *
   * \see CMemHandler
   * \see CMemHandler::StartBlock
   * \see CMemHandler::EndBlock
   * \see CMemHandler::mem_type
   * \see RWS_MEMORY_LEAK_CHECK
   * \see RWS_MEMORY_BOUNDS_CHECK
   * \see MH_MLI_UNDERRUN
   * \see MH_MLI_OVERRUN
   * \see RWS_MEMORY_TYPE_USER
   *
   */

   struct MemLogItem
   {
      const char *pFilename;  /**< Pointer to filename or zero. System uses the stored __FILE__ static values. */
      RwUInt32 lineNum,       /**< File line number where allocated from or MH_NO_LINE if unavailable. */
            memAddr,          /**< Stored for informational purposes and to sort the records.
                                   This is the address RETURNED TO THE CALLER, and NOT the internal header address. */
            size,             /**< This is the block size - the value RETURNED TO THE CALLER, NOT including
                                   header size, trailer size, etc. */
            errData           /**< The MH_MLI_... flags for each error type. Used by the boundary checking code */

#if RWS_MEMORY_USER_TYPE_METRICS

            ,
            userTypeNum;      /**< The user specified memory type at the time of allocation. Either user value
                                   offset from RWS_MEMORY_TYPE_USER or a RenderWare value. */
#else
            ;
#endif

      RwUInt8 nestLevel;      /**< This holds the 'start' & 'end' nesting level. Used in "during-run" leak tests. */
      mem_type type;          /**< The type of memory allocated. One of the CMemHandler::mem_type values. */
      MemLogItem *pNext;      /**< Next item, sorted in memory address order (only one list for all files, i.e. all
                                   allocs, regardless of file, are in one link-list). */
   };

   /**
   *
   * \ingroup MemHandler
   *
   * Private class of the CMemHandler class. Used to hold a name for a nesting level.
   *
   * \see CMemHandler
   * \see CMemHandler::StartBlock
   * \see CMemHandler::EndBlock
   *
   */

   class NestName
   {
   public:

      char m_name[RWS_MEMORY_NEST_NAME_LEN];    /**< The name of the nesting level. */

      /**
      *
      * initializes the name to a blank string.
      *
      */

      NestName(void)
      {
         *m_name = '\0';
      }

      /**
      *
      * Sets the name to the passed in string. This will be truncated, if necessary, to the
      * RWS_MEMORY_NEST_NAME_LEN.
      *
      * \see RWS_MEMORY_NEST_NAME_LEN
      * \param pName A pointer to name to store (may be truncated).
      * 
      */

      void Set(const char *pName)
      {
         if (pName)
         {
            strncpy(m_name, pName, RWS_MEMORY_NEST_NAME_LEN);
            m_name[RWS_MEMORY_NEST_NAME_LEN - 1] = '\0';
         }
      }

      /**
      *
      * Returns the currently stored nesting level name.
      *
      * \return a pointer to the nesting name, will be a valid string, but may be blank.
      *
      */

      const char *Get(void)
      {
         return(m_name);
      }
   };

   /**
   *
   * \ingroup MemHandler
   *
   * Private sub-class of CMemHandler. Base class used by all metric items, holds fields for current, peak,
   * total and number of memory allocations.
   *
   * \see CMemHandler
   * \see RWS_MEMORY_TYPE_USER
   *
   */

   class BaseMetricItem
   {
   public:

      RwUInt32 m_cur,         /**< Current amount of memory allocated in bytes. */
            m_total,          /**< Cumulative allocated memory count in bytes for type. */
            m_peak,           /**< Peak amount of memory allocated in bytes. */
            m_num;            /**< Number of allocation calls made. Rolling count. */

      /**
      *
      * initializes all count values.
      *
      */

      BaseMetricItem(void)
      {
         m_cur = 0;
         m_total = 0;
         m_peak = 0;
         m_num = 0;
      }
   };

   /**
   *
   * \ingroup MemHandler
   *
   * Private sub-class of CMemHandler. Used to keep track on the current, total and peak memory usage of basic
   * memory usage figures.
   *
   * \see CMemHandler
   *
   */

   class BasicMetricItem : public BaseMetricItem
   {
   public:

      RwUInt8 m_type;         /**< Type of allocation. One of the CMemHandler::mem_type values. */
   };

   /**
   *
   * \ingroup MemHandler
   *
   * Private sub-class of CMemHandler. Used to keep track on the current, total and peak memory usage of user
   * type memory usage figures.
   *
   * \see CMemHandler
   *
   */

   class UserPeakMetricItem : public BaseMetricItem
   {
   public:

      RwUInt32 m_userTypeNum;    /**< User type number or RenderWare reserved type number. */
   };

   /**
   *
   * \ingroup MemHandler
   *
   * Private sub-class of CMemHandler. Used for file based memory usage.
   *
   * \see CMemHandler
   *
   */

   class FileMetricItem : public BasicMetricItem
   {
   public:

      const char *m_pFilename;      /**< pointer to compiler's static filename string. */
      bool m_outputFlag;            /**< boolean flag used during reporting functions to indicate if processed. */

      /**
      *
      * Initializes the variables.
      *
      */

      FileMetricItem(void)
      {
         m_pFilename = 0;
         m_outputFlag = false;
      }

      /**
      *
      * Returns the compiler's static filename string or zero if not known. This will occur if the allocation
      * was made from a call which did not supply the filename (i.e. RenderWare or global operator new).
      *
      * \return a pointer to a filename string or zero.
      *
      */

      const char *GetFilename(void)
      {
         return (m_pFilename);
      }

      /**
      *
      * Gets the line number. This is a base class function. In this case it always returns 'MH_NO_LINE', but
      * derived classes, which do store the line number, can return the correct value (if stored).
      *
      * \see LineMetricItem
      * \see LineMetricItem::GetLine
      *
      * \return the line number within the file, in this case, always MH_NO_LINE.
      *
      */

      virtual RwUInt32 GetLine(void)
      {
         return (MH_NO_LINE);
      }

      /**
      *
      * Returns the user specified type number, this may be one of the standard types or a value offset
      * from the RWS_MEMORY_TYPE_USER value. However, in this case the value will always be
      * RWS_MEMORY_TYPE_UNKNOWN because file only metrics do not store this value.
      *
      * \return The stored user type or RenderWare number.
      * \see RWS_MEMORY_TYPE_UNKNOWN
      * \see LineMetricItem
      * \see LineMetricItem::GetUserType
      *
      */

      virtual RwUInt32 GetUserType(void)
      {
         return (RWS_MEMORY_TYPE_UNKNOWN);
      }
   };

   /**
   *
   * \ingroup MemHandler
   *
   * Private sub-class of CMemHandler. Used for file and line based memory usage. Also stores and reports
   * on user type of data (or RenderWare type).
   *
   * \see CMemHandler
   *
   */

   class LineMetricItem : public FileMetricItem
   {
   public:

      RwUInt32 m_line            /**< The line number of MH_NO_LINE if not known, i.e. RenderWare or
                                      global new alloc. */
#if RWS_MEMORY_USER_TYPE_METRICS

            ,
            m_userTypeNum;       /**< The user specified type number, supplied at allocation time. Either
                                      a user value offset from RWS_MEMORY_TYPE_USER or a RenderWare value. */
#else
            ;
#endif

      /**
      *
      * Returns the line number stored or MH_NO_LINE if no line stored.
      *
      * \return The line number or MH_NO_LINE.
      *
      */

      RwUInt32 GetLine(void)
      {
         return (m_line);
      }

      /**
      *
      * Returns the user specified type number, this may be one of the standard types or a value offset
      * from the RWS_MEMORY_TYPE_USER value.
      *
      * \return The stored user type or RenderWare number.
      * \see RWS_MEMORY_TYPE_RW
      * \see RWS_MEMORY_TYPE_STD
      * \see RWS_MEMORY_TYPE_RWS
      * \see RWS_MEMORY_TYPE_USER
      *
      */

      RwUInt32 GetUserType(void)
      {

#if RWS_MEMORY_USER_TYPE_METRICS

         return(m_userTypeNum);
#else
         return(RWS_MEMORY_TYPE_UNKNOWN);
#endif
      }
   };

   //
   // Main class functions, etc...
   //

   CMemHandler(void);                                 // Private, only want internal, Singleton, creation.

   // Stop any creation / duplication.

   CMemHandler(const CMemHandler &rhs);               // Copy constructor - not to be used.
   CMemHandler& operator =(const CMemHandler &rhs);   // Copy operator - not to be used.

   RwUInt32 GetExtraSize(void);
   static RwUInt32 GetAlignedHeaderSize(void);
   static RwUInt32 GetRawHeaderSize(void);
   static void *BuildHeader(void *pAllocStart, RwUInt32 size, mem_type type, void *pFileLineData);
   static bool CheckAndExtractHeader(void *&pStart, RwUInt32 &size, mem_type &type,
         void *&pFileLineData, RwBool ignoreErrors = FALSE);
   static void BuildTrailer(void *pUserAlloc, RwUInt32 userSize);
   static bool CheckTrailer(void *pUserAlloc, RwUInt32 userSize);
   static void SetupLeakData(void *pAllocMem, RwUInt32 userSize, const char *pFilename,
         RwUInt32 lineNum, mem_type type, RwUInt32 userTypeNum);
   static bool CheckLeakData(void *pUserMem, bool headerOkay, bool trailerOkay);
   static RwUInt32 GetMetricSize(RwUInt32 userSize);
   static void SetupBasicMetrics(RwUInt32 metSize, mem_type type, RwUInt32 userTypeNum);
   static void CloseBasicMetrics(RwUInt32 metSize, mem_type type, RwUInt32 UserTypeNum);
   static void *SetupFileLineMetrics(const char *pFilename, RwUInt32 lineNum, RwUInt32 metSize,
         mem_type type, RwUInt32 userTypeNum);
   static void ExtractFileLineInfo(void *pFileLineInfo, const char *&pFilename, RwUInt32 &lineNum);
   static void CloseFileLineMetrics(void *pFileLineData, RwUInt32 metSize);
   static void *SetupInfo(void *pAllocMem, RwUInt32 userSize, const char *pFilename,
         const RwUInt32 lineNum, mem_type type, RwUInt32 userTypeNum);
   static void *CloseInfo(void *pUserMem, RwUInt32 *pUserSize = 0);
   static void Test(bool condition, const char *msg);
   static void Report(const char *msg);
   static void *AddAlignment(void *pUserMem);
   static void *RemoveAlignment(void *pUserMem);
   static bool StringMatch(const char *pStrA, const char *pStrB);
   static void InitMemory(void *pMem, RwUInt32 size, RwUInt32 initValue);

   bool m_doingMsg;                                   /**<  Flag indicating if memory handler is processing a message
                                                            to the debug log (assert, reporting, etc). */

#if RWS_MEMORY_LOG && RWS_MEMORY_LEAK_CHECK
   MemLogItem m_memLogArray[RWS_MEMORY_MAX_MEM_LOGS]; /**<  Array of memory log items.
                                                            If using RWS_MEMORY_LEAK_CHECK. */

   MemLogItem *m_pRootMemLog,                         /**<  Memory item used root.
                                                            If using RWS_MEMORY_LEAK_CHECK. */

         *m_pFreeMemLog;                              /**<  Memory item free root.
                                                            If using RWS_MEMORY_LEAK_CHECK. */

   NestName m_nestNames[RWS_MEMORY_NEST_LEVELS];      /**<  Name of nested levels.
                                                            If using RWS_MEMORY_LEAK_CHECK. */

   RwUInt16 m_curNestLevel;                           /**<  Current nesting level.
                                                            If using RWS_MEMORY_LEAK_CHECK. */
#endif

#if RWS_MEMORY_LOG && RWS_MEMORY_BASIC_METRICS
   BasicMetricItem m_basicMetArray[MHMT_TOTAL];       /**<  Basic metric data per type.
                                                            If using RWS_MEMORY_BASIC_METRICS. */

   BasicMetricItem m_basicMetTotal;                   /**<  Total basic metrics for all types.
                                                            If using RWS_MEMORY_BASIC_METRICS. */
#endif

#if RWS_MEMORY_LOG && RWS_MEMORY_USER_PEAK_METRICS
   UserPeakMetricItem m_userPeakMetArray[
         RWS_MEMORY_MAX_USER_PEAK_METRICS];           /**<  User type peak metric data per user type.
                                                            If using RWS_MEMORY_USER_PEAK_METRICS. */

   RwUInt32 m_numUserPeakMetrics;                     /**<  Count of number of used user type peak metrics
                                                            If using RWS_MEMORY_USER_PEAK_METRICS. */
#endif


#if RWS_MEMORY_LOG && RWS_MEMORY_FILE_METRICS && !RWS_MEMORY_LINE_METRICS

   FileMetricItem m_fileMetArray[RWS_MEMORY_MAX_FILE_METRICS]; /**<  Per file & type metric data.
                                                                     If using RWS_MEMORY_FILE_METRICS
                                                                     & not RWS_MEMORY_LINE_METRICS. */

   RwUInt32 m_numFileMetrics;                                  /**<  Count of number of used file metrics.
                                                                     If using RWS_MEMORY_FILE_METRICS
                                                                     & not RWS_MEMORY_LINE_METRICS. */
#endif

#if RWS_MEMORY_LOG && RWS_MEMORY_LINE_METRICS

   LineMetricItem m_lineMetArray[RWS_MEMORY_MAX_LINE_METRICS]; /**<  Pre file & line metric data. If using
                                                                     RWS_MEMORY_LINE_METRICS. */

   RwUInt32 m_numLineMetrics;                                  /**<  Count of number of used line metrics.
                                                                     If using RWS_MEMORY_LINE_METRICS. */
#endif

public:

   /**
   *
   * Returns a reference to the 'singleton' instance of the memory handler. There can be only one instance of
   * the memory handler and it is held internally as a C++ 'singleton'. NOTE: that because this function holds
   * the 'singleton' static member, it must be called BEFORE any allocation to initialize the memory system.
   * This is handled automatically inside the 'Alloc', 'Calloc' and 'ReAlloc' functions. The reason for the
   * initialization is that when the function is called the static member is allocated and hence the constructor
   * is called - this causes the memory system to be initialized.
   *
   * \return a reference to the single instance of the memory handler.
   * \see Alloc
   * \see Calloc
   * \see ReAlloc
   * \see CMemHandler::CMemHandler
   *
   */

   static CMemHandler &GetRef(void)
   {
      static CMemHandler memHandler;                  // This variable is the global memory handler instance.
      return(memHandler);
   }

   ~CMemHandler(void);                                // MUST be public or code will NOT compile.

#ifdef RWS_DISABLE_MEMORY_CHECKING

   static void *Alloc(size_t size, mem_type type, RwUInt32 hint, RwUInt32 userTypeNum);
   static void *ReAlloc(void *pMem, size_t size, mem_type type, RwUInt32 hint, RwUInt32 userTypeNum);
   static void *Calloc(size_t num, size_t size, mem_type type, RwUInt32 hint, RwUInt32 userTypeNum);

#else

   static void *Alloc(size_t size, mem_type type, RwUInt32 hint, const char *pFilename,
         const RwUInt32 lineNum, RwUInt32 userTypeNum);
   static void *ReAlloc(void *pMem, size_t size, mem_type type, RwUInt32 hint, const char *pFilename,
         const RwUInt32 lineNum, RwUInt32 userTypeNum);
   static void *Calloc(size_t num, size_t size, mem_type type, RwUInt32 hint, const char *pFilename,
         const RwUInt32 lineNum, RwUInt32 userTypeNum);

#endif

   static void Free(void *pMem);
   static void CheckAndReport(void);
   static void StartBlock(const char *pName);
   static void EndBlock(void);
   static RwUInt32 ClearErrorLog(RwUInt8 minLevel = 0);
   static RwUInt32 CheckAllocatedBounds(RwUInt8 minLevel = 0);

   /**
   *
   * Returns a type name string for each of the mem_type values.
   *
   * \see CMemHandler::mem_type
   * \param type The type to return a name for.
   * \return a pointer to a string for the type or one containing a warning if the type is not known.
   *
   */

#if RWS_MEMORY_LOG

   static const char *GetTypeName(mem_type type)
   {
      Test(type < MHMT_TOTAL, "invalid type");

      const char *str = 0;

      switch (type)
      {
      case MHMT_STD:

         str = "Standard memory functions";
         break;

      case MHMT_RWS:

         str = "RenderWare Studio memory functions";
         break;

      case MHMT_RW:

         str = "RenderWare memory functions";
         break;

      default:

         str = "<< UNKNOWN >>";     // Should not get this displayed. If it is, then code needs to be updated.
         break;
      }

      return(str);
   }

#endif

   /**
   *
   * Returns a name string for the passed in userTypeNum value. This may be a RenderWare reserved
   * name, 'unknown' or a user type name. The return string will always be valid and MUST NOT be
   * freed or altered. If the name is a user name, ie offset from RWS_MEMORY_TYPE_USER, the function
   * attempts to get a name from the BaseMemGetName function. If this returns ZERO (indicating that
   * it cannot identify the type number) then this function will build an identifying string instead.
   *
   * \param userTypeNum The user type number for which a name is needed.
   * \return A STATIC string holding the identification string. This must NOT be released
   * or altered by the caller.
   * \see CheckAndReport
   * \see BaseMemGetName
   *
   */

#if RWS_MEMORY_LOG && (RWS_MEMORY_USER_TYPE_METRICS || RWS_MEMORY_USER_PEAK_METRICS)

   static const char *GetUserTypeName(RwUInt32 userTypeNum)
   {
      // Reserved name ?

      if (userTypeNum == RWS_MEMORY_TYPE_RW)
      {
         return(GetTypeName(MHMT_RW));
      }
      else if (userTypeNum == RWS_MEMORY_TYPE_STD)
      {
         return(GetTypeName(MHMT_STD));
      }
      else if (userTypeNum == RWS_MEMORY_TYPE_RWS)
      {
         return(GetTypeName(MHMT_RWS));
      }
      else if (userTypeNum == RWS_MEMORY_TYPE_UNKNOWN)
      {
         return("<< UNKNOWN >>");
      }
      else if (userTypeNum >= RWS_MEMORY_TYPE_USER)
      {
         // It's a user type. Need to check to see if this can be identified by
         // the base user memory naming function...

         const char *pUserName;

         if ((pUserName = BaseMemGetName(userTypeNum)) != 0)
         {
            // The function has supplied a name, return it.

            return(pUserName);
         }
         else
         {
            static char BuildName[32];

            // Function did not identify the name, so build one.

            sprintf(BuildName, "<< USER + %u >>", userTypeNum - RWS_MEMORY_TYPE_USER);
            return(BuildName);
         }
      }
      else
      {
         // Unknown type - new reserved value added and not added to this function?

         return("<< UNKNOWN RESERVED TYPE - Update CMemoryHandler::GetUserTypeName ? >>");
      }
   }

#endif

};

/**
*
* Initializes the memory handling class. Only one and is self initializing - it is not possible to directly
* construct the class. During compiling emits information on what options have been compiled in. Since there
* is only on instance, when it is created the BaseMemInit function is called to allow user define memory
* system to be constructed.
*
* \see BaseMemInit
*
*/

CMemHandler::CMemHandler(void) :
      m_doingMsg(false)
{
   // Initialize the base memory handler (may be user function).

   BaseMemInit();

   // Indicate to user if system is enabled (uses memory so must tell them).

#if RWS_MEMORY_LOG
#if defined(SN_TARGET_PS2)
// #pragma message not supported on prodg
//
#else
#pragma RWS_COMP_WARN(+---------------------------+)
#pragma RWS_COMP_WARN(| MEMORY HANDLER IS ENABLED |)
#pragma RWS_COMP_WARN(+---------------------------+)
#pragma RWS_COMP_WARN(| MUST be disabled for a -- |)
#pragma RWS_COMP_WARN(| FINAL build as it takes - |)
#pragma RWS_COMP_WARN(| additional memory on heap |)
#pragma RWS_COMP_WARN(+---------------------------+)
#endif
#endif

   // Conditionally initialize stuff...


#if RWS_MEMORY_LOG && RWS_MEMORY_LEAK_CHECK
#if defined(SN_TARGET_PS2)
// #pragma message not supported on prodg
//
#else
#pragma RWS_COMP_MSG(+-------------------------+)
#pragma RWS_COMP_MSG(| MEMORY LEAK CHECK IS IN |)
#pragma RWS_COMP_MSG(+-------------------------+)
#endif

   m_pRootMemLog = 0;
   m_pFreeMemLog = m_memLogArray;
   m_curNestLevel = 0;

   // Link all nodes into free list.

   for (RwUInt32 countA = 0; countA < RWS_MEMORY_MAX_MEM_LOGS - 1; countA++)
   {
      m_memLogArray[countA].pNext = m_memLogArray + countA + 1;
      m_memLogArray[countA].pFilename = 0;
   }

   m_memLogArray[RWS_MEMORY_MAX_MEM_LOGS - 1].pNext = 0;
#endif

   // Do leak detector setup...

#if RWS_MEMORY_LOG && RWS_MEMORY_LEAK_CHECK && RWS_MEMORY_BOUNDS_CHECK
#if defined(SN_TARGET_PS2)
// #pragma message not supported on prodg
//
#else
#pragma RWS_COMP_MSG(+---------------------------+)
#pragma RWS_COMP_MSG(| MEMORY BOUNDS CHECK IS IN |)
#pragma RWS_COMP_MSG(+---------------------------+)
#endif
#endif

   // Do basic metrics setup...

#if RWS_MEMORY_LOG && RWS_MEMORY_BASIC_METRICS
#if defined(SN_TARGET_PS2)
// #pragma message not supported on prodg
//
#else
#pragma RWS_COMP_MSG(+-------------------------+)
#pragma RWS_COMP_MSG(| MEMORY BASIC METRICS IN |)
#pragma RWS_COMP_MSG(+-------------------------+)
#endif   

   for (RwUInt32 countB = 0; countB < MHMT_TOTAL; countB++)
   {
      m_basicMetArray[countB].m_type = static_cast<mem_type>(countB);
   }
#endif

   // Do user type peak metric setup...

#if RWS_MEMORY_LOG && RWS_MEMORY_USER_PEAK_METRICS
#if defined(SN_TARGET_PS2)
// #pragma message not supported on prodg
//
#else
#pragma RWS_COMP_MSG(+-----------------------------+)
#pragma RWS_COMP_MSG(| MEMORY USER PEAK METRICS IN |)
#pragma RWS_COMP_MSG(+-----------------------------+)
#endif
   m_numUserPeakMetrics = 0;

#endif

   // Do file metric setup...

#if RWS_MEMORY_LOG && RWS_MEMORY_FILE_METRICS && !RWS_MEMORY_LINE_METRICS
#if defined(SN_TARGET_PS2)
// #pragma message not supported on prodg
//
#else
#pragma RWS_COMP_MSG(+------------------------+)
#pragma RWS_COMP_MSG(| MEMORY FILE METRICS IN |)
#pragma RWS_COMP_MSG(+------------------------+)
#endif
   m_numFileMetrics = 0;

#endif

   // Do line metric setup...

#if RWS_MEMORY_LOG && RWS_MEMORY_LINE_METRICS
#if defined(SN_TARGET_PS2)
// #pragma message not supported on prodg
//
#else
#pragma RWS_COMP_MSG(+------------------------+)
#pragma RWS_COMP_MSG(| MEMORY LINE METRICS IN |)
#pragma RWS_COMP_MSG(+------------------------+)
#endif
   m_numLineMetrics = 0;

#if RWS_MEMORY_USER_TYPE_METRICS
#if defined(SN_TARGET_PS2)
// #pragma message not supported on prodg
//
#else
#pragma RWS_COMP_MSG(+-------------------------------------------+)
#pragma RWS_COMP_MSG(| MEMORY PER FILE/LINE USER TYPE METRICS IN |)
#pragma RWS_COMP_MSG(+-------------------------------------------+)
#endif
#endif

#if RWS_MEMORY_INIT_VALUE
#if defined(SN_TARGET_PS2)
// #pragma message not supported on prodg
//
#else
#pragma RWS_COMP_MSG(+----------------------------------------------+)
#pragma RWS_COMP_MSG(| MEMORY IS BEING INITIALIZED - VERY EXPENSIVE |)
#pragma RWS_COMP_MSG(+----------------------------------------------+)
#endif
#endif

#endif

   // NOTE: Do NOT do reports in this function as it will cause infinite looping on the PS2.
}

/**
*
* Destroys the memory handling class. Calls the BaseMemClose function so user defined memory handlers can
* be closed.
*
* \see BaseMemClose
*
*/

CMemHandler::~CMemHandler(void)
{
   // All done, so close the base memory system (may be user function).
   //
   // NOTE: calling the 'CheckAndReport' function from here may not result in output, hence the closedown code
   //       should call 'MemHandlerCheckAndReport' instead.

   BaseMemClose();
}

/**
*
* Displays a report to the debug console (ensure the debug stream is still alive when you call it). The data
* written depends on the flags being used (see class description) but can include memory leaks for current level and
* sub-levels of, current, peak and total so far memory allocations. The latter can be basic, per file or per line
* in file - also the are NOT for the current level and sub-level, but are total from the start of the memory handler.
*
* NOTE: because this function's prototype must be contained within this code file (to reduce recompilation if
* flag states are changed) there is an access function for use from outside this code file - MemHandlerCheckAndReport.
*
* \see StartBlock
* \see EndBlock
* \see MemHandlerCheckAndReport
*
*/

void CMemHandler::CheckAndReport(void)
{

#if RWS_MEMORY_LOG

   char msg[RWS_MEMORY_MAX_MSG_LEN];

   sprintf(msg, "\n");              // Done this way to stop compiler's 'unused variable' error.
   Report(msg);                     // This may occur on with RWS_MEMORY_LOG set to zero.
   
   Report("###--------------------------###");
   Report("### CMemHandler check/report ###\n");

   // Indicate if memory handlers usage is included.

#if RWS_MEMORY_LOG && RWS_MEMORY_INC_MEM_SYS

   Report("   NOTE: Usage figures INCLUDE memory used by memory handler\n");

#else

   Report("   NOTE: Usage figures does NOT include memory used by memory handler\n");

#endif

   // Display basic (overall) memory usage metrics.

#if RWS_MEMORY_LOG && RWS_MEMORY_BASIC_METRICS

   Report("   Basic metric info...\n");
   for (RwUInt32 count = 0; count < MHMT_TOTAL; count++)
   {
      if (GetRef().m_basicMetArray[count].m_num)
      {
         sprintf(msg, "      Allocations of type [%s]",
               GetTypeName(static_cast<mem_type>(GetRef().m_basicMetArray[count].m_type)));
         Report(msg);
         sprintf(msg, "      :  Current usage of %u bytes", GetRef().m_basicMetArray[count].m_cur);
         Report(msg);
         sprintf(msg, "      :  Peak usage of %u bytes", GetRef().m_basicMetArray[count].m_peak);
         Report(msg);
         sprintf(msg, "      :  Total of %u bytes in %u calls\n", GetRef().m_basicMetArray[count].m_total,
               GetRef().m_basicMetArray[count].m_num);
         Report(msg);
      }
   }

   Report("      Combined allocations");
   sprintf(msg, "      :  Current usage of %u bytes", GetRef().m_basicMetTotal.m_cur);
   Report(msg);
   sprintf(msg, "      :  Peak usage of %u bytes", GetRef().m_basicMetTotal.m_peak);
   Report(msg);
   sprintf(msg, "      :  Total of %u bytes in %u calls\n", GetRef().m_basicMetTotal.m_total,
         GetRef().m_basicMetTotal.m_num);
   Report(msg);

#endif

   // Display user type (including peak) memory usage metrics.

#if RWS_MEMORY_LOG && RWS_MEMORY_USER_PEAK_METRICS

   Report("   User type metric info...\n");
   for (RwUInt32 userCount = 0; userCount < GetRef().m_numUserPeakMetrics; userCount++)
   {
      // As added on first allocation of type, will always have at least on allocation, unlike
      // the basic which is setup per pre-defined system type.

      sprintf(msg, "      Allocations of user type [%s]\n",
            GetRef().GetUserTypeName(GetRef().m_userPeakMetArray[userCount].m_userTypeNum));
      Report(msg);
      sprintf(msg, "      :  Current usage of %u bytes", GetRef().m_userPeakMetArray[userCount].m_cur);
      Report(msg);
      sprintf(msg, "      :  Peak usage of %u bytes", GetRef().m_userPeakMetArray[userCount].m_peak);
      Report(msg);
      sprintf(msg, "      :  Total of %u bytes in %u calls\n", GetRef().m_userPeakMetArray[userCount].m_total,
            GetRef().m_userPeakMetArray[userCount].m_num);
      Report(msg);
   }

#endif

   // Display per file memory usage info (uses file metric items here).

#if RWS_MEMORY_LOG && RWS_MEMORY_FILE_METRICS && !RWS_MEMORY_LINE_METRICS

   // Ensure all the 'output flags' are clear (may be called multiple times).

   for (RwUInt32 clearCount = 0; clearCount < GetRef().m_numFileMetrics; clearCount++)
   {
      GetRef().m_fileMetArray[clearCount].m_outputFlag = false;
   }

   // Display data...

   Report("   Per file metric info...\n");
   for (RwUInt32 fileCount = 0; fileCount < GetRef().m_numFileMetrics; fileCount++)
   {
      if (!GetRef().m_fileMetArray[fileCount].m_outputFlag)
      {
         sprintf(msg, "      %s(1) :\n", (GetRef().m_fileMetArray[fileCount].m_pFilename ?
               GetRef().m_fileMetArray[fileCount].m_pFilename : "UNKNOWN"));
         Report(msg);

         // Output info for each type, start at this un-output record and scan for all others.
         // Mark them as output, to stop secondary writing. Only need to look down the array.
         // No need to recheck the output flag, as if one from a file has been written, all
         // will have been written.

         for (RwUInt32 matchCount = fileCount; matchCount < GetRef().m_numFileMetrics; matchCount++)
         {
            if ((GetRef().m_fileMetArray[matchCount].m_pFilename ==
                  GetRef().m_fileMetArray[fileCount].m_pFilename) ||
                  (GetRef().m_fileMetArray[matchCount].m_pFilename &&
                  GetRef().m_fileMetArray[fileCount].m_pFilename &&
                  StringMatch(GetRef().m_fileMetArray[matchCount].m_pFilename,
                  GetRef().m_fileMetArray[fileCount].m_pFilename)))
            {
               sprintf(msg, "         Allocations of type [%s]",
                     GetTypeName(static_cast<mem_type>(GetRef().m_fileMetArray[matchCount].m_type)));
               Report(msg);
               
               sprintf(msg, "         :  Current usage of %u bytes",
                  GetRef().m_fileMetArray[matchCount].m_cur);
               Report(msg);
               
               sprintf(msg, "         :  Peak usage of %u bytes",
                  GetRef().m_fileMetArray[matchCount].m_peak);
               Report(msg);

               sprintf(msg, "         :  Total of %u bytes in %u calls\n",
                  GetRef().m_fileMetArray[matchCount].m_total,
                  GetRef().m_fileMetArray[matchCount].m_num);
               Report(msg);

               GetRef().m_fileMetArray[matchCount].m_outputFlag = true;
            }
         }
      }
   }

#endif

#if RWS_MEMORY_LOG && RWS_MEMORY_LINE_METRICS

   // Ensure all the 'output flags' are clear (may be called multiple times).

   for (RwUInt32 clearLineCount = 0; clearLineCount < GetRef().m_numLineMetrics; clearLineCount++)
   {
      GetRef().m_lineMetArray[clearLineCount].m_outputFlag = false;
   }

   // Display per file and per line info. Cannot show peaks for file, as it is stored per line and simply adding
   // them all together (or taking highest) will not yield a useful value, as you cannot tell which allocations
   // took place at the same time, etc. If this is needed, just 'FILE_METRICS' only (using file and file/line will
   // cause file/line to override and hence only give totals per file, not peaks).

   Report("   Per file / line metric info...\n");
   for (RwUInt32 lineCount = 0; lineCount < GetRef().m_numLineMetrics; lineCount++)
   {
      if (!GetRef().m_lineMetArray[lineCount].m_outputFlag)
      {
         RwUInt32 totals[MHMT_TOTAL] = {0},
               numCalls[MHMT_TOTAL] = {0},
               curs[MHMT_TOTAL] = {0};

         // Output info for each line, start at this un-output record and scan for all others.
         // Mark them as output, to stop secondary writing. Only need to look down the array.
         // No need to recheck the output flag, as if one from a file has been written, all
         // will have been written.

         for (RwUInt32 lineMatchCount = lineCount; lineMatchCount < GetRef().m_numLineMetrics; lineMatchCount++)
         {
            if ((GetRef().m_lineMetArray[lineMatchCount].m_pFilename ==
                  GetRef().m_lineMetArray[lineCount].m_pFilename) ||
                  (GetRef().m_lineMetArray[lineMatchCount].m_pFilename &&
                  GetRef().m_lineMetArray[lineCount].m_pFilename &&
                  StringMatch(GetRef().m_lineMetArray[lineMatchCount].m_pFilename,
                  GetRef().m_lineMetArray[lineCount].m_pFilename)))
            {
               sprintf(msg, "      %s(%u) : memory allocations from type [%s]",
                     (GetRef().m_lineMetArray[lineMatchCount].m_pFilename ?
                     GetRef().m_lineMetArray[lineMatchCount].m_pFilename : "UNKNOWN"),
                     (GetRef().m_lineMetArray[lineMatchCount].m_line != MH_NO_LINE ? 
                     GetRef().m_lineMetArray[lineMatchCount].m_line : 0),
                     GetTypeName(static_cast<mem_type>(GetRef().m_lineMetArray[lineMatchCount].m_type)));
               Report(msg);

#if RWS_MEMORY_USER_TYPE_METRICS

               sprintf(msg, "      :  User type [%s]",
                     GetRef().GetUserTypeName(GetRef().m_lineMetArray[lineMatchCount].GetUserType()));
               Report(msg);
#endif
               
               sprintf(msg, "      :  Current usage of %u bytes",
                  GetRef().m_lineMetArray[lineMatchCount].m_cur);
               Report(msg);
               
               sprintf(msg, "      :  Peak usage of %u bytes",
                  GetRef().m_lineMetArray[lineMatchCount].m_peak);
               Report(msg);

               sprintf(msg, "      :  Total of %u bytes in %u calls\n",
                  GetRef().m_lineMetArray[lineMatchCount].m_total,
                     GetRef().m_lineMetArray[lineMatchCount].m_num);
               Report(msg);

               GetRef().m_lineMetArray[lineMatchCount].m_outputFlag = true;
               curs[GetRef().m_lineMetArray[lineMatchCount].m_type] +=
                     GetRef().m_lineMetArray[lineMatchCount].m_cur;
               totals[GetRef().m_lineMetArray[lineMatchCount].m_type] +=
                     GetRef().m_lineMetArray[lineMatchCount].m_total;
               numCalls[GetRef().m_lineMetArray[lineMatchCount].m_type] +=
                     GetRef().m_lineMetArray[lineMatchCount].m_num;
            }
         }

#if RWS_MEMORY_FILE_METRICS

         Report("      Totals for file...");
         for (RwUInt32 totCount = 0; totCount < MHMT_TOTAL; totCount++)
         {
            if (numCalls[totCount])
            {
               sprintf(msg, "      :  of type [%s]", GetTypeName(static_cast<mem_type>(totCount)));
               Report(msg);
               sprintf(msg, "         :  Current usage of %u bytes", curs[totCount]);
               Report(msg);
               sprintf(msg, "         :  Total of %u bytes in %u calls", totals[totCount], numCalls[totCount]);
               Report(msg);

               // See above (beginning of file/line report) for reason why peaks are not reported here.
            }
         }
#endif
         Report("      ---\n");
      }
   }


#endif   // log & line metrics

   // Do bounds and memory leak listings. NOTE: because the memory handler cannot be guaranteed to be the last
   // thing shut down there may be memory leaks in the 'system' type that don't really exist.

#if RWS_MEMORY_LOG && RWS_MEMORY_LEAK_CHECK

   MemLogItem *pLogItem = GetRef().m_pRootMemLog;
   RwUInt32 totalErrs = 0,
         totalAllErrs = 0;

   if (pLogItem)
   {

#if RWS_MEMORY_BOUNDS_CHECK

      if (GetRef().m_curNestLevel)
      {
         sprintf(msg, "   Memory leaks & bounds errors for level [%u][%s] and below...\n", GetRef().m_curNestLevel,
               GetRef().m_nestNames[GetRef().m_curNestLevel - 1].Get());
         Report(msg);
      }
      else
      {
         Report("   Memory leaks & bounds error...\n");
      }

#else

      if (GetRef().m_curNestLevel)
      {
         sprintf(msg, "   Memory leaks (no bounds checks performed) for level [%u][%s] and below...\n",
               GetRef().m_curNestLevel, GetRef().m_nestNames[GetRef().m_curNestLevel - 1].Get());
         Report(msg);
      }
      else
      {
         Report("   Memory leaks (no bounds checks performed)...\n");
      }

#endif   // bound check (else)

   }

   while (pLogItem)
   {
      if (pLogItem->nestLevel >= GetRef().m_curNestLevel)
      {
         if (!pLogItem->errData)
         {
            // Memory leak (ignore 'std' if flag is set).

#if !RWS_MEMORY_REPORT_STD

            if (pLogItem->type != MHMT_STD)
#endif

            {

#if RWS_MEMORY_USER_TYPE_METRICS

               sprintf(msg, "      %s(%u) : NOT FREED (alloc was %u bytes @ addr %08X type [%s] user type [%s])",
                     (pLogItem->pFilename == '\0' ? "UNKNOWN" : pLogItem->pFilename),
                     (pLogItem->lineNum == MH_NO_LINE ? 0 : pLogItem->lineNum),
                     pLogItem->size, pLogItem->memAddr, GetTypeName(pLogItem->type),
                     GetUserTypeName(pLogItem->userTypeNum));
#else
               sprintf(msg, "      %s(%u) : NOT FREED (alloc was %u bytes @ addr %08X type [%s])",
                     (pLogItem->pFilename == '\0' ? "UNKNOWN" : pLogItem->pFilename),
                     (pLogItem->lineNum == MH_NO_LINE ? 0 : pLogItem->lineNum),
                     pLogItem->size, pLogItem->memAddr, GetTypeName(pLogItem->type));
#endif

               Report(msg);

               // Count reported errors (will EXCLUDE standard ones if RWS_MEMORY_REPORT_STD is disabled).

               totalErrs++;
            }

            // Need to count ALL errors too (always includes standard ones too).

            totalAllErrs++;
         }

#if RWS_MEMORY_BOUNDS_CHECK

         else
         {
            // Corruption.

#if RWS_MEMORY_USER_TYPE_METRICS

            sprintf(msg, "      %s(%u) : CORRUPTED [%s %s] (alloc was %u bytes @ "
                  "addr %08X type [%s] user type [%s])",
                  (pLogItem->pFilename == '\0' ? "UNKNOWN" : pLogItem->pFilename),
                  (pLogItem->lineNum == MH_NO_LINE ? 0 : pLogItem->lineNum),
                  ((pLogItem->errData & MH_MLI_UNDERRUN) ? "underrun" : ""),
                  ((pLogItem->errData & MH_MLI_OVERRUN) ? "overrun" : ""), pLogItem->size,
                  pLogItem->memAddr, GetTypeName(pLogItem->type),
                  GetUserTypeName(pLogItem->userTypeNum));
#else
            sprintf(msg, "      %s(%u) : CORRUPTED [%s %s] (alloc was %u bytes @ addr %08X type [%s])",
                  (pLogItem->pFilename == '\0' ? "UNKNOWN" : pLogItem->pFilename),
                  (pLogItem->lineNum == MH_NO_LINE ? 0 : pLogItem->lineNum),
                  ((pLogItem->errData & MH_MLI_UNDERRUN) ? "underrun" : ""),
                  ((pLogItem->errData & MH_MLI_OVERRUN) ? "overrun" : ""), pLogItem->size,
                  pLogItem->memAddr, GetTypeName(pLogItem->type));
#endif
            Report(msg);
            totalErrs++;
            totalAllErrs++;
         }

#endif

      }

      pLogItem = pLogItem->pNext;
   }

   Report("");
   sprintf(msg, "      total errors %u (including standard type = %u)\n", totalErrs, totalAllErrs);
   Report(msg);

   // If there were errors then assert.

#if RWS_MEMORY_ASSERT_ON_ERROR

   if (totalErrs)
   {
      Test(FALSE, "   CMemHandler::CheckAndReport - Memory leaks or boundary violations occurred.");
   }

#endif

#endif   // log and leak

   Report("###--------------------------###\n");

#endif      // #if RWS_MEMORY_LOG

}

/**
*
* Starts a sub-block. Causes memory leak checks to be tagged with this level and the CheckAndReport to
* only report from this level or sub-level of.
*
* NOTE: because this function's prototype must be contained within this code file (to reduce recompilation if
* flag states are changed) there is an access function for use from outside this code file - MemHandlerStartBlock.
*
* \see EndBlock
* \see CheckAndReport
* \see MemHandlerStartBlock
* \see CMemHandler::NestName
* \param pName The name given to the sub-block (may be truncated).
*
*/

void CMemHandler::StartBlock(const char *pName)
{
#if RWS_MEMORY_LOG && RWS_MEMORY_LEAK_CHECK

   Test(GetRef().m_curNestLevel < RWS_MEMORY_NEST_LEVELS, "too many nest levels");
   GetRef().m_nestNames[GetRef().m_curNestLevel].Set(pName);
   GetRef().m_curNestLevel++;

#endif
}

/**
*
* Ends a sub-block. If reporting of the sub-level's memory leaks, usage etc is require, then CheckAndReport must be
* called BEFORE this function.
*
* NOTE: because this function's prototype must be contained within this code file (to reduce recompilation if
* flag states are changed) there is an access function for use from outside this code file - MemHandlerEndBlock.
*
* \see StartBlock
* \see CheckAndReport
* \see MemHandlerEndBlock
*
*/

void CMemHandler::EndBlock(void)
{
#if RWS_MEMORY_LOG && RWS_MEMORY_LEAK_CHECK

   Test(GetRef().m_curNestLevel > 0, "already at base nest level");     // Start and ends out of sync most likely.
   GetRef().m_curNestLevel--;

#endif

}

/**
*
* Reports a message using RWS_TRACE. This function MUST BE USED within CMemHandler as a direct call to
* trace functions will cause a recursive call to be setup and eventually a stack overflow.
*
* \param msg The text string to output.
* \see Test
*
*/

void CMemHandler::Report(const char *msg)
{
   if (!GetRef().m_doingMsg)
   {
      GetRef().m_doingMsg = true;
      RWS_TRACE_NO_FUNC(msg);
      GetRef().m_doingMsg = false;
   }
}

/**
*
* Test will fire an assert if the test FAILS. This MUST BE USED in stead of direct calls to assert function, as the
* latter will result in a recursive call being setup and eventually a stack overflow.
*
* \param condition Flag test condition which must fail to cause assertion.
* \param msg The text string to display if the test does fail.
* \see Report
*
*/

void CMemHandler::Test(bool condition, const char *msg)
{
   if (!GetRef().m_doingMsg)
   {
      GetRef().m_doingMsg = true;
      RWS_ASSERT(condition, msg);
      GetRef().m_doingMsg = false;
   }
}

/**
*
* Returns number of extra bytes used in current memory handler config (see class description). Takes alignment issues
* into account. This function is not static - this is so that the 'GetRef' function must be used and in doing so, the
* memory system will be initialized via the CMemHandler constructor.
*
* \return the additional number of bytes needed for both header and trailer to store memory handler's data 
* before and after the user memory allocation.
* \see GetAlignedHeaderSize
* \see GetRef
*
*/

RwUInt32 CMemHandler::GetExtraSize(void)
{
   // Basic structure is [flags/size]...user mem...

   RwUInt32 extraHeader = GetAlignedHeaderSize(),
         extraTrailer = 0;

#if RWS_MEMORY_LOG && RWS_MEMORY_LEAK_CHECK && RWS_MEMORY_BOUNDS_CHECK

   // Adding bounds guards, now have ...header...[start guard]...user mem...[end guard]

   extraTrailer += (sizeof(RwUInt32) * RWS_MEMORY_NUM_BOUNDS);

#endif
   
   return(extraHeader + extraTrailer);
}

/**
*
* Returns the header size needed with the current configuration (see class description). Alignment is INCLUDED.
*
* \return the additional number of bytes needed for the header (with correct alignment for platform) for
* the memory handler's data to be store before the user allocation.
* \see GetRawHeaderSize
* \see AddAlignment
* \see RemoveAlignment
*
*/

RwUInt32 CMemHandler::GetAlignedHeaderSize(void)
{
   RwUInt32 headerSize = GetRawHeaderSize();
   headerSize = (headerSize + (RWS_MEMORY_ALIGNMENT - 1)) & ~(RWS_MEMORY_ALIGNMENT - 1);

   return(headerSize);
}

/**
*
* Returns the header size needed with the current configuration (see class description). This is the actual
* size and any 'aligning up' is NOT taken into account.
*
* \return the additional number of bytes needed for the header (without alignment for platform) for
* the memory handler's data to be store before the user allocation.
* \see GetAlignedHeaderSize
* \see AddAlignment
* \see RemoveAlignment
*
*/

RwUInt32 CMemHandler::GetRawHeaderSize(void)
{
   RwUInt32 extraHeader = 0;

#if RWS_MEMORY_LOG

   // Basic structure is [flags/size]...user mem...

   extraHeader += sizeof(RwUInt32);

#endif

#if RWS_MEMORY_LOG && RWS_MEMORY_LEAK_CHECK && RWS_MEMORY_BOUNDS_CHECK

   // Adding bounds guards, now have ...header...[start guard]...user mem...[end guard]

   extraHeader += (sizeof(RwUInt32) * RWS_MEMORY_NUM_BOUNDS);

#endif

#if RWS_MEMORY_LOG && (RWS_MEMORY_FILE_METRICS || RWS_MEMORY_LINE_METRICS)

   // Adding data pointer for these metrics, so header is now [flags/size][data ptr]...

   extraHeader += sizeof(RwUInt32);

#endif

   // Align the header to correct size.
   return (extraHeader);
}

/**
*
* Populates the header, given the current config. Returns a pointer to the user's memory location
* (first byte after header). Pass in the start of memory as seen by the allocation code (NOT the user), i.e. the
* first byte of where the header is to be stored. Data stored depends on the current configuration (see class
* description).
*
* \param pAllocStart A pointer to the allocated memory, where the header is to be inserted. This is NOT the memory
* which is returned to the allocation requester (user).
* \param size The of the memory block the user requested, i.e. does NOT include memory used by the memory handler.
* \param type The type of memory being allocated, RenderWare, global new, etc...
* \param pFileLineData A pointer to the data returned by the SetupFileLineMetrics function.
* \return Returns a pointer to the user memory, that is the first byte of actual aligned memory,
* after the memory handler's header data. This is the value returned to the caller of functions
*  such as RWS_OP_NEW, RwMalloc, etc.
*
* \see BuildTrailer
* \see SetupFileLineMetrics
* \see CheckAndExtractHeader
* \see AddAlignment
*
*/

void *CMemHandler::BuildHeader(void *pAllocStart, RwUInt32 size, mem_type type, void *pFileLineData)
{
   RwUInt32 *pData = static_cast<RwUInt32 *>(pAllocStart);

#if RWS_MEMORY_LOG

   Test(type < 16, "Type value CANNOT BE greater than 15 as nibble used for storage");
   Test((size & RWS_MEMORY_TYPE_MASK) == 0, "Memory size cannot be stored, too large for space");

   // encode size and type data.

   *pData = static_cast<RwUInt32>(type) << RWS_MEMORY_TYPE_SHIFT;
   *pData |= (size & ~RWS_MEMORY_TYPE_MASK);
   pData++;

#endif

   // insert file line data (if used).

#if RWS_MEMORY_LOG && (RWS_MEMORY_FILE_METRICS || RWS_MEMORY_LINE_METRICS)

   *pData = reinterpret_cast<RwUInt32>(pFileLineData);
   pData++;

#endif

   // If bounds check is to be used, needs to be at the end of the alignment, but need to 
   // ensure alignment is correct, so add size...

#if RWS_MEMORY_LOG && RWS_MEMORY_LEAK_CHECK && RWS_MEMORY_BOUNDS_CHECK

   pData += RWS_MEMORY_NUM_BOUNDS;

#endif

   // ...do the alignment...

   pData = static_cast<RwUInt32 *>(AddAlignment(static_cast<void *>(pData)));

   // ... and finally insert bounds guard (if used).

#if RWS_MEMORY_LOG && RWS_MEMORY_LEAK_CHECK && RWS_MEMORY_BOUNDS_CHECK

   for (RwInt32 count = 1; count <= static_cast<RwInt32>(RWS_MEMORY_NUM_BOUNDS); count++)
   {
      pData[-count] = RWS_MEMORY_HEAD_GUARD;
   }
   
#endif

   // Return aligned memory pointer (user memory).

   return(static_cast<void *>(pData));
}

/**
*
* Compares the two passed in strings for equality. Case is IGNORED and neither string is modified.
*
* \param pStrA A pointer to the first string (must NOT be 0 or NULL).
* \param pStrB A pointer to the second string (must NOT be 0 or NULL).
* \return Returns true if the string match (case ignored) or false otherwise.
* 
*/

bool CMemHandler::StringMatch(const char *pStrA, const char *pStrB)
{
   bool match = false;
   RwUInt32 ALen = strlen(pStrA);

   // Same length ?

   if (ALen == strlen(pStrB))
   {
      const char *pA = pStrA,
         *pB = pStrB,
         *pAEnd = pA + ALen;

      // Same length, so check match (case ignored).

      while (pA != pAEnd)
      {
         if (toupper(*pA) != toupper(*pB))
         {
            break;
         }
         pA++;
         pB++;
      }

      // Matched ?

      if (pA == pAEnd)
      {
         match = true;
      }
   }

   return(match);
}

/**
*
* Checks the header is valid. Returns the size, type and if the guard data is valid. What is check depends on the
* current configuration (see class description). Anything not configured will automatically 'pass'. Size and type
* returned via parameters and the function returns if the guard data is okay. If bound check is not compiled in,
* the function will always succeed. Pass in the user memory point, i.e. the one passed into RwFree, delete, etc. The
* code will automatically adjust the pointer to gain access to the memory handler's header. The adjusted pointer
* is returned.
*
* \param pStart On entry, the pointer to the user memory, i.e. that passed to RwFree, delete, etc. On exit a pointer to
* the memory handler's memory (alloc memory). This is the first byte of the memory handlers header. Note that these
* two may be the same if the memory handler is not using a header (RWS_MEMORY_LOG is disabled).
* \param userSize On exit this is set to the user memory size. That is the size of the block of memory the user
* requested. This does NOT include the size of any memory handler header / trailer.
* \param type On exit the type of the memory, i.e. RenderWare, global new, RenderWare Studio, etc.
* \param pFileLineData On exit a pointer to the memory needed by the CloseFileLineMetrics function.
* \param ignoreErrors This flag indicates if the function should assert on guard block errors. If the purpose of
* the call to this function is as a check prior to freeing the memory block this flag MUST be FALSE, otherwise
* the memory system may crash as a result of the head guard block being destroyed.
* \return true if the boundary check on the start of the user's memory block is still intact or if the boundary
* check is not compiled in. It will be false if the boundary check is compiled in and the guard block has been
* corrupted - i.e. there has been a write to memory before the start of the user's memory block (block underrun).
* \see BuildHeader
* \see CloseFileLineMetrics
* \see RemoveAlignment
* \see RWS_MEMORY_LOG
*
*/

bool CMemHandler::CheckAndExtractHeader(void *&pStart, RwUInt32 &userSize, mem_type &type,
      void *&pFileLineData, RwBool ignoreErrors)
{
   bool retVal = true;
   RwUInt32 *pData;

   pData = static_cast<RwUInt32 *>(pStart);

   // If being used, check the guard data (this is always JUST before the user memory). If there
   // is alignment padding it will occur BEFORE this. i.e....
   //
   // ]...pad...[START GUARD]...user memory...[END GUARD]
   //
   // This guard being the 'START GUARD'.
   //

#if RWS_MEMORY_LOG && RWS_MEMORY_LEAK_CHECK && RWS_MEMORY_BOUNDS_CHECK

   for (RwInt32 count = 1; count <= static_cast<RwInt32>(RWS_MEMORY_NUM_BOUNDS); count++)
   {
      if (pData[-count] != RWS_MEMORY_HEAD_GUARD)
      {
         retVal = false;

         // Check if need to assert.

         if (count == static_cast<RwInt32>(RWS_MEMORY_NUM_BOUNDS))
         {
            RwUInt8 *pFirst = reinterpret_cast<RwUInt8 *>(pData - count);

            if (!ignoreErrors)
            {
               Test(*pFirst == static_cast<RwUInt8>(RWS_MEMORY_HEAD_GUARD & 0xFF),
                     "Head guard destroyed, unsafe to continue");
            }
         }
      }
   }

#endif

   // Remove any additional bytes added due to alignment issues.

   pStart = RemoveAlignment(pStart);
   pData = static_cast<RwUInt32 *>(pStart);

   // If bounds check was in, need to skip passed the dword placement it made...

#if RWS_MEMORY_LOG && RWS_MEMORY_LEAK_CHECK && RWS_MEMORY_BOUNDS_CHECK

   pData -= RWS_MEMORY_NUM_BOUNDS;

#endif

   // If used, get file line metric data.

#if RWS_MEMORY_LOG && (RWS_MEMORY_FILE_METRICS || RWS_MEMORY_LINE_METRICS)

   pData--;
   pFileLineData = reinterpret_cast<void *>(*pData);

#endif

   // Get the memory size and type info out.

#if RWS_MEMORY_LOG

   pData--;
   type = static_cast<mem_type>((*pData & RWS_MEMORY_TYPE_MASK) >> RWS_MEMORY_TYPE_SHIFT);
   userSize = *pData & ~RWS_MEMORY_TYPE_MASK;

#endif

   // Store 'alloc start' memory pointer into start pointer.

   pStart = static_cast<void *>(pData);

   return(retVal);
}

/**
*
* Builds the trailer block (if any). Pass in the USER memory pointer, the first byte of data returned to the allocation
* caller and the size of the block requested by the user (NOT the size with memory handler's header / trail sizes
* added).
*
* \param pUserAlloc Pointer to the first byte of the user's memory.
* \param userSize The size of memory requested by the user (does NOT include memory handler's header and trailer).
* \see BuildHeader
* \see CheckTrailer
*
*/

void CMemHandler::BuildTrailer(void *pUserAlloc, RwUInt32 userSize)
{
#if RWS_MEMORY_LOG && RWS_MEMORY_LEAK_CHECK && RWS_MEMORY_BOUNDS_CHECK

   RwUInt8 *pBytes = static_cast<RwUInt8 *>(pUserAlloc);

   // Since the end pointer may not be 32bit aligned, need to write out each byte separately (non-PC platforms).

   pBytes += userSize;

   for (RwUInt32 count = 0; count < RWS_MEMORY_NUM_BOUNDS; count++)
   {
      *(pBytes++) = static_cast<RwUInt8>(RWS_MEMORY_TAIL_GUARD & 0x000000FF);
      *(pBytes++) = static_cast<RwUInt8>((RWS_MEMORY_TAIL_GUARD & 0x0000FF00) >> 8);
      *(pBytes++) = static_cast<RwUInt8>((RWS_MEMORY_TAIL_GUARD & 0x00FF0000) >> 16);
      *(pBytes++) = static_cast<RwUInt8>((RWS_MEMORY_TAIL_GUARD & 0xFF000000) >> 24);
   }

#endif
}

/**
*
* Checks the trailer is valid. Pass in the user memory pointer (pointer to first byte of user allocation) and the
* user memory size (excludes the memory handler's header and trailer sizes).
*
* \param pUserAlloc Pointer to the first byte of the user allocated memory (first byte after memory handler's header).
* \param userSize The size of the memory requested by the user (excludes size of memory handler's header and trailer).
* \return true if header is okay or not being used (controlled by RWS_MEMORY_BOUNDS_CHECK). Returns false if the
* block has been corrupted (check enabled). This indicates that the user has overridden the end of the memory block
* returned to them.
* \see BuildTrailer
* \see CheckAndExtractHeader
* \see RWS_MEMORY_BOUNDS_CHECK
*
*/

bool CMemHandler::CheckTrailer(void *pUserAlloc, RwUInt32 userSize)
{
   bool retVal = true;

#if RWS_MEMORY_LOG && RWS_MEMORY_LEAK_CHECK && RWS_MEMORY_BOUNDS_CHECK

   RwUInt8 *pBytes = static_cast<RwUInt8 *>(pUserAlloc);

   // Since the end pointer may not be 32bit aligned, need to check each byte separately (non-PC platforms).

   pBytes += userSize;

   for (RwUInt32 count = 0; count < RWS_MEMORY_NUM_BOUNDS; count++)
   {
      if (pBytes[0] != static_cast<RwUInt8>(RWS_MEMORY_TAIL_GUARD & 0x000000FF) ||
            pBytes[1] != static_cast<RwUInt8>((RWS_MEMORY_TAIL_GUARD & 0x0000FF00) >> 8) ||
            pBytes[2] != static_cast<RwUInt8>((RWS_MEMORY_TAIL_GUARD & 0x00FF0000) >> 16) ||
            pBytes[3] != static_cast<RwUInt8>((RWS_MEMORY_TAIL_GUARD & 0xFF000000) >> 24))
      {
         retVal = false;
         break;
      }

      pBytes += 4;
   }

#endif

   return(retVal);
}

/**
*
* Builds data record needed for memory leak checking. Only performs setup if compiled in (see class description).
*
* \param pAllocMem The pointer to the first byte of the memory handler's header (come BEFORE user memory).
* \param userSize The size of the memory block requested by the user (excludes the size of the memory handler's
* header and trailer).
* \param pFilename Pointer to the compiler's static filename for the allocation. Passed into the system by the
* allocation function. This may be zero if the allocation function does not support filename passing.
* \param lineNum The line number from the allocation function. May be MH_NO_LINE if the allocation function does
* not support line numbers.
* \param userTypeNum The user specified memory type number or the RenderWare value.
* \param type The memory type being used, RenderWare, global new, RenderWare Studio, etc.
* \see CheckLeakData
* \see RWS_MEMORY_LEAK_CHECK
* \see RWS_MEMORY_TYPE_USER
*
*/

void CMemHandler::SetupLeakData(void *pAllocMem, RwUInt32 userSize, const char *pFilename,
      RwUInt32 lineNum, mem_type type, RwUInt32 userTypeNum)
{
#if RWS_MEMORY_LOG && RWS_MEMORY_LEAK_CHECK

   MemLogItem *pLogEntry,
         *pCheckEntry,
         *pPrevEntry = 0;

   // Unlink new log entry from head of freelist. If run out, increase size of 'RWS_MEMORY_MAX_MEM_LOGS'.

   Test(GetRef().m_pFreeMemLog != 0, "ran out of memory log objects");
   pLogEntry = GetRef().m_pFreeMemLog;
   GetRef().m_pFreeMemLog = GetRef().m_pFreeMemLog->pNext;

   // Setup a log entry for the allocation.

   pLogEntry->size = userSize;
   pLogEntry->pFilename = pFilename;
   pLogEntry->lineNum = lineNum;
   pLogEntry->errData = 0;
   pLogEntry->type = type;

#if RWS_MEMORY_USER_TYPE_METRICS

   pLogEntry->userTypeNum = userTypeNum;

#endif
   
   pLogEntry->nestLevel = static_cast<RwUInt8>(GetRef().m_curNestLevel);
   pLogEntry->memAddr = reinterpret_cast<RwUInt32>(pAllocMem);

   // Link into list of used blocks, MUST be in memory address order.

   pCheckEntry = GetRef().m_pRootMemLog;
   while(pCheckEntry && pCheckEntry->memAddr < pLogEntry->memAddr)
   {
      pPrevEntry = pCheckEntry;
      pCheckEntry = pCheckEntry->pNext;
   }

   if (pPrevEntry)
   {
      // Belongs part way down list, check points to first which is >=, so
      // insert AFTER previous and BEFORE check... However, check may be NULL.

      pLogEntry->pNext = pPrevEntry->pNext;
      pPrevEntry->pNext = pLogEntry;
   }
   else
   {
      // No addresses <, so belongs at the root.

      pLogEntry->pNext = GetRef().m_pRootMemLog;
      GetRef().m_pRootMemLog = pLogEntry;
   }

#endif
}

/**
*
* Checks to see if there is a memory leak - if the option is compiled in (see class description).
*
* \param pAllocMem Pointer to the first by of the memory handler's header (comes BEFORE the user
* allocation memory).
* \param headerOkay A flag indicating if the header data was okay. Return value from CheckAndExtractHeader.
* \param trailerOkay A flag indicating if the trailer data was okay. Return value from CheckTrailer.
* \return true if everything is okay - i.e. there is no leak. Returns false if a leak was found.
* \see SetupLeakData
* \see CheckAndExtractHeader
* \see CheckTrailer
* \see RWS_MEMORY_LEAK_CHECK
*
*/

bool CMemHandler::CheckLeakData(void *pAllocMem, bool headerOkay, bool trailerOkay)
{
   bool retVal = true;

#if RWS_MEMORY_LOG && RWS_MEMORY_LEAK_CHECK

   MemLogItem *pLogEntry,
         *pPrevEntry = 0;
   RwUInt32 errData = 0;

#if RWS_MEMORY_BOUNDS_CHECK

   // Store error states.

   if (!headerOkay)
   {
      errData |= MH_MLI_UNDERRUN;
   }

   if (!trailerOkay)
   {
      errData |= MH_MLI_OVERRUN;
   }

#endif

   // Find a matching allocation block in the list, based on the deletion address (will
   // be the same as the allocation address). List is sorted in ascending address order,
   // but there may be one alloc and several deletes at the same address.

   pLogEntry = GetRef().m_pRootMemLog;
   while (pLogEntry)
   {
      // Does address match AND it's a allocation indicator.

      if (pLogEntry->memAddr == reinterpret_cast<RwUInt32>(pAllocMem) && !pLogEntry->errData)
      {
         break;
      }

      // Gone over ?

      if (pLogEntry->memAddr > reinterpret_cast<RwUInt32>(pAllocMem))
      {
         pLogEntry = 0;
         break;
      }

      pPrevEntry = pLogEntry;
      pLogEntry = pLogEntry->pNext;
   }

   // If the block is not found, then it was either a) not allocated (corrupt pointer for deletion) or b)
   // has already been freed. BTW - this function should NEVER be called with a NULL (0) pointer, as the
   // code assumes this will not happen. Caller (delete function) should ensure this is not the case.

   Test(pLogEntry != 0, "deleting a block that doesn't exist or has already been freed");
   
   // If block is not needed for problem logging, free it up.

   if (!errData)
   {
      // Unlink entry from used list...

      if (pPrevEntry)
      {
         // Mid-list.

         pPrevEntry->pNext = pLogEntry->pNext;
      }
      else
      {
         // Top of list, i.e. root.

         GetRef().m_pRootMemLog = pLogEntry->pNext;
      }

      // Link at the head of the free list.

      pLogEntry->pNext = GetRef().m_pFreeMemLog;
      GetRef().m_pFreeMemLog  = pLogEntry;
   }
   else
   {
      // Keep data block, but change data so it becomes a delete problem marker instead.

      pLogEntry->errData = errData;
      retVal = false;
   }

#endif

   return(retVal);
}

/**
*
* Returns the size value used for metric calculations. This is the allocation size used in basic, file and line/file
* metric calculations. It is either the user's requested memory size or this size plus the memory handlers header
* and trailer sizes. Which depends on the state of RWS_MEMORY_INC_MEM_SYS. Further information on the headers and
* trailers is given in the class description.
* 
* \param userSize The size of memory block the user requested.
* \return the size of memory to be use for the metrics.
* \see RWS_MEMORY_INC_MEM_SYS.
*
*/

RwUInt32 CMemHandler::GetMetricSize(RwUInt32 userSize)
{
   RwUInt32 metricSize = userSize;

#if RWS_MEMORY_LOG && RWS_MEMORY_INC_MEM_SYS

   // Size is different, as asked to include memory used by memory manager.

   metricSize += GetExtraSize();

#endif
   return(metricSize);
}

/**
*
* Sets up the basic metric data for memory usage tracking. The size should be obtained from 'GetMetricSize'. If this
* function is called the 'CloseBasicMetrics' function must also be called to 'balance' the allocations and
* deallocations. The function also sets up the user type peak metric data and likewise balances the values in the
* 'CloseBasicMetrics'.
*
* \param metSize The size to use for the metrics, returned by 'GetMetricSize'.
* \param type The memory type being allocated, RenderWare, global new, RenderWare Studio, etc.
* \param userTypeNum The user defined memory type number, offset from RWS_MEMORY_TYPE_USER or a
* RenderWare reserved memory type number.
* \see GetMetricSize
* \see CloseBasicMetrics
*
*/

void CMemHandler::SetupBasicMetrics(RwUInt32 metSize, mem_type type, RwUInt32 userTypeNum)
{
#if RWS_MEMORY_LOG && RWS_MEMORY_BASIC_METRICS

   // Requested basic metrics, so store values...

   GetRef().m_basicMetArray[type].m_cur += metSize;
   GetRef().m_basicMetArray[type].m_total += metSize;
   GetRef().m_basicMetArray[type].m_num++;

   if (GetRef().m_basicMetArray[type].m_cur > GetRef().m_basicMetArray[type].m_peak)
   {
      GetRef().m_basicMetArray[type].m_peak = GetRef().m_basicMetArray[type].m_cur;
   }

   // Do total values.

   GetRef().m_basicMetTotal.m_cur += metSize;
   GetRef().m_basicMetTotal.m_total += metSize;
   GetRef().m_basicMetTotal.m_num++;

   if (GetRef().m_basicMetTotal.m_cur > GetRef().m_basicMetTotal.m_peak)
   {
      GetRef().m_basicMetTotal.m_peak = GetRef().m_basicMetTotal.m_cur;
   }

#endif

#if RWS_MEMORY_LOG && RWS_MEMORY_USER_PEAK_METRICS

   // Requested user type peak metric data. First search for matching record...

   RwUInt32 index;

   for (index = 0; index < GetRef().m_numUserPeakMetrics; index++)
   {
      if (GetRef().m_userPeakMetArray[index].m_userTypeNum == userTypeNum)
      {
         break;
      }
   }

   // Found one ?

   if (index == GetRef().m_numUserPeakMetrics)
   {
      // No, need to add an extra one, if space. If asserts here need to increase the size of
      // RWS_MEMORY_MAX_USER_PEAK_METRICS constant.

      Test(index < RWS_MEMORY_MAX_USER_PEAK_METRICS,
            "Ran out of user type peak metric items, too many types. Increase array size.");

      GetRef().m_userPeakMetArray[index].m_cur = 0;
      GetRef().m_userPeakMetArray[index].m_num = 0;
      GetRef().m_userPeakMetArray[index].m_peak = 0;
      GetRef().m_userPeakMetArray[index].m_total = 0;
      GetRef().m_userPeakMetArray[index].m_userTypeNum = userTypeNum;

      GetRef().m_numUserPeakMetrics++;
   }

   // Increment the total values, etc...

   GetRef().m_userPeakMetArray[index].m_cur += metSize;
   GetRef().m_userPeakMetArray[index].m_total += metSize;
   GetRef().m_userPeakMetArray[index].m_num++;

   if (GetRef().m_userPeakMetArray[index].m_cur > GetRef().m_userPeakMetArray[index].m_peak)
   {
      GetRef().m_userPeakMetArray[index].m_peak = GetRef().m_userPeakMetArray[index].m_cur;
   }

#endif

}

/**
*
* Used to remove in basic metric data. MUST BE CALLED if 'SetupBasicMetrics' was. The function will
* also 'close' the data for the user type peak metrics. This is setup via the 'SetupBasicMetrics' so
* no additional function calls are needed.
*
* \param metSize The size of the block with metrics included. See 'GetMetricsSize'.
* \param type The type of memory being handled, RenderWare, global new, RenderWare Studio, etc.
* \param userTypeNum The user define type number offset from RWS_MEMORY_TYPE_USER or a RenderWare
* reserved value.
* \see SetupBasicMetrics
* \see GetMetricSize
* \see RWS_MEMORY_TYPE_USER
*
*/

void CMemHandler::CloseBasicMetrics(RwUInt32 metSize, mem_type type, RwUInt32 userTypeNum)
{
#if RWS_MEMORY_LOG && RWS_MEMORY_BASIC_METRICS

   // Reduce size of allocation for particular type...

   GetRef().m_basicMetArray[type].m_cur -= metSize;

   // Do total value too.

   GetRef().m_basicMetTotal.m_cur -= metSize;

#endif

#if RWS_MEMORY_LOG && RWS_MEMORY_USER_PEAK_METRICS

   RwUInt32 index;

   // Find item in the list.

   for (index = 0; index < GetRef().m_numUserPeakMetrics; index++)
   {
      if (GetRef().m_userPeakMetArray[index].m_userTypeNum == userTypeNum)
      {
         break;
      }
   }

   // Assert if it couldn't be found.

   Test (index < GetRef().m_numUserPeakMetrics, "Failed to find matching user type record.");

   // Reduce size of current allocation.

   GetRef().m_userPeakMetArray[index].m_cur -= metSize;

#endif

}

/**
*
* Setup the data blocks needed for file and line metrics. Data set will depend on compile options (see class
* description).
*
* \param pFilename The compiler static filename string for the allocation. This may be zero as not all allocation
* function can pass in a filename.
* \param lineNum The compiler line number for the allocation. This may be MH_NO_LINE if the allocation does not
* support line numbers.
* \param metSize The metric size, this is the value returned from 'GetMetricSize'.
* \param type The type of memory, RenderWare, etc.
* \param userTypeNum The user specified memory type number (offset from RWS_MEMORY_TYPE_USER) or RenderWare value.
* \return a pointer to the data needed by the 'BuildHeader' function.
* \see GetMetricSize
* \see BuildHeader
* \see CloseFileLineMetrics
* \see RWS_MEMORY_FILE_METRICS
* \see RWS_MEMORY_LINE_METRICS
* \see RWS_MEMORY_TYPE_RW
* \see RWS_MEMORY_TYPE_STD
* \see RWS_MEMORY_TYPE_RWS
* \see RWS_MEMORY_TYPE_USER
*
*/

void *CMemHandler::SetupFileLineMetrics(const char *pFilename, RwUInt32 lineNum, RwUInt32 metSize,
      mem_type type, RwUInt32 userTypeNum)
{
   void *pRetData = 0;

   // Do file only metrics...

#if RWS_MEMORY_LOG && RWS_MEMORY_FILE_METRICS && !RWS_MEMORY_LINE_METRICS

   FileMetricItem *pFileMetric,
         *pEndMetric = GetRef().m_fileMetArray + GetRef().m_numFileMetrics;

   // Search to see if the file has been added for this memory type...

   for (pFileMetric = GetRef().m_fileMetArray; pFileMetric < pEndMetric; pFileMetric++)
   {
      if (((pFileMetric->m_pFilename == pFilename) || (pFileMetric->m_pFilename && pFilename &&
            StringMatch(pFileMetric->m_pFilename, pFilename))) && pFileMetric->m_type == type)
      {
         break;
      }
   }

   // Found it? If not, add a new one.

   if (pFileMetric == pEndMetric)
   {
      // No add one. If run out here (ie asserts) then increase size of RWS_MEMORY_MAX_FILE_METRICS.

      Test(GetRef().m_numFileMetrics < RWS_MEMORY_MAX_FILE_METRICS, "out of file metric items, increase array");
      pFileMetric = GetRef().m_fileMetArray + GetRef().m_numFileMetrics;
      GetRef().m_numFileMetrics++;
      pFileMetric->m_pFilename = pFilename;
      pFileMetric->m_type = type;
   }

   // Update the record to reflect the new allocation.

   pFileMetric->m_cur += metSize;
   pFileMetric->m_total += metSize;
   pFileMetric->m_num++;

   if (pFileMetric->m_cur > pFileMetric->m_peak)
   {
      pFileMetric->m_peak = pFileMetric->m_cur;
   }

   pRetData = static_cast<void *>(pFileMetric);

#endif

   // Do line and file metrics (overrides the file only version).

#if RWS_MEMORY_LOG && RWS_MEMORY_LINE_METRICS

   LineMetricItem *pLineMetric,
         *pEndLineMetric = GetRef().m_lineMetArray + GetRef().m_numLineMetrics;

   // Search to see if the file/line has been added for this memory type...

   for (pLineMetric = GetRef().m_lineMetArray; pLineMetric < pEndLineMetric; pLineMetric++)
   {

#if RWS_MEMORY_USER_TYPE_METRICS

      if (((pLineMetric->m_pFilename == pFilename) || (pLineMetric->m_pFilename && pFilename &&
            StringMatch(pLineMetric->m_pFilename, pFilename))) && pLineMetric->m_type == type &&
            pLineMetric->m_line == lineNum && pLineMetric->m_userTypeNum == userTypeNum)
#else

      if (((pLineMetric->m_pFilename == pFilename) || (pLineMetric->m_pFilename && pFilename &&
            StringMatch(pLineMetric->m_pFilename, pFilename))) && pLineMetric->m_type == type &&
            pLineMetric->m_line == lineNum)
#endif
      {
         break;
      }
   }

   // Found it? If not, add a new one.

   if (pLineMetric == pEndLineMetric)
   {
      // No add one. If run out here (ie asserts) then increase size of RWS_MEMORY_MAX_LINE_METRICS.

      Test(GetRef().m_numLineMetrics < RWS_MEMORY_MAX_LINE_METRICS, "out of line metric items, inc array");
      pLineMetric = GetRef().m_lineMetArray + GetRef().m_numLineMetrics;
      GetRef().m_numLineMetrics++;
      pLineMetric->m_pFilename = pFilename;
      pLineMetric->m_line = lineNum;
      pLineMetric->m_type = type;

#if RWS_MEMORY_USER_TYPE_METRICS

      pLineMetric->m_userTypeNum = userTypeNum;

#endif

   }

   // Update the record to reflect the new allocation.

   pLineMetric->m_cur += metSize;
   pLineMetric->m_total += metSize;
   pLineMetric->m_num++;

   if (pLineMetric->m_cur > pLineMetric->m_peak)
   {
      pLineMetric->m_peak = pLineMetric->m_cur;
   }

   // Store pointer address, needed in code below.

   pRetData = static_cast<void *>(pLineMetric);

#endif

   return(pRetData);
}

/**
*
* Extracts the file and line number from the passed in file/line info pointer.
*
* \param pFileLineInfo The pointer returned by CheckAndExtractHeader or SetupFileLineMetric.
* \param pFilename A pointer which will be made to point to the compiler static filename.
* \param lineNum A variable which is set on exit to the line number.
* \see CheckAndExtractHeader
* \see SetupFileLineMetric
* \see CMemHandler::FileMetricItem
* \see CMemHandler::LineMetricItem
*
*/

void CMemHandler::ExtractFileLineInfo(void *pFileLineInfo, const char *&pFilename, RwUInt32 &lineNum)
{
#if RWS_MEMORY_LOG && (RWS_MEMORY_FILE_METRICS || RWS_MEMORY_LINE_METRICS)

   FileMetricItem *pFileMet = static_cast<FileMetricItem *>(pFileLineInfo);

   pFilename = pFileMet->GetFilename();
   lineNum = pFileMet->GetLine();

#endif
}

/**
*
* Closes the file line metric data. Basically remove deallocations from current count, etc. MUST be called if
* 'SetupFileLineMetrics' is.
*
* \param pFileLineData A data pointer returned by 'CheckAndExtractHeader' function.
* \param metSize The metric size returned by 'GetMetricSize' function.  
* \see GetMetricSize
* \see CheckAndExtractHeader
* \see SetupFileLineMetrics
* \see RWS_MEMORY_FILE_METRICS
* \see RWS_MEMORY_LINE_METRICS
*
*/

void CMemHandler::CloseFileLineMetrics(void *pFileLineData, RwUInt32 metSize)
{
#if RWS_MEMORY_LOG && (RWS_MEMORY_LINE_METRICS || RWS_MEMORY_FILE_METRICS)

   // Reduce ref count on file record. Since LineMetricItem is derived from FileMetricItem,
   // the pointer/code should be valid for both.

   FileMetricItem *pFileMet = static_cast<FileMetricItem *>(pFileLineData);
   pFileMet->m_cur -= metSize;

#endif
}

/**
*
* Sets up the header, trailer, leak logs and metric data for a memory block. What is setup depends on the compiler
* options (see class description). Used from Alloc, ReAlloc and Calloc functions (i.e. during allocations). If used
* then calls must be made to 'CloseInfo' too.
*
* \param pAllocMem A pointer to the first byte of the memory handler's header (come BEFORE user memory block).
* \param userSize The size of memory block the user requested (i.e. excludes memory handler's header & trail sizes).
* \param pFilename The compiler's static filename from the allocator or 0 if not supported.
* \param lineNum The line number from the allocator function or MH_NO_LINE if not supported by allocator.
* \param type The memory type, RenderWare, global new, RenderWare Studio, etc.
* \param userTypeNum The user specified memory type number / RenderWare number.
* \return a pointer to the first byte of the user allocation (first byte after memory handlers header).
* \see CloseInfo
* \see SetupBasicMetrics
* \see SetupFileLineMetrics
* \see BuildHeader
* \see BuildTrailer
* \see SetupLeakData
* \see Alloc
* \see ReAlloc
* \see Calloc
* \see RWS_MEMORY_TYPE_USER
*
*/

void *CMemHandler::SetupInfo(void *pAllocMem, RwUInt32 userSize, const char *pFilename,
      const RwUInt32 lineNum, mem_type type, RwUInt32 userTypeNum)
{
   Test(type < MHMT_TOTAL, "illegal memory type");

   void *pUserMem,
         *pFileLineData;
   RwUInt32 metSize;

   // Setup information blocks for the metrics.

   metSize = GetMetricSize(userSize);
   SetupBasicMetrics(metSize, type, userTypeNum);
   pFileLineData = SetupFileLineMetrics(pFilename, lineNum, metSize, type, userTypeNum);

   // Setup the header and trailers.

   pUserMem = BuildHeader(pAllocMem, userSize, type, pFileLineData);
   BuildTrailer(pUserMem, userSize);

   // Setup informational blocks for leak checking.

   SetupLeakData(pUserMem, userSize, pFilename, lineNum, type, userTypeNum);

   // Return the pointer to the users.

   return(pUserMem);
}

/**
*
* Closes the information structures. Used from Free and ReAlloc (i.e. during deallocations). MUST BE CALLED if the
* 'SetupInfo' function is used, so that metrics are balanced, etc.
*
* \param pUserMem The memory passed in by free, etc. This is known as the 'User pointer'. That is the first byte of
* the user memory. The function will obtain a pointer for the memory handler's header from this.
* \param pUserSize The size of the user memory block is returned via the variable this points to. If this data is
* not needed, the pointer can be set to zero.
* \return a pointer to the memory handler's header (first byte) or the 'Allocation pointer'.
* \see SetupInfo
* \see CheckAndExtractHeader
* \see CheckTrailer
* \see CheckLeakData
* \see CloseBasicMetrics
* \see CloseFileLineMetrics
* \see Free
* \see ReAlloc
*
*/

void *CMemHandler::CloseInfo(void *pUserMem, RwUInt32 *pUserSize)
{
   Test(pUserMem != 0, "function assumes pointer is non-null, please check in caller");

   void *pAllocMem = pUserMem,
         *pFileLineData = 0;
   mem_type type = MHMT_STD;
   RwUInt32 userSize = 0,
         metSize,
         userTypeNum = RWS_MEMORY_TYPE_UNKNOWN;
   bool headerOkay,
         trailerOkay;

   // Get the information...

   headerOkay = CheckAndExtractHeader(pAllocMem, userSize, type, pFileLineData);
   trailerOkay = CheckTrailer(pUserMem, userSize);

   // Check for leaks.

   CheckLeakData(pUserMem, headerOkay, trailerOkay);

   // Extract user type, if stored.

#if RWS_MEMORY_LOG && RWS_MEMORY_LINE_METRICS && RWS_MEMORY_USER_TYPE_METRICS

   userTypeNum = static_cast<LineMetricItem *>(pFileLineData)->m_userTypeNum;

#endif

   // Do metrics...

   metSize = GetMetricSize(userSize);
   CloseBasicMetrics(metSize, type, userTypeNum);
   CloseFileLineMetrics(pFileLineData, metSize);

   // Return the user memory block size ?

   if (pUserSize)
   {
      *pUserSize = userSize;
   }

   return(pAllocMem);
}

/**
*
* Aligns UP the passed in pointer so it is correct for the platform.
*
* \param pUserMem The pointer to the user memory to be aligned. That is the first byte of the memory to return to
* the user.
* \return the pointer to user memory aligned up to the next valid, aligned, memory address.
* \see GetRawHeaderSize
* \see GetAlignedHeaderSize
* \see RemoveAlignment
*
*/

void *CMemHandler::AddAlignment(void *pUserMem)
{
   RwUInt32 loc = reinterpret_cast<RwUInt32>(pUserMem);
   loc -= GetRawHeaderSize();
   loc += GetAlignedHeaderSize();

   return(reinterpret_cast<void *>(loc));
}

/**
*
* Removes the alignment value and returns the pointer to what it would have been without it (via the GetRawExtraSize
* function).
*
* \param pAlignedUserMem A pointer to the aligned first byte of user memory. That is the first byte returned to the
* caller of the allocation function.
* \return a pointer to where the unaligned pointer would have been if no alignment had been applied.
* \see GetRawHeaderSize
* \see GetAlignedHeaderSize
* \see AddAlignment
*
*/

void *CMemHandler::RemoveAlignment(void *pAlignedUserMem)
{
   RwUInt32 loc = reinterpret_cast<RwUInt32>(pAlignedUserMem);
   loc += GetRawHeaderSize();
   loc -= GetAlignedHeaderSize();

   return(reinterpret_cast<void *>(loc));

}

/**
*
* Goes through the memory log items and removes any which are there for the purpose of logging a boundary
* violation, memory leak or other free-time error. The removal will only occur on items with a level
* number equal to or higher than the one specified. This can be useful if doing memory logging on leaks
* on game level load, etc. Clearing the error log before loading and then calling CheckAndReport after
* unloading would then hi-light only errors on that particular load/unload cycle instead of cumulatively
* on all load/unload cycles. Memory logging (RWS_MEMORY_LOG is 1) is required for function to work. The
* spaces in the log used by any cleared errors will be returned to the logging system for use on further
* errors.
*
* \param minLevel The level to clear from. Any error with this block level number or higher will be cleared.
* Any at a lower level number will not.
* \return Returns the number of cleared error log items.
* \see StartBlock
* \see EndBlock
* \see CheckAndReport
* \see RWS_MEMORY_LOG
* \see MemHandlerClearErrLog
*
*/

RwUInt32 CMemHandler::ClearErrorLog(RwUInt8 minLevel)
{
#if RWS_MEMORY_LOG

   MemLogItem *pCur = GetRef().m_pRootMemLog,
         *pLast = 0;
   RwUInt32 numCleared = 0;

   while (pCur)
   {
      if (pCur->errData && pCur->nestLevel >= minLevel)
      {
         MemLogItem *pMove;

         // Move to next time and keep current for deletion. Do NOT update last though, as
         // removing the item.

         pMove = pCur;
         pCur = pCur->pNext;

         // Close would-be hole in list.

         if (pLast)
         {
            pLast->pNext = pCur;
         }
         else
         {
            // Must be at head.

            GetRef().m_pRootMemLog = pCur;
         }

         // Move this item to head of free list and note that a delete has been made.

         pMove->pNext = GetRef().m_pFreeMemLog;
         GetRef().m_pFreeMemLog = pMove;
         numCleared++;
      }
      else
      {
         // Does not require deletion, so move on, updating last.

         pLast = pCur;
         pCur = pCur->pNext;
      }
   }

   return(numCleared);

#else

   return(0);

#endif
}

/**
*
* Checks the currently allocated memory blocks to see if any violations of their boundarys have occurred.
* In this way it is not necessary to wait until the block is freed for the check to occur, as
* CheckAndReport would. This function checks all blocks with a block number of the passed in value or
* higher. This function will only work if RWS_MEMORY_LOG, RWS_MEMORY_LEAK_CHECK and
* RWS_MEMORY_BOUNDS_CHECK are all set to 1. If RWS_MEMORY_ASSERT_ON_ERROR is defined, then if an error
* is found then an assert will occur. Any found boundary errors will be printed to the log window. This
* occurs BEFORE any assertions.
*
* \param minLevel The level to check from. Any allocations with this block level number or higher will
* be checked. Any at a lower level number will not.
* \return Returns the number of found boundary violations.
* \see StartBlock
* \see EndBlock
* \see CheckAndReport
* \see RWS_MEMORY_LOG
* \see RWS_MEMORY_LEAK_CHECK
* \see RWS_MEMORY_BOUNDS_CHECK
* \see RWS_MEMORY_ASSERT_ON_ERROR
* \see CheckAndExtractHeader
* \see CheckTrailer
* \see MemHandlerCheckBounds
*
*/

RwUInt32 CMemHandler::CheckAllocatedBounds(RwUInt8 minLevel)
{
#if RWS_MEMORY_LOG && RWS_MEMORY_LEAK_CHECK && RWS_MEMORY_BOUNDS_CHECK

   MemLogItem *pCur = GetRef().m_pRootMemLog;
   RwUInt32 numErrs = 0;

   // Display report to log...

   Report("###--------------------------------------------------------------###");
   Report("### CMemHandler check on boundary violations on allocated memory ###\n");

   while (pCur)
   {
      bool headerOkay,
            trailerOkay;
      RwUInt32 userSize;
      mem_type type;
      void *pFileLineData;
      void *pUserMem,
            *pAllocMem;

      // Get the information... Need two pointers as 'CheckAndExtractHeader' will modify the
      // pointer passed into it to point to the allocation address instead of the user address.
      //
      // NOTE: call to CheckAndExtractHeader ignores errors, this is ONLY safe because the code
      //       here does not go on to release (free) the memory blocks.

      pUserMem = reinterpret_cast<void *>(pCur->memAddr);
      pAllocMem = reinterpret_cast<void *>(pCur->memAddr);

      headerOkay = CheckAndExtractHeader(pAllocMem, userSize, type, pFileLineData, TRUE);
      trailerOkay = CheckTrailer(pUserMem, userSize);

      // Test it's okay, log if it's not.

      if (!headerOkay || !trailerOkay)
      {
         char msg[RWS_MEMORY_MAX_MSG_LEN];

         // Display error to log...


#if RWS_MEMORY_USER_TYPE_METRICS

         sprintf(msg, "      %s(%u) : CORRUPTED [%s %s] (alloc was %u bytes @ addr %08X type [%s] user type [%s])",
               (pCur->pFilename == '\0' ? "UNKNOWN" : pCur->pFilename),
               (pCur->lineNum == MH_NO_LINE ? 0 : pCur->lineNum),
               (headerOkay ? "" : "underrun"),
               (trailerOkay ? "" : "overrun"), pCur->size,
               pCur->memAddr, GetTypeName(pCur->type),
               GetUserTypeName(pCur->userTypeNum));
#else
         sprintf(msg, "      %s(%u) : CORRUPTED [%s %s] (alloc was %u bytes @ addr %08X type [%s])",
               (pCur->pFilename == '\0' ? "UNKNOWN" : pCur->pFilename),
               (pCur->lineNum == MH_NO_LINE ? 0 : pCur->lineNum),
               ((pCur->errData & MH_MLI_UNDERRUN) ? "underrun" : ""),
               ((pCur->errData & MH_MLI_OVERRUN) ? "overrun" : ""), pCur->size,
               pCur->memAddr, GetTypeName(pCur->type));
#endif
         Report(msg);

         // Increment error count.

         numErrs++;
      }

      // Do the next one...

      pCur = pCur->pNext;
   }

   // Finish logging...

   if (!numErrs)
   {
      Report("   ZERO boundary error found in allocated memory blocks.");
      Report("\n###--------------------------------------------------------------###");
   }
   else
   {
      char msg[RWS_MEMORY_MAX_MSG_LEN];

      sprintf(msg, "\n   FOUND %u boundary errors in allocated memory.", numErrs);
      Report(msg);
      Report("\n###--------------------------------------------------------------###");

      // Need to assert ?

#if RWS_MEMORY_ASSERT_ON_ERROR

      Test(FALSE, "CMemHandler::CheckAllocatedBounds - Boundary errors found within "
            "currently allocated memory blocks.");
#endif

   }

   return(numErrs);

#else

   return(0);

#endif
}

/**
*
* Initializes the passed in memory block to the passed in initialization value.
*
* \param pMem A pointer to the memory to initialize. This MUST be valid and not zero.
* \param size The size of the block of memory pointed to in bytes.
* \param initValue The 32bit value to initialize the memory to. If the memory is not
* a multiple of 4, then the remaining bytes will use the appropriate number of bytes
* from this value.
*
*/

void CMemHandler::InitMemory(void *pMem, RwUInt32 size, RwUInt32 initValue)
{
   RwUInt32 *p32;
   RwUInt8 *p8;

   // Do 32bit values - destroys size value in process.

   p32 = static_cast<RwUInt32 *>(pMem);

   while (size >= 4)
   {
      size -= 4;
      *(p32++) = initValue;
   }

   // Do 8bit values - destroys initValue in process.

   p8 = reinterpret_cast<RwUInt8 *>(p32);

#ifdef DOLPHIN

   // MSB first.

   while (size)
   {      
      size--;
      *(p8++) = static_cast<RwUInt8>((initValue & 0xFF000000) >> 24);
      initValue <<= 8;
   }

#else

   // LSB first.

   while (size)
   {
      size--;
      *(p8++) = static_cast<RwUInt8>(initValue & 0x000000FF);
      initValue >>= 8;
   }

#endif
}

/**
*
* Allocates a block of memory and sets up leak, boundary and metric data for it. Uses the BaseMemAlloc function to
* obtain the block of memory needed. Will initialize memory to know value (RWS_MEMORY_ALLOC_INIT) if the
* RWS_MEMORY_INIT_VALUE flag is set to 1.
*
* NOTE : If memory handling is disabled the 'line' and 'file' parameters are no longer passed. Therefore
*        it is necessary to use a '#ifdef RWS_DISABLE_MEMORY_CHECKING' preprocessor directive and call
*        the appropriate version of the function.
*
* \param userSize the size of the memory block requested by the user.
* \param type the type of memory being allocated, RenderWare, global new, RenderWare Studio, etc...
* \param pFilename the compiler static filename string for where the allocation was made or zero if the allocator
* cannot provide the information.
* \param lineNum the line number for where the allocation was made or MH_NO_LINE if the allocator cannot provide
* the information.
* \param userTypeNum the user allocation type number (some allocator force this to a set value). This is passed to the
* BaseMemAlloc function, but is not used by the CMemHandler class itself.
* \return a pointer to the user memory block or zero (NULL) on failure. Will be correctly aligned for the platform.
* \see SetupInfo
* \see BaseMemAlloc
* \see GetExtraSize
* \see Free
* \see ReAlloc
* \see Calloc
*
*/

#ifdef RWS_DISABLE_MEMORY_CHECKING

void *CMemHandler::Alloc(size_t userSize, mem_type type, RwUInt32 hint, const RwUInt32 userTypeNum)

#else

void *CMemHandler::Alloc(size_t userSize, mem_type type, RwUInt32 hint, const char *pFilename, const RwUInt32 lineNum,
      const RwUInt32 userTypeNum)

#endif

{
   void *pAllocMem,
         *pUserMem;

   // Allocate the memory block - use of 'GetRef' will cause system to initialize.

   pAllocMem = BaseMemAlloc(userSize + GetRef().GetExtraSize(), userTypeNum);

#if RWS_MEMORY_LOG

   Test(pAllocMem != 0, "could not allocate memory");
   
   // Setup information blocks for the metrics.

#ifdef RWS_DISABLE_MEMORY_CHECKING

   pUserMem = SetupInfo(pAllocMem, userSize, 0, MH_NO_LINE, type, userTypeNum);

#else

   pUserMem = SetupInfo(pAllocMem, userSize, pFilename, lineNum, type, userTypeNum);

#endif

#else

   pUserMem = pAllocMem;

#endif

#if RWS_MEMORY_LOG && RWS_MEMORY_INIT_VALUE

   // Initialize the memory to the appropriate 'allocated' value.

   InitMemory(pUserMem, userSize, RWS_MEMORY_ALLOC_INIT);

#endif

   // Note allocation in workspace memory tracker.

#ifdef RWS_DISABLE_MEMORY_CHECKING

   RWS::MemoryProfile::Malloc(pUserMem, userSize, 0, MH_NO_LINE, userTypeNum);

#else

   RWS::MemoryProfile::Malloc(pUserMem, userSize, pFilename, lineNum, userTypeNum);

#endif

   // Return the pointer to the users.

   return(pUserMem);
}

/**
*
* Reallocates a block of memory and sets up leak, boundary and metric data for it. Uses the BaseMemReAlloc function to
* obtain the block of memory needed, copy data, free up old block, etc. See standard C library manuals for reference
* information on 'realloc'.
*
* When the RWS_MEMORY_INIT_VALUE is set to 1, the system will initialize extended memory blocks to the
* RWS_MEMORY_ALLOC_INIT value and if setting the userSize to 0 with a valid pUserMem block, the memory being freed
* will be set to RWS_MEMORY_FREE_INIT. However, if during a resize operation (pUserMem != 0 & userSize != 0) then,
* if the memory is moved to another location, the old memory will NOT be set to RWS_MEMORY_FREE_INIT. If a completely
* new memory block is being allocated (previous memory was non-existent) then the whole block will be set to
* the RWS_MEMORY_ALLOC_INIT value.
*
* NOTE : If memory handling is disabled the 'line' and 'file' parameters are no longer passed. Therefore
*        it is necessary to use a '#ifdef RWS_DISABLE_MEMORY_CHECKING' preprocessor directive and call
*        the appropriate version of the function.
*
* \param pUserMem the previous memory block (if one).
* \param userSize the size of the new memory block requested by the user.
* \param type the type of memory being allocated, RenderWare, global new, RenderWare Studio, etc...
* \param pFilename the compiler static filename string for where the allocation was made or zero if the allocator
* cannot provide the information.
* \param lineNum the line number for where the allocation was made or MH_NO_LINE if the allocator cannot provide
* the information.
* \param userTypeNum the user allocation type number (some allocator force this to a set value). This is passed to the
* BaseMemReAlloc function, but is not used by the CMemHandler class itself.
* \return a pointer to the user memory block or zero (NULL) on failure. Will be correctly aligned for the platform.
* If zero is returned, then the previous block will not have been deallocated.
* \see SetupInfo
* \see CloseInfo
* \see BaseMemReAlloc
* \see GetExtraSize
* \see CheckAndExtractHeader
* \see ExtractFileLineInfo
* \see Alloc
* \see Free
* \see Calloc
* \see RWS_MEMORY_INIT_VALUE
*
*/

#ifdef RWS_DISABLE_MEMORY_CHECKING

void *CMemHandler::ReAlloc(void *pUserMem, size_t userSize, mem_type type, RwUInt32 hint, RwUInt32 userTypeNum)

#else

void *CMemHandler::ReAlloc(void *pUserMem, size_t userSize, mem_type type, RwUInt32 hint, const char *pFilename,
      RwUInt32 lineNum, RwUInt32 userTypeNum)

#endif

{
   void *pAllocMem,
         *pNewAlloc,
         *pNewUser;

   // Doing a free or a new allocation ?

   if (userSize)
   {
      // New allocation - is there old memory to be handled? If so, MUST close it, as if realloc changes
      // the memory location, the old data (and hence header, trailer, etc) will be lost.

#if RWS_MEMORY_LOG
      RwUInt32 oldUserSize = 0;
      mem_type oldType = MHMT_TOTAL;
      void *pFileLineInfo = 0;
      const char *pOldFilename = 0;
      RwUInt32 oldLineNum = 0;

      if (pUserMem)
      {
         void *pUserMemCopy = pUserMem;

         // Need size and type too (in case need to reset).

         CheckAndExtractHeader(pUserMemCopy, oldUserSize, oldType, pFileLineInfo);
         ExtractFileLineInfo(pFileLineInfo, pOldFilename, oldLineNum);

         // Close it.

         pAllocMem = CloseInfo(pUserMem);
      }
      else
      {
         pAllocMem = 0;
      }

#else

      pAllocMem = pUserMem;

#endif

      // Do the reallocation (attempt it). Use of 'GetRef' will cause system to initialize.

      pNewAlloc = BaseMemReAlloc(pAllocMem, userSize + GetRef().GetExtraSize(), userTypeNum);

      // Did it allocate ?

#if RWS_MEMORY_LOG

      if (pNewAlloc)
      {
         // Have a new block of memory, so set it up.

         pNewUser = SetupInfo(pNewAlloc, userSize, pFilename, lineNum, type, userTypeNum);

#if RWS_MEMORY_INIT_VALUE

         // The part of this up to the total size or previous size (smaller of) is carried
         // over (ie copied) but the area above this, if any, must be initialized if needed.

         if (userSize > oldUserSize)
         {
            RwUInt8 *pInit = static_cast<RwUInt8 *>(pNewUser);

            InitMemory(static_cast<void *>(pInit + oldUserSize),
                  userSize - oldUserSize, RWS_MEMORY_ALLOC_INIT);
         }
#endif

      }
      else if (pAllocMem)
      {
         // No and there was memory before so must reset the info structures, as the old block
         // of memory will still be active (according to rules of realloc).

         SetupInfo(pAllocMem, oldUserSize, pOldFilename, oldLineNum, oldType, userTypeNum);
         pNewUser = 0;

         // NOTE: Do NOT clear this block of memory to any initialization values. As returning
         //       user's old block, must maintain contents.
      }

#else

      pNewUser = pNewAlloc;

#endif

   }
   else
   {
      // Free - must handle size being zero this time.

#if RWS_MEMORY_LOG

      if (pUserMem)
      {

#if RWS_MEMORY_INIT_VALUE

         RwUInt32 oldUserSize;
         void *pUserMemCopy = pUserMem,
               *pFileLineInfo = 0;
         mem_type oldType = MHMT_TOTAL;

         // Get additional information needed for setting to free value.

         CheckAndExtractHeader(pUserMemCopy, oldUserSize, oldType, pFileLineInfo);
#endif

         pAllocMem = CloseInfo(pUserMem);

#if RWS_MEMORY_INIT_VALUE

         // Freeing memory, so set to 'freeing' value.

         InitMemory(pUserMem, oldUserSize, RWS_MEMORY_FREE_INIT);
#endif
      }
      else
      {
         pAllocMem = 0;
      }

#else

      pAllocMem = pUserMem;

#endif

      // Call base realloc to handle it (will, or should, return 0).

      pNewUser = BaseMemReAlloc(pAllocMem, userSize, userTypeNum);
   }

#ifdef RWS_DISABLE_MEMORY_CHECKING

   RWS::MemoryProfile::Realloc(pUserMem, pNewUser, userSize, 0, MH_NO_LINE, userTypeNum);

#else

   RWS::MemoryProfile::Realloc(pUserMem, pNewUser, userSize, pFilename, lineNum, userTypeNum);

#endif

   return(pNewUser);
}

/**
*
* Allocates a block of memory and clears it to zero. Also sets up leak, boundary and metric data for it. Uses the
* BaseMemCalloc function to obtain the block of memory needed / clear it. It should be noted that when RWS_MEMORY_LOG
* is enabled, the num and size values are changed. This is due to the extra memory needed by the memory system. The
* values are changed to: size = (size * num) + extra needed; num = 1. When RWS_MEMORY_LOG is disabled, the values are
* not changed. This function will ALWAYS set the memory to zero, even if the RWS_MEMORY_INIT_VALUE flag is set to 1.
*
* NOTE : If memory handling is disabled the 'line' and 'file' parameters are no longer passed. Therefore
*        it is necessary to use a '#ifdef RWS_DISABLE_MEMORY_CHECKING' preprocessor directive and call
*        the appropriate version of the function.
*
* \param num the number of items to allocate contiguously.
* \param size the size of each of the memory block requested by the user. Total = num * size.
* \param type the type of memory being allocated, RenderWare, global new, RenderWare Studio, etc...
* \param pFilename the compiler static filename string for where the allocation was made or zero if the allocator
* cannot provide the information.
* \param lineNum the line number for where the allocation was made or MH_NO_LINE if the allocator cannot provide
* the information.
* \param userTypeNum the user allocation type number (some allocator force this to a set value). This is passed to the
* BaseMemCalloc function, but is not used by the CMemHandler class itself.
* \return a pointer to the user memory block or zero (NULL) on failure. Will be correctly aligned for the platform.
* \see Alloc
* \see ReAlloc
* \see Free
*
*/

#ifdef RWS_DISABLE_MEMORY_CHECKING

void *CMemHandler::Calloc(size_t num, size_t size, mem_type type, RwUInt32 hint, RwUInt32 userTypeNum)

#else

void *CMemHandler::Calloc(size_t num, size_t size, mem_type type, RwUInt32 hint, const char *pFilename,
      RwUInt32 lineNum, RwUInt32 userTypeNum)

#endif

{
   void *pAllocMem,
         *pUserMem;
   size_t allocSize;

   // Allocating in blocks, so if extra size is needed, calculate it in blocks. Check that the
   // block size isn't zero though... NOTE: Use of 'GetRef' will cause system to initialize.
   // Therefore DO NOT compile out this section of code if RWS_MEMORY_LOG is disabled. Doing
   // so would cause the memory system to fail on certain platforms (SN Systems GameCube) because
   // the memory system would not be initialized.

#if RWS_MEMORY_LOG

   // Need extra space, so change parameters.

   size = size * num;
   allocSize = size + GetRef().GetExtraSize();
   num = 1;

#else

   allocSize = size;

#endif

   // Allocate the memory block (using calloc - just in case user has shortcut for zeroed memory).

   pAllocMem = BaseMemCalloc(num, allocSize, userTypeNum);

#if RWS_MEMORY_LOG

   Test(pAllocMem != 0, "could not allocate memory (calloc)");

   // Setup information blocks for the metrics.

   pUserMem = SetupInfo(pAllocMem, size * num, pFilename, lineNum, type, userTypeNum);

#else

   pUserMem = pAllocMem;

#endif

#ifdef RWS_DISABLE_MEMORY_CHECKING

   RWS::MemoryProfile::Calloc(pUserMem, size * num, 0, MH_NO_LINE, userTypeNum);

#else

   RWS::MemoryProfile::Calloc(pUserMem, size * num, pFilename, lineNum, userTypeNum);

#endif

   // Return the pointer to the users.

   return(pUserMem);
}

/**
*
* Frees up a block of memory, performs leak checks, checks for boundary violations and updates (closes) metric
* information. If the RWS_MEMORY_INIT_VALUE flag is set to 1, then just before the memory is freed, it will be
* initialized to the RWS_MEMORY_FREE_INIT value.
*
* \param pUserMem A pointer to the first byte of the user block to be deallocated. This CANNOT BE ZERO (NULL). The
* caller must check for this case.
* \see Alloc
* \see Calloc
* \see ReAlloc
* \see BaseMemFree
* \see CloseInfo
*
*/

void CMemHandler::Free(void *pUserMem)
{
   void *pAllocMem;

#if RWS_MEMORY_LOG && RWS_MEMORY_INIT_VALUE

   RwUInt32 userSize;

#endif

   // Close information system

#if RWS_MEMORY_LOG

#if RWS_MEMORY_INIT_VALUE

   pAllocMem = CloseInfo(pUserMem, &userSize);

#else

   pAllocMem = CloseInfo(pUserMem);

#endif

#else

   pAllocMem = pUserMem;

#endif

#if RWS_MEMORY_LOG && RWS_MEMORY_INIT_VALUE

   // Just before freeing, set memory to know value.

   InitMemory(pUserMem, userSize, RWS_MEMORY_FREE_INIT);

#endif

   RWS::MemoryProfile::Free(pUserMem);

   // Free the memory.

   BaseMemFree(pAllocMem);
}

/**
*
* \ingroup MemHandler
*
* Function to allow calling of check/report function without needed to include large amounts of code
* and function definitions (macro flags especially) in the header file (i.e. less to recompile if change
* the flags). Reports allocations (all levels) and leaks in the current level/sub-levels. Just call
* function in CMemHandler.
*
* \see CMemHandler
* \see CMemHandler::CheckAndReport
* \see MemHandlerStartBlock
* \see MemHandlerEndBlock
* \see MemHandlerCheckBounds
* \see MemHandlerClearErrLog
*
*/

void MemHandlerCheckAndReport(void)
{
   CMemHandler::CheckAndReport();
}

/**
*
* \ingroup MemHandler
*
* Marks the start of a memory block. Used for grouping memory checks, say setup, then mark, then load level data.
* When level closes call 'MemHandlerCheckAndReport' to list any leaks in the block and then 'MemHandlerEndBlock'
* to close the block and drop back a level. The system is nested. Just call function in CMemHandler. Needed to
* stop large recompiles if memory flags are changed.
*
* \param pName The pointer to a name to use for the level. In C++ the default for this is to set it to ZERO, ie
* no name is specified. In C, this value would have to be specified as a string or set to ZERO.
* \see CMemHandler
* \see CMemHandler::StartBlock
* \see MemHandlerCheckAndReport
* \see MemHandlerEndBlock
* \see MemHandlerCheckBounds
* \see MemHandlerClearErrLog
*
*/

void MemHandlerStartBlock(const char *pName)
{
   CMemHandler::StartBlock(pName);
}

/**
*
* \ingroup MemHandler
*
* Marks the end of a sub-block in the memory handler. Just call function in CMemHandler. Needed to stop large
* recompiles if memory flags are changed.
*
* \see CMemHandler
* \see CMemHandler::EndBlock
* \see MemHandlerCheckAndReport
* \see MemHandlerStartBlock
* \see MemHandlerCheckBounds
* \see MemHandlerClearErrLog
*
*/

void MemHandlerEndBlock(void)
{
   CMemHandler::EndBlock();
}

/**
*
* \ingroup MemHandler
*
* Checks the currently allocated blocks to see if any have a boundary violation. Starts at the specified
* block level and returns any at that block or a higher one. See CMemHandler::CheckAllocatedBounds for
* more information.
*
* \param minLevel The minimum block level to start checking at. Default in C++ is ZERO.
* \return The number of found blocks with one or more boundary violations.
* \see CMemHandler
* \see CMemHandler::CheckAllocatedBounds
* \see MemHandlerCheckAndReport
* \see MemHandlerStartBlock
* \see MemHandlerClearErrLog
* \see MemHandlerEndBlock
*
*/

RwUInt32 MemHandlerCheckBounds(RwUInt32 minLevel)
{
   return(CMemHandler::CheckAllocatedBounds(minLevel));
}

/**
*
* \ingroup MemHandler
*
* Clears log entries from the memory handler's error log. See CMemHandler::ClearErrorLog for more
* information. Note that the returned number of errors are allocations. So if any given allocation
* cause more than one type of error (leak, boundary over-run & under-run) it will only be reported
* as ONE error.
*
* \param minLevel The minimum block level to start checking at. Default in C++ is ZERO.
* \return The number of cleared error log items..
* \see CMemHandler
* \see CMemHandler::ClearErrorLog
* \see MemHandlerCheckAndReport
* \see MemHandlerStartBlock
* \see MemHandlerCheckBounds
* \see MemHandlerEndBlock
*
*/

RwUInt32 MemHandlerClearErrLog(RwUInt32 minLevel)
{
   return(CMemHandler::ClearErrorLog(minLevel));
}

/**
*
* \ingroup MemHandler
*
* Directly calls the memory systems allocation code. To be used by classes which override the operator
* new functions but still need to use the memory handler to allocate memory etc.
*
* NOTE : If memory handling is disabled the 'line' and 'file' parameters are no longer passed. Therefore
*        it is necessary to use a '#ifdef RWS_DISABLE_MEMORY_CHECKING' preprocessor directive and call
*        the appropriate version of the function.
*
* \param size The size of the memory block to allocate.
* \param file A pointer to the static filename string for the file allocating the memory. May be 0.
* \param line The line number within the file where the allocation occurs. Can be MH_NO_LINE if unknown.
* \param type The user memory type number.
* \return A pointer to the allocated memory or 0 on failure to allocate.
* \see MemHandlerNewHintEx
* \see MemHandlerNewHint
* \see MemHandlerNew
* \see MemHandlerDel
* \see CMemHandler
* \see CMemHandler::Alloc
*
*/

#if defined(RWS_DISABLE_MEMORY_CHECKING) && !defined(DOXYGEN)

void *MemHandlerNewEx(size_t size, RwUInt32 type)
{
   return(CMemHandler::Alloc(size, CMemHandler::MHMT_RWS, rwMEMHINTDUR_GLOBAL, type));
}

#else

void *MemHandlerNewEx(size_t size, const char *file, int line, RwUInt32 type)
{
   return(CMemHandler::Alloc(size, CMemHandler::MHMT_RWS, rwMEMHINTDUR_GLOBAL, file, line, type));
}

#endif

/**
*
* \ingroup MemHandler
*
* Directly calls the memory systems allocation code. To be used by classes which override the operator
* new functions but still need to use the memory handler to allocate memory etc. This version also takes
* a RenderWare hint value for the memory being allocated.
*
* NOTE : If memory handling is disabled the 'line' and 'file' parameters are no longer passed. Therefore
*        it is necessary to use a '#ifdef RWS_DISABLE_MEMORY_CHECKING' preprocessor directive and call
*        the appropriate version of the function.
*
* \param size The size of the memory block to allocate.
* \param file A pointer to the static filename string for the file allocating the memory. May be 0.
* \param line The line number within the file where the allocation occurs. Can be MH_NO_LINE if unknown.
* \param type The user memory type number.
* \param hint One of the RenderWare hint values.
* \return A pointer to the allocated memory or 0 on failure to allocate.
* \see MemHandlerNew
* \see MemHandlerNewHint
* \see MemHandlerNewEx
* \see MemHandlerDel
* \see CMemHandler
* \see CMemHandler::Alloc
*
*/

#if defined(RWS_DISABLE_MEMORY_CHECKING) && !defined(DOXYGEN)

void *MemHandlerNewHintEx(size_t size, RwUInt32 type, RwMemoryHintDuration hint)
{
   return(CMemHandler::Alloc(size, CMemHandler::MHMT_RWS, hint, type));
}

#else

void *MemHandlerNewHintEx(size_t size, const char *file, int line, RwUInt32 type, RwMemoryHintDuration hint)
{
   return(CMemHandler::Alloc(size, CMemHandler::MHMT_RWS, hint, file, line, type));
}

#endif

/**
*
* \ingroup MemHandler
*
* Directly calls the memory systems allocation code. To be used by classes which override the operator
* new functions but still need to use the memory handler to allocate memory etc. This is the basic version
* which just takes desired block size.
*
* \param size The size of the memory block to allocate.
* \return A pointer to the allocated memory or 0 on failure to allocate.
* \see MemHandlerNewHint
* \see MemHandlerNewEx
* \see MemHandlerNewHintEx
* \see MemHandlerDel
* \see CMemHandler
* \see CMemHandler::Alloc
*
*/

void *MemHandlerNew(size_t size)
{

#ifdef RWS_DISABLE_MEMORY_CHECKING

   return(CMemHandler::Alloc(size, CMemHandler::MHMT_RWS, rwMEMHINTDUR_GLOBAL, RWS_MEMORY_TYPE_RWS));

#else

   return(CMemHandler::Alloc(size, CMemHandler::MHMT_RWS, rwMEMHINTDUR_GLOBAL, 0, MH_NO_LINE, RWS_MEMORY_TYPE_RWS));

#endif

}

/**
*
* \ingroup MemHandler
*
* Directly calls the memory systems allocation code. To be used by classes which override the operator
* new functions but still need to use the memory handler to allocate memory etc. This is one step up 
* from the basic version which takes desired block size and a RenderWare hint value.
*
* \param size The size of the memory block to allocate.
* \param hint One of the RenderWare hint values.
* \return A pointer to the allocated memory or 0 on failure to allocate.
* \see MemHandlerNew
* \see MemHandlerNewEx
* \see MemHandlerNewHintEx
* \see MemHandlerDel
* \see CMemHandler
* \see CMemHandler::Alloc
*
*/

void *MemHandlerNewHint(size_t size, RwMemoryHintDuration hint)
{

#ifdef RWS_DISABLE_MEMORY_CHECKING

   return(CMemHandler::Alloc(size, CMemHandler::MHMT_RWS, hint, RWS_MEMORY_TYPE_RWS));

#else

   return(CMemHandler::Alloc(size, CMemHandler::MHMT_RWS, hint, 0, MH_NO_LINE, RWS_MEMORY_TYPE_RWS));

#endif

}

/**
*
* \ingroup MemHandler
*
* Directly calls the memory systems deallocation code. To be used by classes which override the operator
* delete functions but still need to use the memory handler to deallocate memory etc.
*
* \param pMem The block of memory to deallocate.
* \see MemHandlerNewHint
* \see MemHandlerNew
* \see CMemHandler
* \see CMemHandler::Free
*
*/

void MemHandlerDel(void *pMem)
{
   if (pMem)
   {
      CMemHandler::Free(pMem);
   }
}

/**
*
* \ingroup MemHandler
*
* Redefinition of the global new function (non-array) so it uses the RenderWare Studio standardized
* memory handler class, CMemHandler. This is the C++ standard new function and should not be called for
* code from within RenderWare Studio. All code for here should use the version of new which supplies
* file and line information. To do this, use the RWS_NEW, RWS_OP_NEW, RWS_NEW_TYPE and RWS_OP_NEW_TYPE
* macros.
*
* Note memory type is set to MHMT_STD and the user type number is set to RWS_MEMORY_TYPE_STD. No line or
* file information is supplied (unavailable).
*
* \param size The size, in bytes, of the block to allocate.
* \return a pointer to a memory block or zero (NULL) on failure.
* \see CMemHandler
* \see CMemHandler::Alloc
* \see RWS_NEW
* \see RWS_NEW_TYPE
* \see RWS_OP_NEW
* \see RWS_OP_NEW_TYPE
*
*/

void *operator new(size_t size)
{
   // NOTE: must always call CMemHandler so that initialization of underlying memory system occurs.

#ifdef RWS_DISABLE_MEMORY_CHECKING

   return(CMemHandler::Alloc(size, CMemHandler::MHMT_STD, rwMEMHINTDUR_GLOBAL, RWS_MEMORY_TYPE_STD));

#else

   return(CMemHandler::Alloc(size, CMemHandler::MHMT_STD, rwMEMHINTDUR_GLOBAL, 0, MH_NO_LINE, RWS_MEMORY_TYPE_STD));

#endif

}

/**
*
* \ingroup MemHandler
*
* Redefinition of the global new function (array) so it uses the RenderWare Studio standardized
* memory handler class, CMemHandler. This is the C++ standard new function and should not be called for
* code from within RenderWare Studio. All code for here should use the version of new which supplies
* file and line information. To do this, use the RWS_NEW and RWS_NEW_TYPE macros.
*
* Note memory type is set to MHMT_STD and the user type number is set to RWS_MEMORY_TYPE_STD. No line or
* file information is supplied (unavailable).
*
* \param size The size, in bytes, of the block to allocate.
* \return a pointer to a memory block or zero (NULL) on failure.
* \see CMemHandler
* \see CMemHandler::Alloc
* \see RWS_NEW
* \see RWS_NEW_TYPE
*
*/

void *operator new[](size_t size)
{
   // NOTE: must always call CMemHandler so that initialization of underlying memory system occurs.

#ifdef RWS_DISABLE_MEMORY_CHECKING

   return(CMemHandler::Alloc(size, CMemHandler::MHMT_STD, rwMEMHINTDUR_GLOBAL, RWS_MEMORY_TYPE_STD));

#else

   return(CMemHandler::Alloc(size, CMemHandler::MHMT_STD, rwMEMHINTDUR_GLOBAL, 0, MH_NO_LINE, RWS_MEMORY_TYPE_STD));

#endif

}

/**
*
* \ingroup MemHandler
*
* Redefinition of the global delete function (non-array) so it uses the RenderWare Studio standardized
* memory handler class, CMemHandler. This is the C++ standard delete function but is also called for
* memory allocated with the RWS_NEW, RWS_OP_NEW, RWS_NEW_TYPE and RWS_OP_NEW_TYPE macros.
*
* \param pMem A pointer to the block of memory to be freed.
* \see CMemHandler
* \see CMemHandler::Free
* \see RWS_NEW
* \see RWS_NEW_TYPE
* \see RWS_OP_NEW
* \see RWS_OP_NEW_TYPE
*
*/

void operator delete(void *pMem)
{
   if (pMem)
   {
      CMemHandler::Free(pMem);
   }
}

/**
*
* \ingroup MemHandler
*
* Redefinition of the global delete function (array) so it uses the RenderWare Studio standardized
* memory handler class, CMemHandler. This is the C++ standard delete function but is also called for
* memory allocated with the RWS_NEW and RWS_NEW_TYPE macros.
*
* \param pMem A pointer to the block of memory to be freed.
* \see CMemHandler
* \see CMemHandler::Free
* \see RWS_NEW
* \see RWS_NEW_TYPE
*
*/

void operator delete[](void *pMem)
{
   if (pMem)
   {
      CMemHandler::Free(pMem);
   }
}

/**
*
* \ingroup MemHandler
*
* Redefinition of the global new function (non-array) so it uses the RenderWare Studio standardized
* memory handler class, CMemHandler. This is the C++ standard new function and should not be called for
* code from within RenderWare Studio.
*
* This version is called as a result of using RWS_NEW, RWS_OP_NEW, RWS_NEW_TYPE and RWS_OP_NEW_TYPE. It
* provides additional information over and above the 'standard new', such as filename, line number and
* a user type number.
*
* Note memory type is set to MHMT_RWS and the user type number, with RWS_NEW and RWS_OP_NEW, is set
* to RWS_MEMORY_TYPE_STD. With RWS_NEW_TYPE and RWS_OP_NEW_TYPE the user can set this value.
*
* \param size The size, in bytes, of the block to allocate.
* \param file The compiler static filename of where the allocation occurs.
* \param line The compiler line number of where the allocation occurs.
* \param userTypeNum The user defined memory type number passed in from RWS_NEW_TYPE and RWS_OP_NEW_TYPE.
* \return a pointer to a memory block or zero (NULL) on failure.
* \see CMemHandler
* \see CMemHandler::Alloc
* \see RWS_NEW
* \see RWS_NEW_TYPE
* \see RWS_OP_NEW
* \see RWS_OP_NEW_TYPE
*
*/

#ifdef RWS_DISABLE_MEMORY_CHECKING

void *operator new(size_t size, RwUInt32 userTypeNum)
{
   // NOTE: must always call CMemHandler so that initialization of underlying memory system occurs.
   return(CMemHandler::Alloc(size, CMemHandler::MHMT_RWS, rwMEMHINTDUR_GLOBAL, userTypeNum));
}

#else

void *operator new(size_t size, const char *file, int line, RwUInt32 userTypeNum)
{
   // NOTE: must always call CMemHandler so that initialization of underlying memory system occurs.
   return(CMemHandler::Alloc(size, CMemHandler::MHMT_RWS, rwMEMHINTDUR_GLOBAL, file, line, userTypeNum));
}

#endif

/**
*
* \ingroup MemHandler
*
* Redefinition of the global new function (array) so it uses the RenderWare Studio standardized
* memory handler class, CMemHandler. This is the C++ standard new function and should not be called for
* code from within RenderWare Studio.
*
* This version is called as a result of using RWS_NEW and RWS_NEW_TYPE. It provides additional
* information over and above the 'standard new', such as filename, line number and a user type number.
*
* Note memory type is set to MHMT_RWS and the user type number, with RWS_NEW, is set to
* RWS_MEMORY_TYPE_STD. With RWS_NEW_TYPE the user can set this value.
*
* \param size The size, in bytes, of the block to allocate.
* \param file The compiler static filename of where the allocation occurs.
* \param line The compiler line number of where the allocation occurs.
* \param userTypeNum The user defined memory type number passed in from RWS_NEW_TYPE.
* \return a pointer to a memory block or zero (NULL) on failure.
* \see CMemHandler
* \see CMemHandler::Alloc
* \see RWS_NEW
* \see RWS_NEW_TYPE
*
*/

#ifdef RWS_DISABLE_MEMORY_CHECKING

void *operator new[](size_t size, RwUInt32 userTypeNum)
{
   // NOTE: must always call CMemHandler so that initialization of underlying memory system occurs.

   return(CMemHandler::Alloc(size, CMemHandler::MHMT_RWS, rwMEMHINTDUR_GLOBAL, userTypeNum));
}

#else

void *operator new[](size_t size, const char *file, int line, RwUInt32 userTypeNum)
{
   // NOTE: must always call CMemHandler so that initialization of underlying memory system occurs.

   return(CMemHandler::Alloc(size, CMemHandler::MHMT_RWS, rwMEMHINTDUR_GLOBAL, file, line, userTypeNum));
}

#endif

/**
*
* \ingroup MemHandler
*
* This is a dummy delete (non-array) and should not be called. It is required by some compilers (Visual Studio). It
* simply calls the CMemHandlers free function (just in case it is called).
*
* \param pMem The pointer to the block of memory to be freed.
* \see CMemHandler
* \see CMemHandler::Free
* 
*/

#ifdef RWS_DISABLE_MEMORY_CHECKING

void operator delete(void *pMem, RwUInt32 type)
{
   if (pMem)
   {
      CMemHandler::Free(pMem);
   }
}

#else

void operator delete(void *pMem, const char *file, int line, RwUInt32 type)
{
   if (pMem)
   {
      CMemHandler::Free(pMem);
   }
}

#endif

/**
*
* \ingroup MemHandler
*
* This is a dummy delete (array) and should not be called. It is required by some compilers (Visual Studio). It
* simply calls the CMemHandlers free function (just in case it is called).
*
* \param pMem The pointer to the block of memory to be freed.
* \see CMemHandler
* \see CMemHandler::Free
* 
*/

#ifdef RWS_DISABLE_MEMORY_CHECKING

void operator delete[](void *pMem, RwUInt32 type)
{
   if (pMem)
   {
      CMemHandler::Free(pMem);
   }
}

#else

void operator delete[](void *pMem, const char *file, int line, RwUInt32 type)
{
   if (pMem)
   {
      CMemHandler::Free(pMem);
   }
}

#endif

/**
*
* \ingroup MemHandler
*
* Target function (via lookup table) for the RwMalloc function. Can be called directly as a replacement for
* malloc and RwMalloc. However, a much better way is to use the RWS_OP_NEW or RWS_OP_NEW_TYPE macro.
*
* \see RWS_OP_NEW
* \see RWS_OP_NEW_TYPE
* \see CMemHandler
* \see CMemHandler::Alloc
* \see RWSFree
* \see RWSReAlloc
* \see RWSCalloc
* \param size The size of the memory block to allocate.
* \return a pointer to the memory block or zero on failure.
*
*/

void *RWSMalloc(size_t size, RwUInt32 hint)
{
   // NOTE: must always call CMemHandler so that initialization of underlying memory system occurs.

#ifdef RWS_DISABLE_MEMORY_CHECKING

   return(CMemHandler::Alloc(size, CMemHandler::MHMT_RW, rwMEMHINTDUR_GLOBAL, RWS_MEMORY_TYPE_RW));

#else

   return(CMemHandler::Alloc(size, CMemHandler::MHMT_RW, rwMEMHINTDUR_GLOBAL, RwMemoryGetLastAllocFile(), RwMemoryGetLastAllocLine(), RWS_MEMORY_TYPE_RW));

#endif

}

/**
*
* \ingroup MemHandler
*
* Target function (via lookup table) for the RwFree function. Can be called directly as a replacement
* for free and RwFree. However, a much better way is to use the RWS_OP_NEW and RWS_OP_NEW_TYPE macro
* and the standard C++ delete / operator delete.
*
* \see RWS_OP_NEW
* \see RWS_OP_NEW_TYPE
* \see CMemHandler
* \see CMemHandler::Free
* \see RWSMalloc
* \see RWSReAlloc
* \see RWSCalloc
* \param pMem A pointer to the memory to be freed.
*
*/

void RWSFree(void *pMem)
{
   if (pMem)
   {
      CMemHandler::Free(pMem);
   }
}

/**
*
* \ingroup MemHandler
*
* Target function (via lookup table) for the RwRealloc function. Can be called directly as a
* replacement for realloc or RwRealloc. Use of this function is unadvisable due to the fact that
* it is very likely to fragment memory. 
*
* \see RWS_OP_NEW
* \see RWS_OP_NEW_TYPE
* \see CMemHandler
* \see CMemHandler::ReAlloc
* \see RWSMalloc
* \see RWSFree
* \see RWSCalloc
* \param size The size of the memory block to allocate.
* \param pMem A pointer to memory to be extended / freed (depending on new size). Can be zero.
* \return a pointer to the memory block or zero on failure. Note that if zero is returned the
* memory pointed by pMem will still be valid.
*
*/

void *RWSReAlloc(void *pMem, size_t newSize, RwUInt32 hint)
{
   // NOTE: must always call CMemHandler so that initialization of underlying memory system occurs.

#ifdef RWS_DISABLE_MEMORY_CHECKING

   return(CMemHandler::ReAlloc(pMem, newSize, CMemHandler::MHMT_RW, rwMEMHINTDUR_GLOBAL, RWS_MEMORY_TYPE_RW));

#else

   return(CMemHandler::ReAlloc(pMem, newSize, CMemHandler::MHMT_RW, rwMEMHINTDUR_GLOBAL, RwMemoryGetLastAllocFile(), RwMemoryGetLastAllocLine(),
         RWS_MEMORY_TYPE_RW));

#endif

}

/**
*
* \ingroup MemHandler
*
* Target function (via lookup table) for the RwCalloc function. Can be called directly as a
* replacement for calloc or RwCalloc. However, a much better way is to use the RWS_OP_NEW or
* RWS_OP_NEW_TYPE macros & clear the memory separately.
*
* \see RWS_OP_NEW
* \see RWS_OP_NEW_TYPE
* \see CMemHandler
* \see CMemHandler::Calloc
* \see RWSMalloc
* \see RWSFree
* \see RWSReAlloc
* \param size The size of the memory block to allocate.
* \return a pointer to the memory block or zero on failure.
*
*/
void *RWSCalloc(size_t num, size_t size, RwUInt32 hint)
{
   // NOTE: must always call CMemHandler so that initialization of underlying memory system occurs.

#ifdef RWS_DISABLE_MEMORY_CHECKING

   return(CMemHandler::Calloc(num, size, CMemHandler::MHMT_RW, rwMEMHINTDUR_GLOBAL, RWS_MEMORY_TYPE_RW));

#else

   return(CMemHandler::Calloc(num, size, CMemHandler::MHMT_RW, rwMEMHINTDUR_GLOBAL, RwMemoryGetLastAllocFile(), RwMemoryGetLastAllocLine(),
         RWS_MEMORY_TYPE_RW));

#endif

}


#if defined(SN_TARGET_NGC)

/**
*
* \ingroup MemHandler
*
* Custom malloc() function for Gamecube when building with SN Systems ProDG.
* This needed because the Gamecube SDK does not define malloc() and neither does ProDG.
*
* \param size The size of the memory block to allocate.
* \return a pointer to the memory block or zero on failure.
*
*/
void* malloc (size_t blocksize)
{
	return RwsDolphinHeapAlloc( blocksize );
}

/**
*
* \ingroup MemHandler
*
* Custom free() function for Gamecube when building with SN Systems ProDG.
* This needed because the Gamecube SDK does not define free() and neither does ProDG.
*
* \param ptr A pointer to the memory block to free.
* \return a pointer to the memory block or zero on failure.
*
*/
void free (void* ptr)
{
   if ( ptr != NULL )
   {
	   RwsDolphinHeapFree( ptr );
   }
}

#endif

