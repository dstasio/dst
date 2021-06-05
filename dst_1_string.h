// Version 0.1
#if !defined(DST_1_STRING_H)
#ifdef __cplusplus
extern "C" 
{
#endif

#ifdef DST_STRING_STATIC
#define dstr_func static
#else
#define dstr_func
#endif

#include <stdint.h>

// =====================================================================================
// Declarations
// =====================================================================================
// @todo: move struct definitions to implementation section?
typedef enum
{
    DSTR_ASCII,
    DSTR_UTF8
    //DSTR_UTF16,
    //DSTR_UTF32,
} dstr_Format;

typedef struct
{
    void       *buffer;
    uint64_t    buffer_size;
    uint64_t    length;
    dstr_Format format;
} dstr_String;


// @doc: allocates a new block of memory using the dstr_malloc macro
//       - out_string:   pre-allocated struct that receives the output data
//       - content:      encoded bytes to be decoded and copied into the string
//       - content_size: size in bytes of 'content'
//       - format:       format used to decode 'content';
//                       can be: DSTR_ASCII, DSTR_UTF8   [default: ASCII]
dstr_func void dstr_to_string(dstr_String *out_string, void *content, uint64_t content_size, dstr_Format format = DSTR_ASCII);

//dstr_func void dstr_make_empty_string(dstr_String *out_string, uint64_t length, dstr_Format format = DSTR_ASCII);


dstr_func int dstr_is_numeric(uint32_t chr);


dstr_func uint32_t dstr32_parse_uint(uint32_t *str, uint32_t *out_n_chars_parsed = 0);
dstr_func uint32_t  dstr8_parse_uint(char     *str, uint32_t *out_n_chars_parsed = 0);

dstr_func int32_t  dstr32_parse_int (uint32_t *str, uint32_t *out_n_chars_parsed = 0);
dstr_func int32_t   dstr8_parse_int (char     *str, uint32_t *out_n_chars_parsed = 0);

dstr_func float dstr8_parse_float(char *str, uint32_t *out_n_chars_parsed = 0);

#ifdef DST_STRING_IMPLEMENTATION
// =====================================================================================
// Implementations
// =====================================================================================

dstr_func void dstr_to_string(dstr_String *out_string,
                                  void        *content,
                                  uint64_t     content_size,
                                  dstr_Format  format)
{
    switch(format)
    {
    case DSTR_ASCII:
    {
    } break;

    case DSTR_UTF8:
    {
    } break;
    }
}


dstr_func int dstr_is_numeric(uint32_t chr)
{
    int result = 0;
    if ((chr >= (uint32_t)'0') && (chr <= (uint32_t)'9'))
        result = 1;
    return result;
}

dstr_func uint32_t dstr8_parse_uint(char     *str,
                                        uint32_t *out_n_chars_parsed)
{
    uint32_t result       = 0;
    uint32_t chars_parsed = 0;
    char *at = str;
    while(dstr_is_numeric((uint32_t)*at))
    {
        // @todo: find a more efficient algorithm
        result *= 10;
        result += *at - '0';
        at += 1;
        chars_parsed += 1;
    }

    if (out_n_chars_parsed)
        *out_n_chars_parsed = chars_parsed;
    return result;
}

dstr_func int32_t dstr8_parse_int(char     *str,
                                  uint32_t *out_n_chars_parsed)
{
    int32_t result = 1;
    // @todo: algorithm
    if (*str == '-')
    {
        result = -1;
        str += 1;
    }
    else if (*str == '+')
        str += 1;
    // @note: parse_uint returns 0 if no numeric character was found
    result *= dstr8_parse_uint(str, out_n_chars_parsed);
    return result;
}

dstr_func float dstr8_parse_float(char     *str,
                                  uint32_t *out_n_chars_parsed)
{
    uint32_t chars_parsed = 0;

    float result   = 0.f;
    float sign     = 1.f;
    float fraction = 0.f;

    // @todo: algorithm
    if (*str == '-')
    {
        sign = -1.f;
        str += 1;
    }
    else if (*str == '+')
        str += 1;

    while(dstr_is_numeric((uint32_t)*str) ||
          *str == '.')
    {
        if (*str == '.')
        {
            // @todo: error reporting
            if(fraction)
                break;
            fraction = 1.f;
        }
        else
        {
            result *= 10;
            result += *str - '0';
            if (fraction)
                fraction *= 10.f;
        }
        chars_parsed += 1;
        str += 1;
    }

    if (fraction)
        result /= sign*fraction;
    if (out_n_chars_parsed)
        *out_n_chars_parsed = chars_parsed;
    return result;
}

#endif // DST_STRING_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#define DST_1_STRING_H
#endif
