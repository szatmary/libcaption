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
#include "flv.h"
#include "mpeg.h"
#include "srt.h"
#include "wonderland.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SECONDS_PER_LINE 3.0
void append_caption(const utf8_char_t* data, srt_t* srt)
{
    int r;
    ssize_t size = (ssize_t)strlen(data);
    size_t char_count, line_length = 0, trimmed_length = 0;

    for (r = 0; 0 < size && SCREEN_ROWS > r; ++r) {
        line_length = utf8_line_length(data);
        trimmed_length = utf8_trimmed_length(data, line_length);
        char_count = utf8_char_count(data, trimmed_length);

        // If char_count is greater than one line can display, split it.
        if (SCREEN_COLS < char_count) {
            char_count = utf8_wrap_length(data, SCREEN_COLS);
            line_length = utf8_string_length(data, char_count + 1);
        }

        // fprintf (stderr,"%.*s\n", line_length, data);
        double timestamp = srt->cue_tail ? srt->cue_tail->timestamp + SECONDS_PER_LINE : 0;
        srt_cue_t* cue = srt_cue_new(srt, data, line_length);
        cue->timestamp = timestamp;
        cue->duration = SECONDS_PER_LINE;

        data += line_length;
        size -= (ssize_t)line_length;
    }
}

int main(int argc, char** argv)
{
    int i = 0;
    flvtag_t tag;
    srt_t* srt = 0;
    int has_audio, has_video;
    FILE* flv = flv_open_read(argv[1]);
    FILE* out = flv_open_write(argv[2]);
    flvtag_init(&tag);

    srt = srt_new();

    for (i = 0; wonderland[i][0]; ++i) {
        append_caption(wonderland[i], srt);
    }

    if (!flv_read_header(flv, &has_audio, &has_video)) {
        fprintf(stderr, "%s is not an flv file\n", argv[1]);
        return EXIT_FAILURE;
    }

    flv_write_header(out, has_audio, has_video);

    while (flv_read_tag(flv, &tag)) {
        if (srt->cue_head && flvtag_avcpackettype_nalu == flvtag_avcpackettype(&tag) && srt->cue_head->timestamp <= flvtag_pts_seconds(&tag)) {
            fprintf(stderr, "%f %s\n", flvtag_pts_seconds(&tag), srt_cue_data(srt->cue_head));
            flvtag_addcaption_text(&tag, srt_cue_data(srt->cue_head));
            srt_cue_free_head(srt);
        }

        flv_write_tag(out, &tag);
    }

    return EXIT_SUCCESS;
}
