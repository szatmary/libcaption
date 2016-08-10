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

#include "utf8.h"
#include "eia608.h"
#include "caption.h"
#include <stdio.h>
#include <string.h>
////////////////////////////////////////////////////////////////////////////////
void caption_frame_clear (caption_frame_t* frame)
{
    frame->state = (caption_frame_state_t) {0,0,0,0,0,0}; // clear global state
    memset (&frame->cell[0][0],0,sizeof (caption_frame_cell_t) *SCREEN_COLS*SCREEN_ROWS);
}

void caption_frame_init (caption_frame_t* frame)
{
    frame->str_pts = -1;
    frame->end_pts = -1;
    caption_frame_clear (frame);
}
////////////////////////////////////////////////////////////////////////////////
#define CAPTION_CLEAR     0
#define CAPTION_LOADING   1
#define CAPTION_POP_ON    2
#define CAPTION_PAINT_ON  3
#define CAPTION_ROLL_UP   4
////////////////////////////////////////////////////////////////////////////////
// Helpers
caption_frame_cell_t* frame_cell (caption_frame_t* frame, int row, int col)
{
    return &frame->cell[row][col];
}

static caption_frame_cell_t* frame_cell_get (caption_frame_t* frame)
{
    return frame_cell (frame, frame->state.row, frame->state.col);
}

static char* frame_cell_get_char (caption_frame_t* frame)
{
    return &frame_cell_get (frame)->data[0];
}

static void eia608_cell_clear (caption_frame_cell_t* cell)
{
    memset (cell,0,sizeof (caption_frame_cell_t));
}

static void eia608_cell_clear_at (caption_frame_t* frame, int row, int col)
{
    eia608_cell_clear (frame_cell (frame, row, col));
}
////////////////////////////////////////////////////////////////////////////////
uint16_t _eia608_from_utf8 (const char* s); // function is in eia608.c.re2c
int caption_frame_write_char (caption_frame_t* frame, int row, int col, eia608_style_t style, int underline, const char* c)
{
    // Check to see if this is a valis charcter
    if (! _eia608_from_utf8 (c)) {
        return 0;
    }

    caption_frame_cell_t* cell = frame_cell (frame, row, col);

    if (utf8_char_copy (&cell->data[0],c)) {
        cell->uln = underline;
        cell->sty = style;
        return 1;
    }

    return 0;
}

const utf8_char_t* caption_frame_read_char (caption_frame_t* frame, int row, int col, eia608_style_t* style, int* underline)
{
    caption_frame_cell_t* cell = frame_cell (frame, row, col);

    if (style) {
        (*style) = cell->sty;
    }

    if (underline) {
        (*underline) = cell->uln;
    }

    return &cell->data[0];
}

////////////////////////////////////////////////////////////////////////////////
// Parsing
void caption_frame_carriage_return (caption_frame_t* frame)
{
    int c, r = frame->state.row - (frame->state.rup-1);

    if (0  >= r || CAPTION_ROLL_UP != frame->state.mod) {
        return;
    }

    for (; r < SCREEN_ROWS; ++r) {
        uint8_t* dst = (uint8_t*) frame_cell (frame,r-1,0);
        uint8_t* src = (uint8_t*) frame_cell (frame,r-0,0);
        memcpy (dst,src,sizeof (caption_frame_cell_t) * SCREEN_COLS);
    }

    memset (frame_cell (frame,SCREEN_ROWS-1,0), 0,sizeof (caption_frame_cell_t) * SCREEN_COLS);
}
////////////////////////////////////////////////////////////////////////////////
int eia608_write_char (caption_frame_t* frame, char* c)
{
    if (0 == c || 0 == c[0] ||
            SCREEN_ROWS <= frame->state.row || 0 > frame->state.row ||
            SCREEN_COLS <= frame->state.col || 0 > frame->state.col) {
        return 0;
    }

    caption_frame_cell_t* cell = frame_cell_get (frame);

    if (0 >= utf8_char_copy (&cell->data[0],c)) {
        return 0;
    } else {
        cell->sty = frame->state.sty;
        cell->uln = frame->state.uln;
        frame->state.col += 1;
        return 1;
    }
}

int caption_frame_decode_text (caption_frame_t* frame, uint16_t cc_data)
{
    int chan;
    char char1[5], char2[5];
    size_t chars = eia608_to_utf8 (cc_data, &chan, &char1[0], &char2[0]);

    if (0 < chars) {
        eia608_write_char (frame,char1);
    }

    if (1 < chars) {
        eia608_write_char (frame,char2);
    }

    return 1;
}

int caption_frame_decode_preamble (caption_frame_t* frame, uint16_t cc_data)
{
    eia608_style_t sty;
    int row, col, chn, uln;

    if (eia608_parse_preamble (cc_data, &row, &col, &sty, &chn, &uln)) {
        frame->state.row = row;
        frame->state.col = col;
        frame->state.sty = sty;
        frame->state.uln = uln;
        return 1;
    }

    return 0;
}

int caption_frame_decode_midrowchange (caption_frame_t* frame, uint16_t cc_data)
{
    eia608_style_t sty;
    int chn, unl;

    if (eia608_parse_midrowchange (cc_data,&chn,&sty,&unl)) {
        frame->state.sty = sty;
        frame->state.uln = unl;
    }

    return 0;
}


int caption_frame_decode_control (caption_frame_t* frame, uint16_t cc_data)
{
    int cc;
    eia608_control_t cmd = eia608_parse_control (cc_data,&cc);

    switch (cmd) {
    // PAINT ON
    case eia608_control_resume_direct_captioning:
        frame->state.rup = 0;
        frame->state.mod = CAPTION_PAINT_ON;
        return LIBCAPTION_OK;

    case eia608_control_erase_display_memory:
        caption_frame_clear (frame);

        if (CAPTION_PAINT_ON == frame->state.mod || CAPTION_ROLL_UP == frame->state.mod) {
            return LIBCAPTION_READY;
        } else {
            return LIBCAPTION_OK;
        }

    // ROLL-UP
    case eia608_control_roll_up_2:
        frame->state.rup = 1;
        frame->state.mod = CAPTION_ROLL_UP;
        return LIBCAPTION_OK;

    case eia608_control_roll_up_3:
        frame->state.rup = 2;
        frame->state.mod = CAPTION_ROLL_UP;
        return LIBCAPTION_OK;

    case eia608_control_roll_up_4:
        frame->state.rup = 3;
        frame->state.mod = CAPTION_ROLL_UP;
        return LIBCAPTION_OK;

    case eia608_control_carriage_return:
        caption_frame_carriage_return (frame);
        return LIBCAPTION_READY;

    // Corrections (Is this only valid as part of paint on?)
    case eia608_control_backspace:
        // do not reverse wrap (tw 28:20)
        frame->state.col = (0 < frame->state.col) ? (frame->state.col - 1) : 0;
        eia608_cell_clear_at (frame, frame->state.row, frame->state.col);
        return LIBCAPTION_READY;

    case eia608_control_delete_to_end_of_row: {
        int c;

        for (c = frame->state.col ; c < SCREEN_COLS ; ++c) {
            eia608_cell_clear_at (frame, frame->state.row, c);
        }
    }

    return LIBCAPTION_READY;

    // POP ON
    case eia608_control_resume_caption_loading:
        frame->state.rup = 0;
        frame->state.mod = CAPTION_LOADING;
        return LIBCAPTION_OK;

    case eia608_control_erase_non_displayed_memory:
        caption_frame_clear (frame);
        return LIBCAPTION_OK;

    case eia608_control_end_of_caption:
        frame->state.rup = 0;
        frame->state.mod = CAPTION_POP_ON;
        return LIBCAPTION_READY;

    // cursor positioning
    case eia608_tab_offset_0:
    case eia608_tab_offset_1:
    case eia608_tab_offset_2:
    case eia608_tab_offset_3:
        frame->state.col += (cmd - eia608_tab_offset_0);
        return LIBCAPTION_OK;

    // Unhandled
    default:
    case eia608_control_alarm_off:
    case eia608_control_alarm_on:
    case eia608_control_text_restart:
    case eia608_control_text_resume_text_display:
        return 0;
    }
}

int caption_frame_decode (caption_frame_t* frame, uint16_t cc_data, double pts)
{
    int status = 0;

    if (!eia608_varify (cc_data)) {
        return 0;
    }

    if (CAPTION_POP_ON == frame->state.mod) {
        // caption_frame_init (frame);
    } else if (CAPTION_PAINT_ON == frame->state.mod || CAPTION_ROLL_UP == frame->state.mod) {
        frame->str_pts = frame->end_pts = -1;
    }

    if (0 < pts) {
        if (0 > frame->str_pts) { frame->str_pts = pts; }

        if (pts > frame->end_pts) { frame->end_pts = pts; }
    }

    if (eia608_is_control (cc_data)) {
        status = caption_frame_decode_control (frame,cc_data);
    } else if (eia608_is_basicna (cc_data) ||
               eia608_is_specialna (cc_data) ||
               eia608_is_westeu (cc_data)) {

        // Don't decode text if we dont know what more we are in.
        if (CAPTION_CLEAR == frame->state.mod) {
            return 1;
        }

        status = caption_frame_decode_text (frame,cc_data);

        // If we are in paint on mode, display immiditally
        if (1 == status && (CAPTION_PAINT_ON == frame->state.mod || CAPTION_ROLL_UP == frame->state.mod)) {
            status = 2;
        }
    } else if (eia608_is_preamble (cc_data)) {
        status =  caption_frame_decode_preamble (frame,cc_data);
    } else if (eia608_is_midrowchange (cc_data)) {
        status =  caption_frame_decode_midrowchange (frame,cc_data);
    }

    return status;
}
////////////////////////////////////////////////////////////////////////////////
int caption_frame_from_text (caption_frame_t* frame, const utf8_char_t* data)
{
    int r, c, chan = 0;
    ssize_t size = strlen (data);
    size_t char_count, char_length, line_length = 0, trimmed_length = 0;
    caption_frame_init (frame);
    frame->state.mod = 2; // POP-ON

    for (r = 0 ; 0 < size && SCREEN_ROWS > r ; ++r) {
        const utf8_char_t* cap_data = data;
        line_length = utf8_line_length (cap_data);
        trimmed_length = utf8_trimmed_length (cap_data,line_length);
        char_count = utf8_char_count (cap_data,trimmed_length);

        // If char_count is greater than one line can display, split it.
        if (SCREEN_COLS < char_count) {
            char_count = utf8_wrap_length (cap_data,SCREEN_COLS);
            line_length = utf8_string_length (cap_data,char_count+1);
        }

        // Write the line
        for (c = 0 ; c < char_count ; ++c) {
            caption_frame_write_char (frame,r,c,eia608_style_white,0,&cap_data[0]);
            char_length = utf8_char_length (cap_data);
            cap_data += char_length;
        }

        data += line_length;
        size -= line_length;
    }

    return 0;
}
////////////////////////////////////////////////////////////////////////////////
size_t caption_frame_dump_buffer (caption_frame_t* frame, utf8_char_t* buf)
{
    int r, c;
    size_t bytes, total = 0;
    bytes = sprintf (buf, "   row: %d\tcol: %d\n   mode: %s\troll-up: %d\n",
                     frame->state.row, frame->state.col,
                     eia608_mode_map[frame->state.mod],frame->state.rup?1+frame->state.rup:0);
    total += bytes; buf += bytes;
    bytes = sprintf (buf, "   00000000001111111111222222222233\n   01234567890123456789012345678901\n  %s--------------------------------%s\n",
                     EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT, EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT);
    total += bytes; buf += bytes;

    for (r = 0 ; r < SCREEN_ROWS ; ++r) {
        bytes = sprintf (buf, "%02d%s", r, EIA608_CHAR_VERTICAL_LINE);
        total += bytes; buf += bytes;

        for (c = 0 ; c < SCREEN_COLS ; ++c) {
            caption_frame_cell_t* cell = frame_cell (frame,r,c);
            bytes = utf8_char_copy (buf, (0==cell->data[0]) ?EIA608_CHAR_SPACE:&cell->data[0]);
            total += bytes; buf += bytes;
        }

        bytes = sprintf (buf, "%s\n", EIA608_CHAR_VERTICAL_LINE);
        total += bytes; buf += bytes;
    }

    bytes = sprintf (buf, "  %s--------------------------------%s\n",
                     EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT, EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_LEFT);
    total += bytes; buf += bytes;
    return total;
}

void caption_frame_dump (caption_frame_t* frame)
{
    utf8_char_t buff[CAPTION_FRAME_DUMP_BUF_SIZE];
    size_t size = caption_frame_dump_buffer (frame, buff);
    fprintf (stderr,"%s\n", buff);
}

size_t caption_frame_json (caption_frame_t* frame, utf8_char_t* buf)
{
    size_t bytes, total = 0;
    int r,c,count = 0;
    bytes = sprintf (buf, "{\"format\":\"eia608\",\"mode\":\"%s\",\"rollUp\":%d,\"data\":[",
                     eia608_mode_map[frame->state.mod],frame->state.rup?1+frame->state.rup:0);
    total += bytes; buf += bytes;

    for (r = 0 ; r < SCREEN_ROWS ; ++r) {
        for (c = 0 ; c < SCREEN_COLS ; ++c) {
            caption_frame_cell_t* cell = frame_cell (frame,r,c);

            if (0 != cell->data[0]) {
                const char* data = ('"' == cell->data[0]) ?"\\\"": (const char*) &cell->data[0]; //escape quote
                bytes = sprintf (buf, "%s\n{\"row\":%d,\"col\":%d,\"char\":\"%s\",\"style\":\"%s\"}",
                                 (0<count?",":""),r,c,data,eia608_style_map[cell->sty]);
                total += bytes; buf += bytes; ++count;
            }
        }
    }

    bytes = sprintf (buf, "\n]}\n");
    total += bytes; buf += bytes;
    return total;
}
