#ifndef ALLOCATOR_H
#define ALLOCATOR_H
 
#include "base.h"
#include "page.h"

#define PAGE_SIZE 4096

#include <string.h> // for memcpy
// macros for dynamic arrays
// from my own project https://github.com/mentoltea/cutils
#define CUTILS_DARRAY_ALLOCATOR crossplatform_alloc
#define CUTILS_DARRAY_DEALLOCATOR crossplatform_free
#define CUTILS_DARRAY_MEMCPY memcpy
#define CUTILS_DARRAY_DEFAULT_SIZE 10
#define CUTILS_DARRAY_DEFAULT_EXPANSION 2.0
#include "darray.h"

void* malloc(size_t size);
void free(void* addr);

// Resets (clears) page system.
// All given pointers will be invalidated after this call. 
void mreset();

#endif // ALLOCATOR_H