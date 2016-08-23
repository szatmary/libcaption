/**********************************************************************************************/
/* Copyright 2016-2016 Twitch Interactive, Inc. or its affiliates. All Rights Reserved.       */
/*                                                                                            */
/* Licensed under the Apache License, Version 2.0 (the "License"). You may not use this file  */
/* except in compliance with the License. A copy of the License is located at                 */
/*                                                                                            */
/*     http://aws.amazon.com/apache2.0/                                                       */
/*                                                                                            */
/* or in the "license" file accompanying this file. This file is distributed on an "AS IS"    */
/* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the    */
/* License for the specific language governing permissions and limitations under the License. */
/**********************************************************************************************/

#ifndef LIBCAPTION_UTF8_H
#define LIBCAPTION_UTF8_H

#include <stddef.h>
#include <inttypes.h>

// These types exist to make the code more self dcoumenting
// utf8_char_t point is a null teminate string of utf8 encodecd chars
//
// utf8_size_t is the length of a string in chars
// size_t is bytes
typedef char utf8_char_t;
typedef size_t utf8_size_t;
/*! \brief
    \param

    Skiped continuation bytes
*/
const utf8_char_t* utf8_char_next (const char* s);
/*! \brief
    \param

    returnes the length of the char in bytes
*/
size_t utf8_char_length (const utf8_char_t* c);

/*! \brief
    \param

    returns length of the string in bytes
    size is number of charcter to count (0 to count until NULL term)
*/
size_t utf8_string_length (const utf8_char_t* data, utf8_size_t size);
/*! \brief
    \param
*/
size_t utf8_char_copy (utf8_char_t* dst, const utf8_char_t* src);

/*! \brief
    \param

    returnes the number of utf8 charcters in a string givne the numbe of bytes
    to coutn until the a null terminator, pass 0 for size
*/
utf8_size_t utf8_char_count (const char* data, size_t size);
/*! \brief
    \param

    returnes the length of the line in bytes triming not printable charcters at the end
*/
size_t utf8_trimmed_length (const char* data, size_t size);
/*! \brief
    \param

    returns the length in bytes of the line including the new line charcter(s)
    auto detects between windows(CRLF), unix(LF), mac(CR) and riscos (LFCR) line endings
*/
size_t utf8_line_length (const char* data);
/*! \brief
    \param

    returns number of chars to include before split
*/
utf8_size_t utf8_wrap_length (const utf8_char_t* data, utf8_size_t size);

/*! \brief
    \param

    returns number of new lins in teh string
*/
int utf8_line_count (const utf8_char_t* data);


#endif
