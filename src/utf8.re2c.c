/**********************************************************************************************/
/* The MIT License                                                                            */
/*                                                                                            */
/* Copyright 2016-2017 Twitch Interactive, Inc. or its affiliates. All Rights Reserved.       */
/*                                                                                            */
/* Permission is hereby granted, free of charge, to any person obtaining a copy               */
/* of this software and associated documentation files (the "Software"), to deal              */
/* in the Software without restriction, including without limitation the rights               */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell                  */
/* copies of the Software, and to permit persons to whom the Software is                      */
/* furnished to do so, subject to the following conditions:                                   */
/*                                                                                            */
/* The above copyright notice and this permission notice shall be included in                 */
/* all copies or substantial portions of the Software.                                        */
/*                                                                                            */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR                 */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,                   */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE                */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER                     */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,              */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN                  */
/* THE SOFTWARE.                                                                              */
/**********************************************************************************************/

#include "utf8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*!re2c
    re2c:define:YYCTYPE = utf8_codepoint_t;
    re2c:yyfill:enable = 0;
    re2c:flags:tags = 1;
*/

size_t utf8_codepoint_length(const utf8_codepoint_t* codepoint)
{
    if (!codepoint) {
        return 0;
    }

    const utf8_codepoint_t* YYCURSOR = codepoint;
    /*!re2c
        [\x00\xff] { return 0; }
        [\x01-\x7f] { return 1; }
        [\x80-\xbf] { return 0; } // continuation byte
        [\xc0-\xdf] [\x80-\xbf] {2} { return 2; }
        [\xe0-\xef] [\x80-\xbf] {3} { return 3; }
        [\xf0-\xfe] [\x80-\xbf] {4} { return 4; }
    */
}

const utf8_codepoint_t* utf8_codepoint_next(const utf8_codepoint_t* codepoint)
{
    size_t codepoint_length = utf8_codepoint_length(codepoint);
    return codepoint_length ? codepoint + codepoint_length : 0;
}

// returns number of bytes copied dst must have at leadt 5 bytes to be safe
size_t utf8_codepoint_copy(utf8_codepoint_t* dst, const utf8_codepoint_t* src)
{
    size_t codepoint_length = utf8_codepoint_length(src);

    if (codepoint_length && dst) {
        memcpy(dst, src, codepoint_length);
        dst[codepoint_length] = '\0';
    }

    return codepoint_length;
}

size_t utf8_codepoint_is_whitespace(const utf8_codepoint_t* codepoint)
{
    const utf8_codepoint_t* YYCURSOR = codepoint;
    /*!re2c
    * { return 0; }
    [ \t\r\n] { return 1; }
    */
}

size_t utf8_string_length(const utf8_codepoint_t* str, size_t* bytes)
{
    size_t codepoint_size = 0, codepoint_bytes = 0, codepoint_count = 0;
    do {
        codepoint_size = utf8_codepoint_length(str);
        ++codepoint_count, str += codepoint_size, codepoint_bytes += codepoint_size;
    } while (codepoint_size);

    if (bytes) {
        *bytes = codepoint_bytes;
    }

    return codepoint_count;
}

size_t utf8_string_wrap_length(const utf8_codepoint_t* str, size_t max_codepoints, size_t* bytes)
{
    size_t wrap_bytes = 0;
    size_t wrap_count = 0;
    size_t codepoint_bytes = 0;
    size_t codepoint_count = 0;
    const utf8_codepoint_t* YYCURSOR = str;
    while (codepoint_count < max_codepoints) {
        /*!re2c
        "\x00" { break; }
        [ \s\r\n] { 
            wrap_bytes = codepoint_bytes;
            wrap_count = codepoint_count;
            ++codepoint_bytes, ++codepoint_count;
            continue;
        }
        [\x00-\x7f] { ++codepoint_count; ++codepoint_bytes; continue; }
        [\x80-\xbf] { ++codepoint_bytes; continue; }
        [\xc0-\xdf] [\x80-\xbf] {2} { ++codepoint_count; codepoint_bytes += 2; continue; }
        [\xe0-\xef] [\x80-\xbf] {3} { ++codepoint_count; codepoint_bytes += 3; continue; }
        [\xf0-\xff] [\x80-\xbf] {4} { ++codepoint_count; codepoint_bytes += 4; continue; }
    */
    }

    if (!wrap_count) {
        wrap_bytes = codepoint_bytes;
        wrap_count = codepoint_count;
    }

    if (bytes) {
        *bytes = wrap_bytes;
    }

    return wrap_count;
}

size_t utf8_string_line_length(const utf8_codepoint_t* str, size_t* bytes)
{
    size_t codepoint_bytes = 0;
    size_t codepoint_count = 0;
    const utf8_codepoint_t* YYCURSOR = str;
    for (;;) {
        /*!re2c
        "\x00\xff" { break; }
        "\r" | "\n" { codepoint_bytes += 1, codepoint_count += 1; break; }
        "\r\n" | "\n\r" { codepoint_bytes += 2, codepoint_count += 2; break; }
        [\x00-\x7f] { ++codepoint_count; ++codepoint_bytes; continue; }
        [\x80-\xbf] { ++codepoint_bytes; continue; }
        [\xc0-\xdf] { ++codepoint_count; ++codepoint_bytes; continue; }
        [\xe0-\xef] { ++codepoint_count; ++codepoint_bytes; continue; }
        [\xf0-\xef7] { ++codepoint_count; ++codepoint_bytes; continue; }
    */
    }

    if (bytes) {
        *bytes = codepoint_bytes;
    }

    return codepoint_count;
}

const utf8_codepoint_t* utf8_string_skip_whitespace(const utf8_codepoint_t* str)
{
    const utf8_codepoint_t* YYCURSOR = str;
    for (;;) {
        /*!re2c
        [ \s\r\n]  { str += utf8_codepoint_length(str); continue; }
        * { return str; }
    */
    }
}

size_t utf8_string_line_count(const utf8_codepoint_t* str)
{
    if (!str[0]) {
        return 0;
    }

    const utf8_codepoint_t* YYCURSOR = str;
    for (size_t lines = 0;; ++lines) {
        /*!re2c
        * { return 1 + lines; }
        "\r\n" | "\r" | "\n\r" | "\r" { ++lines; }        
    */
    }

    return 0;
}

utf8_codepoint_t* utf8_string_copy(const utf8_codepoint_t* begin, const utf8_codepoint_t* end)
{
    if (begin == end) {
        return 0;
    }

    size_t size = (end - begin);
    utf8_codepoint_t* new_string = (utf8_codepoint_t*)malloc(1 + size);
    memcpy(new_string, begin, size), new_string[size] = 0;
    return new_string;
}

utf8_codepoint_t* utf8_load_text_file(const char* path, size_t* size)
{
    utf8_codepoint_t* data = NULL;
    FILE* file = fopen(path, "r");

    if (file) {
        fseek(file, 0, SEEK_END);
        size_t file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (0 == (*size) || file_size <= (*size)) {
            (*size) = 0;
            data = (utf8_codepoint_t*)malloc(1 + file_size);
            memset(data, '\0', file_size);

            if (data) {
                utf8_codepoint_t* pos = data;
                size_t bytes_read = 0;

                while (0 < (bytes_read = fread(pos, 1, file_size - (*size), file))) {
                    pos += bytes_read;
                    (*size) += bytes_read;
                }
            }

            fclose(file);
        }
    }

    data[*size] = 0;
    return data;
}