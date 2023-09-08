/***************************************************************************
 *                                                                         *
 * Module  : dlmem.c                                                       *
 *                                                                         *
 * Purpose : Dolphin memory heap manager                                   *
 *                                                                         *
 **************************************************************************/

/****************************************************************************
 Includes
 */

#include <string.h>

#include "rwcore.h"
#include "assert.h"

#include "dolphin.h"

#include "dlmem.h"

/****************************************************************************
 Global prototypes
 */

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

OSHeapHandle    TheHeap;

/****************************************************************************
 Functions
 */

RwBool RwsDolphinHeapInit(void)
{
    void*   arenaLo = NULL;
    void*   arenaHi = NULL;

    arenaLo = OSGetArenaLo();
    arenaHi = OSGetArenaHi();

    /* OSInitAlloc should only ever be invoked once. */
    arenaLo = OSInitAlloc(arenaLo, arenaHi, 1); /* 1 heap */
    OSSetArenaLo(arenaLo);

    /* The boundaries given to OSCreateHeap should be 32B aligned */
    TheHeap = OSCreateHeap((void*)OSRoundUp32B(arenaLo),
                           (void*)OSRoundDown32B(arenaHi));

    /* TheHeap - this always seems to be 0, even in the sample code */

    OSSetCurrentHeap(TheHeap);

    /* From here on out, OSAlloc and OSFree behave like malloc and free
     * respectively
     */

    OSSetArenaLo(arenaLo = arenaHi);

    return (TRUE);
}

void RwsDolphinHeapFree(void *memory)
{
    /* realloc can filter NULL pointers down to here */
    //assert(OSCheckHeap(TheHeap) >= 0);
    if (memory)
    {
        /* Free memory that starts with size field */
        OSFree(((RwUInt8 *)memory) - 32);
    }
    //assert(OSCheckHeap(TheHeap) >= 0);
}

void *RwsDolphinHeapAlloc(size_t size)
{
    RwUInt32    *memory;
    
    if (size < 0)
    {
        return (NULL);
    }

   //assert(OSCheckHeap(TheHeap) >= 0);
    /* Need to keep track of size of realloc */
    memory = (RwUInt32 *)OSAlloc(size + 32);
    //assert(OSCheckHeap(TheHeap) >= 0);

    if (memory)
    {
       /* Store the size so we can do a realloc later */
       *memory = size;
        ((RwUInt8 *)memory) += 32;
    }

    return ((void *)memory);
}

void *RwsDolphinHeapCalloc(size_t num, size_t size)
{
    void *mem;

    mem = RwsDolphinHeapAlloc(num*size);
    if (mem)
    {
        memset(mem, 0, num*size);
    }

    return (mem);
}

void *RwsDolphinHeapRealloc(void *memory, size_t size)
{
    /* We have to do a real dumb thing here, grab the size from the old block!!! */
    RwUInt32  oldSize;
    RwUInt32 *newMem;

   if (memory)
   {
      oldSize = *(RwUInt32 *)(((RwUInt8 *)memory) -32);
   }
   else
   {
      oldSize = 0;
   }

    /* Allocate new memory */
    newMem = (RwUInt32 *)RwsDolphinHeapAlloc(size);
    if (newMem)
    {
       if (oldSize < size)
       {
           /* Expanding the block */
           memcpy(newMem, memory, oldSize);
       }
       else
       {
           /* Shrinking the block */
           memcpy(newMem, memory, size);
       }

       /* Free the old block */
       RwsDolphinHeapFree(memory);
    }

    return (newMem);
}

RwBool RwsDolphinTermMemorySystem(void)
{
    return TRUE;
}
