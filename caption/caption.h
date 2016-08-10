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

#ifndef LIBCAPTION_H
#define LIBCAPTION_H
#include "utf8.h"
#include "eia608.h"

// ssize_t is POSIX and does not exist on Windows
#if defined(_MSC_VER)
#if defined(_WIN64)
typedef signed long ssize_t;
#else
typedef signed int ssize_t;
#endif
#endif

#define SCREEN_ROWS 15
#define SCREEN_COLS 32

typedef struct {
    unsigned int uln : 1; //< underline
    unsigned int sty : 3; //< style
    utf8_char_t data[5];  //< 4 byte utf8 values plus null term
}  caption_frame_cell_t;

typedef struct  {
    unsigned int uln : 1; //< underline
    unsigned int sty : 3; //< style
    unsigned int mod : 3; //< current mode
    unsigned int rup : 2; //< roll-up line count minus 1
    int row, col;
} caption_frame_state_t;

typedef struct {
    double str_pts;
    double end_pts;
    caption_frame_state_t state;
    caption_frame_cell_t cell[SCREEN_ROWS][SCREEN_COLS];
} caption_frame_t;

// typedef enum {
//     eia608_paint_on = 0,
//     eia608_pop_on   = 1,
//     eia608_rollup_2 = 2,
//     eia608_rollup_3 = 3,
//     eia608_rollup_4 = 4,
// } eia608_display_mode_t;
// eia608_display_mode_t caption_frame_mode (caption_frame_t* frame);





/*!
    \brief Initializes an allocated caption_frame_t instance
    \param frame Pointer to prealocated caption_frame_t object
*/
void caption_frame_init (caption_frame_t* frame);
/*! \brief Writes a single charcter to a caption_frame_t object
    \param frame A pointer to an allocted and initialized caption_frame_t object
    \param row Row position to write charcter, must be between 0 and SCREEN_ROWS-1
    \param col Column position to write charcter, must be between 0 and SCREEN_ROWS-1
    \param style Style to apply to charcter
    \param underline Set underline attribute, 0 = off any other value = on
    \param c pointer to a single valid utf8 charcter. Bytes are automatically determined, and a NULL terminator is not required
*/
int caption_frame_write_char (caption_frame_t* frame, int row, int col, eia608_style_t style, int underline, const utf8_char_t* c);
/*! \brief
    \param
*/
const utf8_char_t* caption_frame_read_char (caption_frame_t* frame, int row, int col, eia608_style_t* style, int* underline);
/*! \brief
    \param
*/
caption_frame_cell_t* frame_cell (caption_frame_t* frame, int row, int col);

#define LIBCAPTION_OK 1
#define LIBCAPTION_ERROR 0
#define LIBCAPTION_READY 2
/*! \brief
    \param
*/
int caption_frame_decode (caption_frame_t* frame, uint16_t cc_data, double pts);

/*! \brief
    \param
*/
int caption_frame_from_text (caption_frame_t* frame, const utf8_char_t* data);
/*! \brief
    \param
*/
#define CAPTION_FRAME_DUMP_BUF_SIZE 4096
size_t caption_frame_dump_buffer (caption_frame_t* frame, utf8_char_t* buf);
void caption_frame_dump (caption_frame_t* frame);
/*! \brief
    \param
*/
#define CAPTION_FRAME_JSON_BUF_SIZE 32768
size_t caption_frame_json (caption_frame_t* frame, utf8_char_t* buf);

#endif
