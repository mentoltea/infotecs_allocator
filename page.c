#include "page.h"

bool page_init(Page* page, size_t size) {
    void* data = crossplatform_alloc(size);
    if (!data) return false;

    page->start = data;
    page->size = size;

    page->first = NULL;
    page->last = NULL;

    page->available_total = size;
    page->available_max = size;

    return true;
}

void page_destroy(Page* page) {
    crossplatform_free(page->start);
    page->start = NULL;   
}

void* page_alloc(Page* page, size_t size) {
    size_t fullsize = size + sizeof(ElementMetainfo);
    if (page->available_max < fullsize) return NULL;

    if (page->first == NULL) {
        // first allocation on the page
        ElementMetainfo* meta = page->start;
        void* data = (meta + 1);
        meta->fullsize = fullsize;
        meta->next = NULL;

        page->first = meta;
        page->last = meta;
        page->available_total -= fullsize;
        page->available_max -= fullsize;

        return data;
    }

    void* prev_start = NULL;
    void* prev_end = page->start;
    void* next_start = page->first;

    ElementMetainfo* element_before_min = NULL;
    ElementMetainfo* element_after_min = NULL;
    void* min_free_room_start = NULL;
    size_t min_free_room_size = SIZE_MAX;

    // tactic - use smallest free room
    while (1) {
        size_t free_room_size;
        if (next_start == NULL) free_room_size = (size_t)(page->start + page->size - prev_end);
        else free_room_size = (size_t)(next_start - prev_end);
        
        if (free_room_size >= fullsize && free_room_size < min_free_room_size) {
            min_free_room_start = prev_end;
            min_free_room_size = free_room_size;

            element_before_min = prev_start;
            element_after_min = next_start;
        }

        if (next_start == NULL) break;
        prev_start = next_start;
        prev_end = next_start + ((ElementMetainfo*)next_start)->fullsize;
        next_start = ((ElementMetainfo*)next_start)->next;
    }

    // in theory it cannot happen as we know that available_max >= fullsize
    // therefore there must be at least one available room
    // but na vsyakiy sluchay leave it here
    if (!min_free_room_start) return NULL;

    ElementMetainfo* meta = min_free_room_start;
    void* data = (meta + 1);
    meta->fullsize = fullsize;

    if (element_before_min) element_before_min->next = meta;
    meta->next = element_after_min;
    
    if (page->first == element_after_min) page->first = meta;
    if (page->last == element_before_min) page->last = meta;
    
    page->available_total -= fullsize;

    if (min_free_room_size == page->available_max) {
        // recount available max
        if (page->available_total == 0) page->available_max = 0;
        else page->available_max = page_available_max(page);
    }
    return data;
}

size_t page_available_max(Page* page) {
    void* prev_end = page->start;
    void* next_start = page->first;
    size_t max_size = 0;
    while (1) {
        size_t free_room_size;
        if (next_start == NULL) free_room_size = (size_t)(page->start + page->size - prev_end);
        else free_room_size = (size_t)(next_start - prev_end);
        
        if (free_room_size > max_size) max_size = free_room_size;

        if (next_start == NULL) break;
        prev_end = next_start + ((ElementMetainfo*)next_start)->fullsize;
        next_start = ((ElementMetainfo*)next_start)->next;
    }
    return max_size;
}

void page_free(Page* page, void* addr) {
    ElementMetainfo* meta = ((ElementMetainfo*)addr - 1);

    ElementMetainfo* before = NULL;
    ElementMetainfo* after = meta->next;

    ElementMetainfo* prev_start = NULL;
    ElementMetainfo* next_start = page->first;
    while (1) {
        if (next_start == meta) {
            before = prev_start;
            break;
        }

        if (next_start == NULL) return; // not found (ignore?)
        
        prev_start = next_start;
        next_start = ((ElementMetainfo*)next_start)->next;
    }

    if (before) before->next = after;

    if (page->first == meta) page->first = after;
    if (page->last == meta) page->last = before;

    page->available_total += meta->fullsize;
    page->available_max = page_available_max(page);
}