#ifndef BASE_H
#define BASE_H

#include <stdint.h>

// #if 0
#ifdef _WIN32 // Windows

#include <memoryapi.h>

#else // Unix/Posix (presumably)

#include <unistd.h>
#include <sys/mman.h>

#endif

// @note
// read & write & execute permission. 
void* crossplatform_alloc(size_t size);

void crossplatform_free(void* addr);

#endif // BASE_H