#ifndef RWUTIL_ALLOC_H
#define RWUTIL_ALLOC_H

#include "RwUtil.h"

/**
*   This memory allocation function is used for all allocations made by the
*   RenderWare specific containers and creation of RwCommObject (and derived)
*   objects. It ensures that wherever these containers and objects are they
*   they always use a consistent memory allocator.
*
*   \ingroup rwutilalloc
*
*   \param size in bytes to allocate
*
*   \return a void pointer to the allocated memory
*/
RWUTIL_API void *RwUtilMalloc(size_t size);

/**
*   This memory allocation function is used for all deallocations made by the
*   RenderWare specific containers and deletion of RwCommObject (and derived)
*   objects. It ensures that wherever these containers and objects are they
*   they always use a consistent memory allocator.
*
*   \ingroup rwutilalloc
*
*   \param p a pointer to the memory to free
*
*/
RWUTIL_API void RwUtilFree(void *p);


#endif // RWUTIL_ALLOC_H
