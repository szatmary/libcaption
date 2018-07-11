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

#include "types.h"

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
*/
size_t utf8_codepoint_length(const utf8_codepoint_t* codepoint);

/*! \brief copyies on codepoind fron src to dst
    \param
*/
size_t utf8_codepoint_copy(utf8_codepoint_t* dst, const utf8_codepoint_t* src);

/*! \brief
    \param
*/
size_t utf8_codepoint_is_whitespace(const utf8_codepoint_t* c);
/*! \brief
    \param
*/
const utf8_codepoint_t* utf8_string_skip_whitespace(const utf8_codepoint_t* str);
/*! \brief
    \param
*/
const utf8_codepoint_t* utf8_string_length(const utf8_codepoint_t* str, size_t* codepoints);
/*! \brief
    \param
*/
const utf8_codepoint_t* utf8_string_wrap(const utf8_codepoint_t* str, size_t max_codepoints, size_t* codepoints);
/*! \brief
    \param
*/
const utf8_codepoint_t* utf8_string_line(const utf8_codepoint_t* str, size_t* codepoints);
/*! \brief
    \param

    returns the number of lines in the the string
    auto detects between windows(CRLF), unix(LF), mac(CR) and riscos (LFCR) line endings
*/
size_t utf8_string_line_count(const utf8_codepoint_t* str);

/*! \brief
    \param
*/
utf8_codepoint_t* utf8_string_copy(const utf8_codepoint_t* begin, const utf8_codepoint_t* end);

/*! \brief
    \param
*/
utf8_codepoint_t* utf8_load_text_file(const char* path, size_t* size);

#ifdef __cplusplus
}
#endif
#endif
