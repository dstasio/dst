#if !defined(DST_MEMORY_H)
#ifdef __cplusplus
extern "C" 
{
#endif

#ifdef DST_MEMORY_STATIC
#define dstmem_internal static
#else
#define dstmem_internal
#endif

#include <stdint.h>

// =====================================================================================
// Declarations
// =====================================================================================
dstmem_internal void *dst_malloc(uint64_t size);
dstmem_internal void  dst_free(void *memory);


#ifdef DST_MEMORY_IMPLEMENTATION
// =====================================================================================
// Implementations
// =====================================================================================
#if defined(_WIN32)
// @todo: possibly remove windows.h?
#include <windows.h>

dstmem_internal void *dst_malloc(uint64_t size)
{
    void *memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (!memory)
    {
        // @todo: error management
    }
    return memory;
}

dstmem_internal void dst_free(void *memory)
{
    VirtualFree(memory, 0, MEM_RELEASE);
}

#elif defined(__linux__)
TEST_LINUX;
#elif defined(__APPLE__)
TEST_MACOS;
#endif // _WIN32/__linux__/__APPLE__

#endif // DST_MEMORY_IMPLEMENTATION

#ifdef __cplusplus
}
#endif
#define DST_MEMORY_H
#endif
