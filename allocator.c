#include "allocator.h"

typedef struct Pages {
    Page* items;
    size_t count;
    size_t capacity;
} Pages;

static Pages pages = {0};
static pthread_mutex_t pages_mutex = PTHREAD_MUTEX_INITIALIZER;

void* malloc(size_t size) {
    pthread_mutex_lock(&pages_mutex);

    size_t fullsize = size + sizeof(ElementMetainfo);
    Page* min_page = NULL;
    size_t min_size = SIZE_MAX;
    for (size_t idx = 0; idx < pages.count; idx++) {
        Page *page = &pages.items[idx];
        if (page->available_max >= fullsize && page->available_max < min_size) {
            min_page = &pages.items[idx];
            min_size = pages.items[idx].available_max;
            if (page->available_max == fullsize) break;
        }
    }
    if (min_page == NULL) {
        Page new_page = {0};
        size_t page_size = PAGE_SIZE;
        while (size > page_size) page_size = page_size << 1;
        if (page_init(&new_page, page_size) == false) return NULL;
        da_append(pages, new_page);
        min_page = &pages.items[pages.count-1];
    }
    
    void* data = page_alloc(min_page, size);

    pthread_mutex_unlock(&pages_mutex);

    return data;
}

void free(void* addr) {
    pthread_mutex_lock(&pages_mutex);
    for (size_t idx = 0; idx < pages.count; idx++) {
        Page *page = &pages.items[idx]; 
        if (addr >= page->start && addr < page->start + page->size) {
            page_free(page, addr);
            pthread_mutex_unlock(&pages_mutex);
            return;
        }
    }
    pthread_mutex_unlock(&pages_mutex);
    // addres not in current pages (ignore?)
}

void mreset() {
    pthread_mutex_lock(&pages_mutex);

    for (size_t idx = 0; idx < pages.count; idx++) {
        Page *page = &pages.items[idx];
        page_destroy(page);
    }
    crossplatform_free(pages.items);
    pages.count = 0;

    pthread_mutex_unlock(&pages_mutex);
}