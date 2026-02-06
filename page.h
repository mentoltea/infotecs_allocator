#ifndef PAGE_H
#define PAGE_H

#include "base.h"
#include <stdbool.h>

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

#endif // PAGE_H