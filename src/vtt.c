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

vtt_t *vtt_new() {
    vtt_t *vtt = malloc(sizeof(vtt_t));
    vtt->region_head = NULL;
    vtt->style_head = NULL;
    vtt->cue_head = NULL;
    return vtt;
}

void vtt_free(vtt_t* vtt)
{
    while (vtt->region_head != NULL) {
        vtt->region_head = vtt_region_free_head(vtt->region_head);
    }
    while (vtt->style_head != NULL) {
        vtt->style_head = vtt_style_free_head(vtt->style_head);
    }
    while (vtt->cue_head != NULL) {
        vtt->cue_head = vtt_cue_free_head(vtt->cue_head);
    }
    free(vtt);
}

vtt_region_t *vtt_region_new(vtt_t* vtt, const utf8_char_t* data, size_t size)
{
    vtt_region_t* region = malloc(sizeof(vtt_region_t) + size + 1);
    region->next = NULL;
    region->id_size = 0;
    region->region_id = NULL;
    region->block_size = size;
    
    if (vtt->region_head == NULL) {
        vtt->region_head = region;        
    } else {
        vtt->region_tail->next = region;
    }
    vtt->region_tail = region;

    utf8_char_t* dest = (utf8_char_t*)vtt_region_data(region);
    if (data) {
        memcpy(dest, data, size);
    } else {
        memset(dest, 0, size);
    }
    dest[size] = '\0';

    return region;
}

void vtt_region_set_id(vtt_region_t* region, const utf8_char_t* data, size_t size)
{
    if (region->region_id != NULL) {
        free(region->region_id);
    }

    region->id_size = size;
    region->region_id = malloc(size + 1);
    if (data) {
        memcpy(region->region_id, data, size);
    } else {
        memset(region->region_id, 0, size);
    }
    region->region_id[size] = '\0';
}

vtt_region_t* vtt_region_free_head(vtt_region_t* region)
{
    vtt_region_t* next = region->next;
    if (region->region_id != NULL) {
        free(region->region_id);
    }
    free(region);
    return next;
}

vtt_style_t *vtt_style_new(vtt_t* vtt, const utf8_char_t* data, size_t size)
{
    vtt_style_t* style = malloc(sizeof(vtt_style_t) + size + 1);
    style->next = NULL;
    if (vtt->style_head == NULL) {
        vtt->style_head = style;
    } else {
        vtt->style_tail->next = style;
    }
    vtt->style_tail = style;

    utf8_char_t* dest = (utf8_char_t*)vtt_style_data(style);
    if (data) {
        memcpy(dest, data, size);
    } else {
        memset(dest, 0, size);
    }
    dest[size] = '\0';

    return style;
}

vtt_style_t* vtt_style_free_head(vtt_style_t* style)
{
    vtt_style_t* next = style->next;
    free(style);
    return next;
}

vtt_cue_t* vtt_cue_new(vtt_t* vtt, const utf8_char_t* data, size_t size)
{
    vtt_cue_t* cue = malloc(sizeof(vtt_cue_t) + size + 1);
    cue->next = NULL;
    cue->timestamp = 0.0;
    cue->duration = 0.0;
    cue->cue_settings = NULL;
    cue->cue_id = NULL;
    cue->region = NULL;
    cue->text_size = size;

    if (vtt->cue_head == NULL) {
        vtt->cue_head = cue;
    } else {
        vtt->cue_tail->next = cue;
    }
    vtt->cue_tail = cue;

    utf8_char_t* dest = (utf8_char_t*)vtt_cue_data(cue);    
    if (data) {
        memcpy(dest, data, size);
    } else {
        memset(dest, 0, size);
    }

    dest[size] = '\0';
    return cue;
}

vtt_cue_t* vtt_cue_free_head(vtt_cue_t* head)
{
    vtt_cue_t* next = head->next;
    if (head->cue_id != NULL) {
        free(head->cue_id);
    }
    if (head->cue_settings != NULL) {
        free(head->cue_settings);
    }
    free(head);
    return next;
}


#define VTTTIME2SECONDS(HH, MM, SS, MS) ((HH * 3600.0) + (MM * 60.0) + SS + (MS / 1000.0))
double parse_timestamp(const utf8_char_t *line) {
    int hh, mm, ss, ms;
    if (sscanf(line, "%d:%2d:%2d.%3d", &hh, &mm, &ss, &ms) == 4) {
        return VTTTIME2SECONDS(hh, mm, ss, ms);
    }
    if (sscanf(line, "%2d:%2d.%3d", &mm, &ss, &ms) == 3) {
        return VTTTIME2SECONDS(0.0, mm, ss, ms);
    }
    return -1.0;
}

void parse_timestamps(const utf8_char_t* line, double *start_pts, double *end_pts, char **cue_settings) {
    char start_str[32];
    char end_str[32];
    char cue_str[1024];

    int matches = sscanf(line, " %31s --> %31s%1023[^\n\r]", start_str, end_str, cue_str);
    *start_pts = -1;
    *cue_settings = NULL;

    fprintf(stderr, "Matches: %d\n", matches);
    if (matches >= 1) {
        *start_pts = parse_timestamp(start_str);
        fprintf(stderr, "Start pts: %lf\n", *start_pts);
    }
    if (matches >= 2) {
        *end_pts = parse_timestamp(end_str);
        fprintf(stderr, "End pts: %lf\n", *end_pts);
    }
    if ((matches == 3) && (strlen(cue_str) > 0)) {
        int cue_size = strlen(cue_str);
        fprintf(stderr, "Cues: %d %s\n", cue_size, *cue_settings);
        *cue_settings = malloc(cue_size + 1);
        strncpy(*cue_settings, cue_str, cue_size);
        *cue_settings[cue_size] = '\0';
    }
}

vtt_t* vtt_parse(const utf8_char_t* data, size_t size)
{
    vtt_t *vtt = NULL;
    double str_pts = 0, end_pts = 0;
    size_t line_length = 0, trimmed_length = 0;
    char *cue_settings;
    enum VTT_BLOCK_TYPE block_type;
    size_t cue_id_length = 0;
    const utf8_char_t* cue_id = NULL;

    if (!data || !size || size < 6) {
        return NULL;
    }

    // TODO: Support UTF-8 BOM?
    if (strncmp(data, "WEBVTT", 6) != 0) {
        // WebVTT files must start with WEBVTT
        fprintf(stderr, "Invalid webvtt header: %.*s\n", 6, data);
        return NULL;
    }

    data += 6;
    size -= 6;

    vtt = vtt_new();

    for (;;) {
        line_length = 0;

        do {
            data += line_length;
            size -= line_length;
            line_length = utf8_line_length(data); // Line length 
            trimmed_length = utf8_trimmed_length(data, line_length);
            // Skip empty lines
        } while (0 < line_length && 0 == trimmed_length);

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
                fprintf(stderr, "Bad timestamp\n");
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
        const utf8_char_t* text = data;
        size_t text_size = 0;

        line_length = 0;

        do {
            text_size += line_length;
            line_length = utf8_line_length(data);
            trimmed_length = utf8_trimmed_length(data, line_length);
            // printf ("cap (%d): '%.*s'\n", line_length, (int) trimmed_length, data);
            data += line_length;
            size -= line_length;
        } while (trimmed_length);

        // should we trim here?
        // TODO: Add a notion of VTT type here (region, note, etc.)
        switch (block_type) {
            case VTT_REGION:
                vtt_region_new(vtt, text, text_size);
                // TODO: Set the region ID
                break;
            case VTT_STYLE:
                vtt_style_new(vtt, text, text_size);
                break;
            case VTT_CUE:
            {
                vtt_cue_t* cue = vtt_cue_new(vtt, text, text_size);
                cue->timestamp = str_pts;
                cue->duration = end_pts - str_pts;
                cue->cue_settings = cue_settings;
                if (cue_id != NULL) {
                    cue->cue_id = malloc(cue_id_length + 1);
                    memcpy(cue->cue_id, cue_id, cue_id_length);
                    cue->cue_id[cue_id_length] = '\0';
                }
            }
                break;
            case VTT_NOTE:
                break;
        }

        cue_id = NULL;
    }

    return vtt;
}

int vtt_cue_to_caption_frame(vtt_cue_t* cue, caption_frame_t* frame)
{
    const char* data = vtt_cue_data(cue);
    return caption_frame_from_text(frame, data);
}

vtt_cue_t* vtt_cue_from_caption_frame(caption_frame_t* frame, vtt_t *vtt)
{
    // CRLF per row, plus an extra at the end
    vtt_cue_t* cue = vtt_cue_new(vtt, NULL, 2 + CAPTION_FRAME_TEXT_BYTES);
    utf8_char_t* data = vtt_cue_data(cue);

    caption_frame_to_text(frame, data);
    // vtt requires an extra new line
    strcat((char*)data, "\r\n");

    return cue;
}

static inline void _crack_time(double tt, int* hh, int* mm, int* ss, int* ms)
{
    (*ms) = (int)((int64_t)(tt * 1000) % 1000);
    (*ss) = (int)((int64_t)(tt) % 60);
    (*mm) = (int)((int64_t)(tt / (60)) % 60);
    (*hh) = (int)((int64_t)(tt / (60 * 60)));
}

static void _dump(vtt_t* vtt)
{
    vtt_region_t* region;
    vtt_style_t* style;
    vtt_cue_t* cue;
    printf("WEBVTT\r\n");

    region = vtt->region_head;
    while (region != NULL) {
        printf("REGION\r\n%s\r\n", vtt_region_data(region));
        region = region->next;
    }

    style = vtt->style_head;
    while (style != NULL) {
        printf("STYLE\r\n%s\r\n", vtt_style_data(style));
        style = style->next;
    }

    cue = vtt->cue_head;
    while (cue != NULL) {
        int hh1, hh2, mm1, mm2, ss1, ss2, ms1, ms2;
        _crack_time(cue->timestamp, &hh1, &mm1, &ss1, &ms1);
        _crack_time(cue->timestamp + cue->duration, &hh2, &mm2, &ss2, &ms2);

        printf("%d:%02d:%02d.%03d --> %02d:%02d:%02d.%03d\r\n%s\r\n",
            hh1, mm1, ss1, ms1, hh2, mm2, ss2, ms2, vtt_cue_data(cue));
    }
}

void vtt_dump(vtt_t* head) { _dump(head); }
