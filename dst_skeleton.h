// Version 0.1
#if !defined(DST_SUBST_H)
#ifdef __cplusplus
extern "C" 
{
#endif

#ifdef DST_SUBST_STATIC
#define dstsubst_internal static
#else
#define dstsubst_internal
#endif

#include <stdint.h>

// =====================================================================================
// Declarations
// =====================================================================================

#ifdef DST_SUBST_IMPLEMENTATION
// =====================================================================================
// Implementations
// =====================================================================================

#if defined(_WIN32)
_WIN32
#elif defined(__linux__)
_LINUX
#elif defined(__APPLE__)
_MACOS
#endif // _WIN32/__linux__/__APPLE__

#endif // DST_SUBST_IMPLEMENTATION

#ifdef __cplusplus
}
#endif
#define DST_SUBST_H
#endif
