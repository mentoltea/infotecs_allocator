#include "page.h"
#include <stdio.h>
#include <assert.h>

int main() {
    Page page;
    size_t size = 1024;
    assert(page_init(&page, size) == true);
    printf("page: %p - %p\n", page.start, page.start+page.size);
    printf("page.available_max: %lld\n\n", page.available_max);

    void* ptr1 = page_alloc(&page, 400);
    printf("ptr1: %p\n", ptr1);
    printf("page.available_max: %lld\n\n", page.available_max);

    void* ptr2 = page_alloc(&page, 400);
    printf("ptr2: %p\n", ptr2);
    printf("page.available_max: %lld\n\n", page.available_max);
    
    page_free(&page, ptr1);
    printf("free ptr1: %p\n", ptr1);
    printf("page.available_max: %lld\n\n", page.available_max);
    
    void* ptr3 = page_alloc(&page, 200);
    printf("ptr3: %p\n", ptr3);
    printf("page.available_max: %lld\n\n", page.available_max);

    void* ptr4 = page_alloc(&page, 10);
    printf("ptr4: %p\n", ptr4);
    printf("page.available_max: %lld\n\n", page.available_max);

    void* ptr5 = page_alloc(&page, 100);
    printf("ptr5: %p\n", ptr5);
    printf("page.available_max: %lld\n\n", page.available_max);

    page_destroy(&page);
    return 0;
}