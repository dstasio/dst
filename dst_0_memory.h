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
typedef uint8_t  dstu8;
typedef uint16_t dstu16;
typedef uint32_t dstu32;
typedef uint64_t dstu64;
typedef int8_t  dsts8;
typedef int16_t dsts16;
typedef int32_t dsts32;
typedef int64_t dsts64;


// =====================================================================================
// Declarations
// =====================================================================================
dstmem_internal void *dst_malloc(dstu64 size);
dstmem_internal void  dst_free(void *memory);


// =====================================================================================
// Implementations
// =====================================================================================
#if defined(_WIN32)
// @todo: possibly remove windows.h?
#include <windows.h>

dstmem_internal void *dst_malloc(dstu64 size)
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
#endif

#ifdef __cplusplus
}
#endif
#define DST_MEMORY_H
#endif
