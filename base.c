#include "base.h"

void* crossplatform_alloc(size_t size) {
    void* result = NULL;
#ifdef _WIN32 // Windows
    result = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
#else // Unix
    size_t alloc_size = size + sizeof(size_t);
    result = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (result == MAP_FAILED) result = NULL;
    else {
        *(size_t*)result = alloc_size;
        result = ((size_t*)result + 1);
        // Needs to remember allocated size bcs munmap requires it
    }
#endif
    return result;
}

void crossplatform_free(void* addr) {
#ifdef _WIN32 // Windows
    VirtualFree(addr, 0, MEM_RELEASE);
#else // Unix
    size_t alloc_size = *((size_t*)addr - 1);
    munmap(addr, alloc_size);
#endif
}