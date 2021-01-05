// version 0.1

#if !defined(DST_UNICODE_H)
#ifdef __cplusplus
extern "C" 
{
#endif

#ifdef DST_FILE_STATIC
#define dst_internal static
#else
#define dst_internal
#endif

// @todo: to be removed
#include "dst_int.h"

// *********************************************************************************
// Declarations
// *********************************************************************************


typedef struct
{
    dstu64  buffer_size;
    dstu64  length;
    dstu8  *bytes;
} dst_String;

dst_internal dst_String  dst_string_from_buffer(dstu8 *data, dstu64 size);
dst_internal dstu64      dst_utf8_length(dstu8 *data, dstu64 *out_size);

dstu8 utf8_char_length_table[16] = 
{
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    2, 2, 3, 4,
};

#ifdef DST_FILE_IMPLEMENTATION
// *********************************************************************************
// Implementations
// *********************************************************************************

//dst_build_string(char *str)
//{
//}

// @doc: if size is 2^64 bytes, string is assumed to be null-terminated
dst_internal dst_String dst_string_from_buffer(dstu8 *data, dstu64 size = 0)
{
    dst_String result = {};
    if (size > 0)
        result.buffer_size = size;
    result.bytes = data;

    result.length = dst_utf8_length(data, &result.buffer_size);

    return result;
}

// @todo: support known-size buffers
// @doc: size pointer returns string size in bytes, can be null.
dst_internal dstu64 dst_utf8_length(dstu8 *data, dstu64 *out_size = 0)
{
    dstu64 length = 0;
    dstu8 *at = data;

    dstu64 total_size = 0;
    while (*at)
    {
        dstu8 char_size = utf8_char_length_table[(*at) >> 4];

        length += 1;
        total_size += char_size;
        at += char_size;
    }

    if (out_size)
    {
        *out_size = total_size;
    }
    return length;
}

#endif // DST_FILE_IMPLEMENTATION

#ifdef __cplusplus
}
#endif
#define DST_UNICODE_H
#endif
