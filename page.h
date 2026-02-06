#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "base.h"
#include <string.h> // for memcpy
#include <stdbool.h>

// macros for dynamic arrays
// from my own project https://github.com/mentoltea/cutils
#define CUTILS_DARRAY_ALLOCATOR crossplatform_alloc
#define CUTILS_DARRAY_DEALLOCATOR crossplatform_free
#define CUTILS_DARRAY_MEMCPY memcpy
#define CUTILS_DARRAY_DEFAULT_SIZE 10
#define CUTILS_DARRAY_DEFAULT_EXPANSION 2.0
#include "darray.h"

typedef struct ElementMetainfo {
    struct ElementMetainfo *next;
    size_t fullsize; // includes metainfo size
} ElementMetainfo;

typedef struct Page {
    void* start;
    size_t size;

    ElementMetainfo* first;
    ElementMetainfo* last;

    size_t available_total;
    // maximum available with single allocation
    size_t available_max;
} Page;


// @returns true - success. false - error.
bool page_init(Page* page, size_t size);
void page_destroy(Page* page);

void* page_alloc(Page* page, size_t size);
size_t page_available_max(Page* page);
void page_free(Page* page, void* addr);

#endif // ALLOCATOR_H