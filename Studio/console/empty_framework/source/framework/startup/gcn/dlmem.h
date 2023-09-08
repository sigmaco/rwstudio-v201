/***************************************************************************
 *                                                                         *
 * Module  : dlmem.h                                                       *
 *                                                                         *
 * Purpose : Dolphin memory heap manager                                   *
 *                                                                         *
 **************************************************************************/

#ifndef DLMEM_H
#define DLMEM_H

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

RwBool RwsDolphinHeapInit(void);
void RwsDolphinHeapFree(void *memory);
void *RwsDolphinHeapAlloc(size_t size);
void *RwsDolphinHeapCalloc(size_t num, size_t size);
void *RwsDolphinHeapRealloc(void *memory, size_t size);
RwBool RwsDolphinTermMemorySystem(void);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* DLMEM_H */
