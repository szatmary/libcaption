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
    re2c:indent:string = "    ";
    re2c:flags:tags = 1;
    re2c:yyfill:enable = 0;
    re2c:define:YYCTYPE = "unsigned char";
*/

size_t utf8_codepoint_length(const utf8_codepoint_t* codepoint)
{
    if (!codepoint) {
        return 0;
    }

    const unsigned char* YYCURSOR = (unsigned char*)codepoint;
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

// returns number of bytes copied dst should have at least 5 bytes to be safe
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
    const unsigned char* YYCURSOR = (unsigned char*)codepoint;
    /*!re2c
    * { return 0; }
    [ \t\r\n] { return 1; }
    */
}

size_t utf8_string_length(const utf8_codepoint_t* str, size_t* bytes)
{
    size_t cc = 0;
    size_t cb = 0;
    const unsigned char* YYCURSOR = (unsigned char*)str;
    for (;;) {
        /*!re2c
        [\x00\xff] { break; }
        [\x01-\x7f] { ++cc, ++cb; }
        [\x80-\xbf] { ++cb; continue; }
        [\xc0-\xdf] [\x80-\xbf] {2} { ++cc, cb += 2; }
        [\xe0-\xef] [\x80-\xbf] {3} { ++cc, cb += 3; }
        [\xf0-\xfe] [\x80-\xbf] {4} { ++cc, cb += 4; }
    */
    }

    if (bytes) {
        *bytes = cb;
    }

    return cc;
}

size_t utf8_string_wrap_length(const utf8_codepoint_t* str, size_t line_length, size_t* bytes)
{
    size_t wb = 0, wc = 0; // wrap bytes,  wrap count
    size_t cb = 0, cc = 0; // codepoint bytes, codepoint count
    const unsigned char* YYCURSOR = (unsigned char*)str;
    while (cc < line_length) {
        /*!re2c
        [\x00\xff] { break; }
        [ \s\r\n] { ++cc; ++cb; continue; }
        [\x01-\x7f] { ++cc; ++cb; wc = cc; wb = cb; continue; }
        [\x80-\xbf] { ++cb; continue; }
        [\xc0-\xdf] [\x80-\xbf] {2} { ++cc; cb += 2; wc = cc; wb = cb; continue; }
        [\xe0-\xef] [\x80-\xbf] {3} { ++cc; cb += 3; wc = cc; wb = cb; continue; }
        [\xf0-\xfe] [\x80-\xbf] {4} { ++cc; cb += 4; wc = cc; wb = cb; continue; }
    */
    }

    if (!wc) {
        wc = cc, wb = cb;
    }

    if (bytes) {
        *bytes = wb;
    }

    return wc;
}

size_t utf8_string_line_length(const utf8_codepoint_t* str, size_t* bytes)
{
    size_t cb = 0;
    size_t cc = 0;
    const unsigned char* YYCURSOR = (unsigned char*)str;
    for (;;) {
        /*!re2c
        [\x00\xff\r\n] { break; }
        [\x01-\x7f] { ++cc; ++cb; continue; }
        [\x80-\xbf] { ++cb; continue; }
        [\xc0-\xdf] [\x80-\xbf] {2} { ++cc; cb += 2; continue; }
        [\xe0-\xef] [\x80-\xbf] {3} { ++cc; cb += 3; continue; }
        [\xf0-\xfe] [\x80-\xbf] {4} { ++cc; cb += 4; continue; }
    */
    }

    if (bytes) {
        *bytes = cb;
    }

    return cc;
}

const utf8_codepoint_t* utf8_string_skip_whitespace(const utf8_codepoint_t* str)
{
    const unsigned char* YYCURSOR = (unsigned char*)str;
    for (;;) {
        /*!re2c
        [ \s\r\n]  { str += utf8_codepoint_length(str); continue; }
        * { return str; }
    */
    }
}

size_t utf8_string_line_count(const utf8_codepoint_t* str)
{
    if (!str || !str[0]) {
        return 0;
    }

    const unsigned char* YYCURSOR = (unsigned char*)str;
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
    utf8_codepoint_t* data = 0;
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