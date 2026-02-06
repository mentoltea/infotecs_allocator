#include "allocator.h"

typedef struct Pages {
    Page* items;
    size_t count;
    size_t capacity;
} Pages;

static Pages pages = {0};

void* malloc(size_t size) {
    size_t fullsize = size + sizeof(ElementMetainfo);
    Page* min_page = NULL;
    size_t min_size = SIZE_MAX;
    for (int idx = 0; idx < pages.count; idx++) {
        Page *page = &pages.items[idx];
        if (page->available_max >= fullsize && page->available_max < min_size) {
            min_page = &pages.items[idx];
            min_size = pages.items[idx].available_max;
            if (page->available_max == fullsize) break;
        }
    }
    if (min_page == NULL) {
        size_t page_size = PAGE_SIZE;
        while (size > page_size) page_size = page_size << 1;
        if (page_init(min_page, page_size) == false) return NULL;
        da_append(pages, *min_page);
    }
    return page_alloc(min_page, size);
}

void free(void* addr) {
    for (int idx = 0; idx < pages.count; idx++) {
        Page *page = &pages.items[idx]; 
        if (addr >= page->start && addr < page->start + page->size) {
            page_free(page, addr);
            return;
        }
    }
    // addres not in current pages (ignore?)
}

void mreset() {
    for (int idx = 0; idx < pages.count; idx++) {
        Page *page = &pages.items[idx];
        page_destroy(page);
    }
    crossplatform_free(pages.items);
    pages.count = 0;
}