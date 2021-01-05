#if !defined(DST_FILE_H)
#ifdef __cplusplus
extern "C" 
{
#endif

#ifdef DST_FILE_STATIC
#define dst_internal static
#else
#define dst_internal
#endif

#include "dst_int.h"

typedef struct
{
    dstu8 *data;
    dstu64 size;
} dst_File;

#if defined(_WIN32)
// @todo: possibly remove windows.h?
#include <windows.h>

#ifdef DST_FILE_IMPLEMENTATION
// @todo: utf-8 support
dst_internal dst_File dst_read_entire_file(char *path)
{
    dst_File result = {};
    HANDLE file_handle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0,
                                     OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (file_handle != INVALID_HANDLE_VALUE)
    {
        // TODO(dave): Currently only supports up to 4GB files
        dstu32 file_size = GetFileSize(file_handle, 0);
        DWORD bytes_read;

        // TODO(dave): Remove this allocation
        u8 *buffer = (u8 *)VirtualAlloc(0, file_size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
        if(ReadFile(file_handle, buffer, file_size, &bytes_read, 0))
        {
            result.data = buffer;
            result.size = (u32)bytes_read;
        }
        else
        {
            throw_error("Unable to read file: %s\n", path);
        }

        CloseHandle(file_handle);
    }
    else
    {
        throw_error("Unable to open file: %s\n", path);
        DWORD error = GetLastError();
    }

    return result;
}
#endif

#elif defined(__linux__)
#elif defined(__APPLE__)
#endif

#ifdef __cplusplus
}
#endif
#define DST_FILE_H
#endif
