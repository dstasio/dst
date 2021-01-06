// version 0.1

#if !defined(DST_UNICODE_H)
#ifdef __cplusplus
extern "C" 
{
#endif

#define DSTUTF_NOT_FOUND 0xFFFFFFFFFFFFFFFF

#ifdef DST_FILE_STATIC
#define dstutf_internal static
#else
#define dstutf_internal
#endif

// @todo: to be removed
#include "dst_int.h"

typedef dstu32 dstutf_Codepoint;

// *********************************************************************************
// Declarations
// *********************************************************************************


typedef struct
{
    dstu64  buffer_size;
    dstu64  length;
    dstu8  *bytes;
} dstutf_String;

// @doc: string is assumed to be null-terminated
dstutf_internal dstutf_String  dstutf_make_string(char *data);

// @doc: size pointer returns string size in bytes, can be null.
dstutf_internal dstu64         dstutf_length(dstu8 *data, dstu64 *out_size);

// @doc: returns index of the first appearance of 'token' in 'str', or DSTUTF_NOT_FOUND
dstutf_internal dstu64         dstutf_find(dstutf_String str, dstutf_String token);


dstu8 dstutf8__char_length_table[16] = 
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

//dstutf_build_string(char *str)
//{
//}

dstutf_internal dstutf_String
dstutf_make_string(char *data)
{
    dstutf_String result = {};
    result.bytes = (dstu8 *)data;
    result.length = dstutf_length(result.bytes, &result.buffer_size);
    return result;
}

// @todo: support known-size buffers
dstutf_internal dstu64
dstutf_length(dstu8 *data, dstu64 *out_size = 0)
{
    dstu64 length = 0;
    dstu8 *at = data;

    dstu64 total_size = 0;
    while (*at)
    {
        dstu8 char_size = dstutf8__char_length_table[(*at) >> 4];

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

// @doc: reads codepoint and advances pointer to the next position
dstutf_internal dstutf_Codepoint
dstutf__eat_codepoint(dstu8 **s)
{
    dstutf_Codepoint result = 0;
    dstu8 char_size = dstutf8__char_length_table[(**s) >> 4];

    // @todo: look at disassembly
    dstu8 bitmask_length = 8 - (char_size + !!(char_size-1));
    dstu8 bitmask = ~(0xFF << bitmask_length);

    result = **s & bitmask;

    bitmask = 0b111111;
    char_size -= 1;  *s += 1;
    while (char_size > 0)
    {
        result <<= 6;
        result |= **s & bitmask;

        char_size -= 1;  *s += 1;
    }

    return result;
}

dstutf_internal dstu64
dstutf_find(dstutf_String str, dstutf_String token)
{
    dstu8 *source_at = str.bytes;
    dstu8 *token_at = token.bytes;
    dstu8 *token_end = token.bytes + token.buffer_size;
    dstutf_Codepoint token_codepoint = dstutf__eat_codepoint(&token_at);

    dstu8 read = 0;
    dstu64 match_index = DSTUTF_NOT_FOUND;
    while (read < str.length)
    {
        dstutf_Codepoint str_codepoint = dstutf__eat_codepoint(&source_at);

        if (str_codepoint == token_codepoint)
        {
            if (match_index == DSTUTF_NOT_FOUND)
                match_index = read;

            if (token_at == token_end)
                break;
            token_codepoint = dstutf__eat_codepoint(&token_at);
        }
        else if (match_index != DSTUTF_NOT_FOUND)
        {
            token_at = token.bytes;
            token_codepoint = dstutf__eat_codepoint(&token_at);
        }

        read += 1;
    }

    return match_index;
}

#endif // DST_FILE_IMPLEMENTATION

#ifdef __cplusplus
}
#endif
#define DST_UNICODE_H
#endif
