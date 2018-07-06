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

const utf8_codepoint_t* utf8_codepoint_next(const utf8_codepoint_t* codepoint)
{
    size_t codepoint_length = utf8_codepoint_length(codepoint);
    return codepoint_length ? codepoint + codepoint_length : 0;
}

size_t utf8_codepoint_length(const utf8_codepoint_t* codepoint)
{
    // count null term as zero size
    if (!codepoint || !(*codepoint)) {
        return 0;
    }

    // 0xxxx, 1 byte
    // 10xxx, continuation byte
    // 110xx, 2 bytes
    // 1110x, 3 bytes
    // 11110, 4 bytes
    // 11111, illegal
    static const size_t _utf8_codepoint_length[] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 4, ((size_t)-1)
    };

    size_t codepoint_length = _utf8_codepoint_length[(codepoint[0] >> 3) & 0x1F];

    // Treat illegal charcters as terminators
    if (((size_t)-1) == codepoint_length) {
        return 0;
    }

    // If we get a continuation byte, walk forward until we find the next codepoint.
    size_t continuation = codepoint_length;
    while (0 == continuation && 0 != codepoint[0] && 4 > codepoint_length) {
        ++codepoint_length, ++codepoint;
        continuation = _utf8_codepoint_length[(codepoint[0] >> 3) & 0x1F];
    }

    return codepoint_length;
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

size_t utf8_codepoint_is_whitespace(const utf8_codepoint_t* c)
{
    // 0x7F is DEL
    if (!c || (c[0] >= 0 && c[0] <= ' ') || c[0] == 0x7F) {
        return 1;
    }

    // EIA608_CHAR_NO_BREAK_SPACE // TODO should we count NBSP? Def not in word wrap
    if (0xC2 == (unsigned char)c[0] && 0xA0 == (unsigned char)c[1]) {
        return 2;
    }

    return 0;
}

size_t utf8_codepoint_is_newline(const utf8_codepoint_t* codepoint)
{
    if ('\r' == codepoint[0]) {
        return '\n' == codepoint[1] ? 2 : 1; // windows/unix
    } else if ('\n' == codepoint[0]) {
        return '\r' == codepoint[1] ? 2 : 1; // riscos/macos
    } else {
        return 0;
    }
}

size_t utf8_string_length(const utf8_codepoint_t* str, size_t* bytes)
{
    size_t _bytes;
    if (!bytes) {
        bytes = &_bytes;
    }

    for (size_t codepoints = 0, (*bytes) = 0;; ++codepoints) {
        size_t codepoint_length = utf8_codepoint_length(str);

        if (!codepoint_length) {
            return codepoints;
        }

        str += codepoint_length, (*bytes) += codepoint_length;
    }
}

size_t utf8_string_line_length(const utf8_codepoint_t* str, size_t* bytes)
{
    size_t str_bytes = 0;
    for (size_t codepoints = 0;; ++codepoints) {
        size_t codepoint_length = utf8_codepoint_length(str);
        size_t newline_length = utf8_codepoint_is_newline(str);

        if (!codepoint_length || newline_length) {
            if (bytes) {
                *bytes = str_bytes;
            }
            return codepoints + newline_length;
        }

        str += codepoint_length;
        str_bytes += codepoint_length;
    }
}

size_t utf8_string_wrap_length(const utf8_codepoint_t* str, size_t max_codepoints, size_t* bytes)
{
    size_t _bytes;
    if (!bytes) {
        bytes = &_bytes;
    }

    for (size_t codepoints = 0, (*bytes) = 0;; ++codepoints) {
        size_t newline_length = utf8_codepoint_is_newline(str);

        if (newline_length || codepoints >= max_codepoints) {
            return codepoints;
        }

        size_t codepoint_length = utf8_codepoint_length(str);
        if (!codepoint_length || 0 < utf8_codepoint_is_newline(str)) {
            return codepoints;
        }

        str += codepoint_length, (*bytes) += codepoint_length;
    }
}

utf8_codepoint_t* utf8_string_skip_whitespace(const utf8_codepoint_t* str)
{
    for (size_t whitespace_length = 0; 0 < (whitespace_length = utf8_codepoint_is_whitespace(str)); str += whitespace_length) {
        //noop
    };

    return (utf8_codepoint_t*)str;
}

size_t utf8_string_trimmed_length(const utf8_codepoint_t* str, size_t max_codepoints, size_t* bytes)
{
    size_t trimed_codepoints = 0, trimed_bytes = 0, total_bytes = 0;
    for (size_t codepoints = 0; codepoints < max_codepoints; ++codepoints) {
        size_t codepoint_length = utf8_codepoint_length(str);
        size_t whitespace_length_length = utf8_codepoint_is_whitespace(str);
        str += codepoint_length, total_bytes += codepoint_length;
        if (!whitespace_length_length) {
            trimed_codepoints = codepoints;
            trimed_bytes = total_bytes;
        }
    }
    if (bytes) {
        *bytes = trimed_bytes;
    }
    return trimed_codepoints;
}

size_t utf8_string_line_count(const utf8_codepoint_t* str)
{
    size_t lines = 0;
    for (size_t bytes = 0; utf8_string_line_length(str, &bytes); str += bytes) {
        ++lines;
    }
    return lines;
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

#ifndef strnstr
char* strnstr(const char* string1, const char* string2, size_t len)
{
    size_t length2;

    length2 = strlen(string2);
    if (!length2) {
        return (char*)string1;
    }

    while (len >= length2) {
        len--;
        if (!memcmp(string1, string2, length2))
            return (char*)string1;
        string1++;
    }
    return NULL;
}
#endif