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

vtt_t* vtt_new(const utf8_char_t* data, size_t size, double timestamp, vtt_t* prev, vtt_t** head)
{
    vtt_t* vtt = malloc(sizeof(vtt_t) + size + 1);
    vtt->next = 0;
    vtt->duration = 0;
    vtt->aloc = size;
    vtt->timestamp = timestamp;
    utf8_char_t* dest = (utf8_char_t*)vtt_data(vtt);

    if (prev) {
        prev->next = vtt;

        if (0 >= prev->duration) {
            prev->duration = timestamp - prev->timestamp;
        }
    }

    if (head && 0 == (*head)) {
        (*head) = vtt;
    }

    if (data) {
        memcpy(dest, data, size);
    } else {
        memset(dest, 0, size);
    }

    dest[size] = '\0';
    return vtt;
}

vtt_t* vtt_free_head(vtt_t* head)
{
    vtt_t* next = head->next;
    if (head->cue_settings != NULL) {
        free(head->cue_settings);
    }
    free(head);
    return next;
}

void vtt_free(vtt_t* vtt)
{
    while (vtt) {
        vtt = vtt_free_head(vtt);
    }
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

void parse_timestamps(const utf8_char_t* line, double *start_pts, double *end_pts, char **cues) {
    char start_str[32];
    char end_str[32];
    char cue_str[1024];

    int matches = sscanf(line, " %31s --> %31s%1023[^\n\r]", start_str, end_str, cue_str);
    *start_pts = -1;
    *cues = NULL;

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
        fprintf(stderr, "Cues: %d %s\n", cue_size, *cues);
        *cues = malloc(cue_size + 1);
        strncpy(*cues, cue_str, cue_size);
        *cues[cue_size] = '\0';
    }
}

vtt_t* vtt_parse(const utf8_char_t* data, size_t size)
{
    int counter;
    vtt_t *head = 0, *prev = 0;
    double str_pts = 0, end_pts = 0;
    size_t line_length = 0, trimmed_length = 0;
    char *cue_settings;

    if (!data || !size || size < 6) {
        return NULL;
    }

    if (strncmp(data, "WEBVTT", 6) != 0) {
        // WebVTT files must start with WEBVTT
        printf("Invalid webvtt header: %.*s\n", 6, data);
        return NULL;
    }

    data += 6;
    size -= 6;

    for (;;) {
        line_length = 0;

        do {
            data += line_length;
            size -= line_length;
            line_length = utf8_line_length(data);
            trimmed_length = utf8_trimmed_length(data, line_length);
            // Skip empty lines
        } while (0 < line_length && 0 == trimmed_length);

        // linelength cant be zero before EOF
        if (0 == line_length) {
            break;
        }

        if (strnstr(data, "-->", line_length) != NULL) {
            fprintf(stderr, "Parse timestamps\n");
            parse_timestamps(data, &str_pts, &end_pts, &cue_settings);
            if (str_pts == -1) {
                // Failed to parse timestamps
                printf("Bad timestamp\n");
                return NULL;
            }
        } else {
            // Expected timestamp
            printf("Expected timestamp\n");
            return NULL;
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
        vtt_t* vtt = vtt_new(text, text_size, str_pts, prev, &head);
        prev = vtt;
        vtt->duration = end_pts - str_pts;
        vtt->cue_settings = cue_settings;
    }

    return head;
}

int vtt_to_caption_frame(vtt_t* vtt, caption_frame_t* frame)
{
    const char* data = vtt_data(vtt);
    return caption_frame_from_text(frame, data);
}

vtt_t* vtt_from_caption_frame(caption_frame_t* frame, vtt_t* prev, vtt_t** head)
{
    // CRLF per row, plus an extra at the end
    vtt_t* vtt = vtt_new(NULL, 2 + CAPTION_FRAME_TEXT_BYTES, frame->timestamp, prev, head);
    utf8_char_t* data = vtt_data(vtt);

    caption_frame_to_text(frame, data);
    // vtt requires an extra new line
    strcat((char*)data, "\r\n");

    return vtt;
}

static inline void _crack_time(double tt, int* hh, int* mm, int* ss, int* ms)
{
    (*ms) = (int)((int64_t)(tt * 1000) % 1000);
    (*ss) = (int)((int64_t)(tt) % 60);
    (*mm) = (int)((int64_t)(tt / (60)) % 60);
    (*hh) = (int)((int64_t)(tt / (60 * 60)));
}

static void _dump(vtt_t* head)
{
    int i;
    vtt_t* vtt;

    printf("WEBVTT\r\n");

    for (vtt = head, i = 1; vtt; vtt = vtt_next(vtt), ++i) {
        int hh1, hh2, mm1, mm2, ss1, ss2, ms1, ms2;
        _crack_time(vtt->timestamp, &hh1, &mm1, &ss1, &ms1);
        _crack_time(vtt->timestamp + vtt->duration, &hh2, &mm2, &ss2, &ms2);

        printf("%d:%02d:%02d.%03d --> %02d:%02d:%02d.%03d\r\n%s\r\n",
            hh1, mm1, ss1, ms1, hh2, mm2, ss2, ms2, vtt_data(vtt));
    }
}

void vtt_dump(vtt_t* head) { _dump(head); }
