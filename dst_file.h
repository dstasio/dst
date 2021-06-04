// version 0.1
// 
// Notes: Currently only works for windows; to be ported.
//
// TODO: Better error management (was the file not read because it doesn't exist or because it's protected? etc.)

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

#include <stdint.h>
//typedef uint8_t  dstu8;
//typedef uint16_t dstu16;
//typedef uint32_t dstu32;
//typedef uint64_t dstu64;
//typedef int8_t  dsts8;
//typedef int16_t dsts16;
//typedef int32_t dsts32;
//typedef int64_t dsts64;


//typedef struct
//{
//    dstu8 *data;
//    dstu64 size;
//} dst_File;

#if defined(_WIN32)
// @todo: possibly remove windows.h?
#include <windows.h>

// *********************************************************************************
// Declarations
// *********************************************************************************

// @doc: Reads the file at 'path' and returns its content and its size.
//       If the file could not be read, 'data' and 'size' will return 0.
dst_internal void dst_read_entire_file(char *path, uint8_t **data, uint64_t *size);


#ifdef DST_FILE_IMPLEMENTATION
// *********************************************************************************
// Implementations
// *********************************************************************************


// @todo: utf-8 support
dst_internal void dst_read_entire_file(char *path, uint8_t **out_data, uint64_t *out_size)
{
    HANDLE file_handle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0,
                                     OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (file_handle != INVALID_HANDLE_VALUE)
    {
        // @todo: Currently only supports up to 4GB files
        uint32_t file_size = GetFileSize(file_handle, 0);
        DWORD bytes_read;

        // @todo: Remove this allocation
        *out_data = (uint8_t *)VirtualAlloc(0, file_size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
        if(ReadFile(file_handle, &out_data, file_size, &bytes_read, 0))
        {
            *out_size = (uint32_t)bytes_read;
        }
        else
        {
            *out_data = 0;
            *out_size = 0;
            //throw_error("Unable to read file: %s\n", path);
        }

        CloseHandle(file_handle);
    }
    else
    {
        *out_data = 0;
        *out_size = 0;
        //throw_error("Unable to open file: %s\n", path);
        DWORD error = GetLastError();
    }
}
#endif // DST_FILE_IMPLEMENTATION

#elif defined(__linux__)
TEST_LINUX;
#elif defined(__APPLE__)
TEST_MACOS;
#endif

#ifdef __cplusplus
}
#endif
#define DST_FILE_H
#endif
