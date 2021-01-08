// version 0.1b

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

#include <stdint.h>
typedef uint8_t  dstu8;
typedef uint16_t dstu16;
typedef uint32_t dstu32;
typedef uint64_t dstu64;
typedef int8_t  dsts8;
typedef int16_t dsts16;
typedef int32_t dsts32;
typedef int64_t dsts64;

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
dstutf_internal dstutf_String       dstutf_make_string(char *data);
// @doc: size pointer returns string size in bytes, can be null.
dstutf_internal dstu64              dstutf_length(dstu8 *data, dstu64 *out_size);
// @doc: returns index of the first appearance of 'token' in 'str', or DSTUTF_NOT_FOUND
dstutf_internal dstu64              dstutf_find(dstutf_String str, dstutf_String token, dstu64 start_index);
// @doc: reads codepoint and advances pointer to the next position
dstutf_internal dstutf_Codepoint    dstutf__eat_codepoint(dstu8 **s);

// @doc: the following decode, respectively, the utf8/utf16 character pointed to by the input pointer;
//       the number of 'units' of the encoded character is returned by 'out_char_len' if not null
dstutf_internal dstu32 dstutf_decode_utf8 (dstu8  *utf, dstu32 *out_char_len);
dstutf_internal dstu32 dstutf_decode_utf16(dstu16 *utf, dstu32 *out_char_len);

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

dstutf_internal dstu32
dstutf_decode_utf8(dstu8 *utf, dstu32 *out_char_len = 0)
{
    dstutf_Codepoint result = 0;

    dstu8 char_size = dstutf8__char_length_table[(*utf) >> 4];
    if (out_char_len)
        *out_char_len = char_size;

    // @todo: look at disassembly
    dstu8 bitmask_length = 8 - (char_size + !!(char_size-1));
    dstu8 bitmask = ~(0xFF << bitmask_length);

    result = *utf & bitmask;

    bitmask = 0b111111;
    char_size -= 1;  utf += 1;
    while (char_size > 0)
    {
        result <<= 6;
        result |= *utf & bitmask;

        char_size -= 1;  utf += 1;
    }
    return result;
}

// @todo: @important: manage different endianness
dstutf_internal dstu32
dstutf_decode_utf16(dstu16 *utf, dstu32 *out_char_len = 0)
{
    dstutf_Codepoint result = 0;

    // @todo: error reporting; assert that the character has values in ranges: U+0000-U+D7FF or U+E000-U+FFFF
    dstu32 char_size = (*utf > 0x10000) ? 2 : 1;
    if (out_char_len)
        *out_char_len = char_size;

    if (char_size == 1)
    {
        result = *utf;
    }
    else
    {
        result = 0x10000;
        result += (*utf - 0xD800) * 0x400;
        utf += 1;
        result += *utf - 0xDC00;
    }

    return result;
}

dstutf_internal dstutf_Codepoint
dstutf__eat_codepoint(dstu8 **s)
{
    dstutf_Codepoint result = 0;

    dstu32 char_size = 0;
    dstutf_decode_utf8(*s, &char_size);
    s += char_size;

    return result;
}

dstutf_internal dstu64
dstutf_find(dstutf_String str, dstutf_String token, dstu64 start_index = 0)
{
    dstu64 index = start_index;
    dstu8 *source_at = str.bytes + index;
    dstu8 *token_at = token.bytes;
    dstu8 *token_end = token.bytes + token.buffer_size;
    dstutf_Codepoint token_codepoint = dstutf__eat_codepoint(&token_at);

    dstu64 match_index = DSTUTF_NOT_FOUND;
    while (index < str.length)
    {
        dstutf_Codepoint str_codepoint = dstutf__eat_codepoint(&source_at);

        if (str_codepoint == token_codepoint)
        {
            if (match_index == DSTUTF_NOT_FOUND)
                match_index = index;

            if (token_at == token_end)
                break;
            token_codepoint = dstutf__eat_codepoint(&token_at);
        }
        else if (match_index != DSTUTF_NOT_FOUND)
        {
            token_at = token.bytes;
            token_codepoint = dstutf__eat_codepoint(&token_at);
        }

        index += 1;
    }

    return match_index;
}

#endif // DST_FILE_IMPLEMENTATION

#ifdef __cplusplus
}
#endif
#define DST_UNICODE_H
#endif
