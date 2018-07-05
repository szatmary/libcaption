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
#ifndef LIBCAPTION_UTF8_H
#define LIBCAPTION_UTF8_H
#ifdef __cplusplus
extern "C" {
#endif

// #include "types.h"
#include <inttypes.h>
#include <stddef.h>

// These types exist to make the code more self dcoumenting
// utf8_char_t point is a null teminate string of utf8 encodecd chars
//
// utf8_size_t is the length of a string in chars
// size_t is bytes
typedef char utf8_codepoint_t;
#define MAX_UTF8_STRING_BYTES (50 * 1024 * 1024)

/*! \brief Returns a pointer to the next codepoint in a utf8 string
    \param

*/
const utf8_codepoint_t* utf8_codepoint_next(const utf8_codepoint_t* codepoint);
/*! \brief returns the length of the codepoint in bytes
    \param

    if codepoint points to a continuation byte, The number of bytes before the next found codepoint is returned
*/
//
size_t utf8_codepoint_length(const utf8_codepoint_t* codepoint);

/*! \brief copyies on codepoind fron src to dst
    \param
    returns number of bytes copied dst should have at least 5 bytes to be safe
*/
size_t utf8_codepoint_copy(utf8_codepoint_t* dst, const utf8_codepoint_t* src);

/*! \brief
    \param

    returns the length of the codepoint in bytes if it is a whitepace charcter, Otherwise returns 0
*/
size_t utf8_codepoint_is_whitespace(const utf8_codepoint_t* c);
/*! \brief returns the length of the codepoint in bytes if it is a newline charcter, Otherwise returns 0
    \param

    
    // returns 0 if codepoint is not a new line charcter
    // Returns 1 for single byte new lines (i.e. windows/riscos)
    // Returns 2 for multi-byte new lines (i.e. unix/macos)
    // Assumes exastance of a null terminator
*/
size_t utf8_codepoint_is_newline(const utf8_codepoint_t* codepoint);

/*! \brief
    \param

    returns the number of codepoints in the string, up to a null terminator
    bytes (if supplied) will be set to the number of byts the string consumes
*/
size_t utf8_string_length(const utf8_codepoint_t* str, size_t* bytes);

/*! \brief
    \param

    // returns the number of codepoints of up to and including the first new line charcter(s)
    // auto detects between windows(CRLF), unix(LF), mac(CR) and riscos (LFCR) line endings
*/
size_t utf8_string_line_length(const utf8_codepoint_t* str, size_t* bytes);

/*! \brief
    \param

    // returns number of codepoints to include before wraping the line
    // If a new line charcter is encountered, it is included, thus the return value can be larger than max_codepoints
    // Combined with utf8_string_trimmed_length() 
*/
size_t utf8_string_wrap_length(const utf8_codepoint_t* str, size_t max_codepoints, size_t* bytes);

/*! \brief
    \param


    // returns the number of codepoint triming not printable charcters at the end
*/
size_t utf8_string_trimmed_length(const utf8_codepoint_t* str, size_t max_codepoints, size_t* bytes);

/*! \brief
    \param

    returns the number of lines in the the string
    auto detects between windows(CRLF), unix(LF), mac(CR) and riscos (LFCR) line endings
*/
size_t utf8_string_line_count(const utf8_codepoint_t* str);

/*! \brief
    \param
*/
utf8_codepoint_t* utf8_load_text_file(const char* path, size_t* size);

/*! \brief
    \param

    Compares 2 strings up to max len
*/
#ifndef strnstr
char* strnstr(const char* string1, const char* string2, size_t len);
#endif

#ifdef __cplusplus
}
#endif
#endif
