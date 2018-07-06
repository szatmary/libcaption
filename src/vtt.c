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
#include "vtt.h"
#include "utf8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

vtt_t* vtt_new()
{
    vtt_t* vtt = calloc(sizeof(vtt_t), 1);
    vtt->cue = vtt_block_vector_new();
    vtt->note = vtt_block_vector_new();
    vtt->style = vtt_block_vector_new();
    vtt->region = vtt_block_vector_new();
    return vtt;
}

void vtt_del(vtt_t* vtt)
{
    vtt_block_vector_del(&vtt->cue);
    vtt_block_vector_del(&vtt->note);
    vtt_block_vector_del(&vtt->style);
    vtt_block_vector_del(&vtt->region);
    free(vtt);
}

void vtt_block_ctor(vtt_block_t** vtt_block)
{
    (*vtt_block) = 0;
}

void vtt_block_cdor(vtt_block_t** vtt_block)
{
    if (*vtt_block) {
        free(*vtt_block);
        fprintf(stderr, "memleak\n");
    }
}

vtt_block_t* vtt_block_new(const utf8_codepoint_t* data, size_t size, enum VTT_BLOCK_TYPE type)
{
    vtt_block_t* block = malloc(sizeof(vtt_block_t) + size + 1);
    block->type = type;
    block->timestamp = 0.0;
    block->duration = 0.0;
    block->cue_settings = NULL;
    block->cue_id = NULL;
    block->text_size = size;

    utf8_codepoint_t* dest = (utf8_codepoint_t*)vtt_block_data(block);
    if (data) {
        memcpy(dest, data, size);
    } else {
        memset(dest, 0, size);
    }

    dest[size] = '\0';
    return block;
}

#define VTTTIME2SECONDS(HH, MM, SS, MS) ((HH * 3600.0) + (MM * 60.0) + SS + (MS / 1000.0))
double parse_timestamp(const utf8_codepoint_t* line)
{
    int hh, mm, ss, ms;
    if (sscanf(line, "%d:%2d:%2d%*1[,.]%3d", &hh, &mm, &ss, &ms) == 4) {
        return VTTTIME2SECONDS(hh, mm, ss, ms);
    }
    if (sscanf(line, "%2d:%2d%*1[,.]%3d", &mm, &ss, &ms) == 3) {
        return VTTTIME2SECONDS(0.0, mm, ss, ms);
    }
    return -1.0;
}

void parse_timestamps(const utf8_codepoint_t* line, double* start_pts, double* end_pts, char** cue_settings)
{
    char start_str[32];
    char end_str[32];
    char cue_str[1024];

    int matches = sscanf(line, " %31s --> %31s%1023[^\n\r]", start_str, end_str, cue_str);
    *start_pts = -1;
    *cue_settings = NULL;

    printf("Matches: %d\n", matches);

    if (matches >= 1) {
        *start_pts = parse_timestamp(start_str);
        printf("Start pts: %f\n", *start_pts);
    }
    if (matches >= 2) {
        *end_pts = parse_timestamp(end_str);
    }
    if ((matches == 3) && (strlen(cue_str) > 0)) {
        int cue_size = strlen(cue_str);
        *cue_settings = malloc(cue_size + 1);
        strncpy(*cue_settings, cue_str, cue_size);
        (*cue_settings)[cue_size] = '\0';
    }
}

vtt_t* vtt_parse(const utf8_codepoint_t* data, size_t size)
{
    return _vtt_parse(data, size, 0);
}

// TODO this may need to be rewrittent after utf8 stuff changes
vtt_t* _vtt_parse(const utf8_codepoint_t* data, size_t size, int srt_mode)
{
    vtt_t* vtt = NULL;
    double str_pts = 0, end_pts = 0;
    size_t trimmed_length, line_length = 0;
    char* cue_settings;
    enum VTT_BLOCK_TYPE block_type;
    size_t cue_id_length = 0;
    const utf8_codepoint_t* cue_id = NULL;

    if (!data || !size || size < 6) {
        return NULL;
    }

    // TODO: Support UTF-8 BOM?
    if (!srt_mode && strncmp(data, "WEBVTT", 6) != 0) {
        // WebVTT files must start with WEBVTT
        fprintf(stderr, "Invalid webvtt header: %.*s\n", 6, data);
        return NULL;
    }

    data += 6;
    size -= 6;

    vtt = vtt_new();

    for (;;) {
        data = utf8_string_skip_whitespace(data);
        line_length = utf8_string_line_length(data, 0); // Line length

        // line length only zero at EOF
        if (0 == line_length) {
            break;
        }

        if (strnstr(data, "REGION", line_length) != NULL) {
            block_type = VTT_REGION;
        } else if (strnstr(data, "STYLE", line_length) != NULL) {
            block_type = VTT_STYLE;
        } else if (strnstr(data, "NOTE", line_length) != NULL) {
            block_type = VTT_NOTE;
        } else if (strnstr(data, "-->", line_length) != NULL) {
            block_type = VTT_CUE;
            parse_timestamps(data, &str_pts, &end_pts, &cue_settings);
            if (str_pts == -1) {
                // Failed to parse timestamps
                fprintf(stderr, "Bad timestamp: %.*s\n", (int)line_length, data);
                return NULL;
            }
        } else {
            if (cue_id != NULL) {
                // Invalid text found
                fprintf(stderr, "ERR: Unrecognized block\n");
                return NULL;
            }

            cue_id = data;
            cue_id_length = line_length;

            data += line_length;
            size -= line_length;
            continue;
        }

        data += line_length;
        size -= line_length;

        // Caption text starts here
        const utf8_codepoint_t* text = data;
        size_t text_size = 0;

        do {
            size_t line_bytes = 0;
            line_length = utf8_string_line_length(data, &line_bytes);
            trimmed_length = utf8_string_trimmed_length(data, line_length, 0);
            // printf ("cap (%d): '%.*s'\n", line_length, (int) trimmed_length, data);
            data += line_bytes, size -= line_bytes;
        } while (trimmed_length);

        // should we trim here?

        vtt_block_t** block = 0;
        switch (block_type) {
        case VTT_REGION:
            block = vtt_block_vector_push_back(&vtt->region);
            break;
        case VTT_STYLE:
            block = vtt_block_vector_push_back(&vtt->style);
            break;
        case VTT_NOTE:
            block = vtt_block_vector_push_back(&vtt->note);
            break;
        case VTT_CUE:
            block = vtt_block_vector_push_back(&vtt->cue);
            break;
        };

        // vtt_block_t* block = vtt_block_new(vtt, text, text_size, block_type);
        (*block) = vtt_block_new(text, text_size, block_type);

        if (block_type == VTT_CUE) {
            (*block)->timestamp = str_pts;
            (*block)->duration = end_pts - str_pts;
            (*block)->cue_settings = cue_settings;
            if (cue_id != NULL) {
                // TODO make a utf8_string_new function ti replace this
                (*block)->cue_id = malloc(cue_id_length + 1);
                memcpy((*block)->cue_id, cue_id, cue_id_length);
                (*block)->cue_id[cue_id_length] = '\0';
            }
        }

        cue_id = NULL;
    }

    return vtt;
}

int vtt_cue_to_caption_frame(vtt_block_t* cue, caption_frame_t* frame)
{
    const char* data = vtt_block_data(cue);
    return caption_frame_from_text(frame, data);
}

libcaption_stauts_t vtt_cue_from_caption_frame(caption_frame_t* frame, vtt_t* vtt)
{
    vtt_block_t** cue = vtt_block_vector_back(&vtt->cue);
    if (cue && 0 >= (*cue)->duration) {
        (*cue)->duration = frame->timestamp - (*cue)->timestamp;
    }

    // CRLF per row, plus an extra at the end
    cue = vtt_block_vector_push_back(&vtt->cue);
    (*cue) = vtt_block_new(NULL, 2 + CAPTION_FRAME_TEXT_BYTES, VTT_CUE);
    utf8_codepoint_t* data = vtt_block_data(*cue);

    caption_frame_to_text(frame, data);
    (*cue)->timestamp = frame->timestamp;
    // vtt requires an extra new line
    strcat((char*)data, "\r\n");
    return LIBCAPTION_OK;
}

static void _dump(vtt_t* vtt)
{
    vtt_block_t** block;
    printf("WEBVTT\r\n\r\n");

    for (size_t i = 0; i < vtt_block_vector_count(&vtt->region); ++i) {
        block = vtt_block_vector_at(&vtt->region, i);
        printf("REGION\r\n%s\r\n", vtt_block_data(*block));
    }

    for (size_t i = 0; i < vtt_block_vector_count(&vtt->style); ++i) {
        block = vtt_block_vector_at(&vtt->style, i);
        printf("STYLE\r\n%s\r\n", vtt_block_data(*block));
    }

    for (size_t i = 0; i < vtt_block_vector_count(&vtt->cue); ++i) {
        block = vtt_block_vector_at(&vtt->cue, i);
        int hh1, hh2, mm1, mm2, ss1, ss2, ms1, ms2;
        vtt_crack_time((*block)->timestamp, &hh1, &mm1, &ss1, &ms1);
        vtt_crack_time((*block)->timestamp + (*block)->duration, &hh2, &mm2, &ss2, &ms2);

        if ((*block)->cue_id != NULL) {
            printf("%s\n", (*block)->cue_id);
        }

        printf("%02d:%02d:%02d.%03d --> %02d:%02d:%02d.%03d",
            hh1, mm1, ss1, ms1, hh2, mm2, ss2, ms2);

        if ((*block)->cue_settings != NULL) {
            printf(" %s", (*block)->cue_settings);
        }

        printf("\r\n%s\r\n", vtt_block_data(*block));
    }
}

void vtt_dump(vtt_t* head) { _dump(head); }
