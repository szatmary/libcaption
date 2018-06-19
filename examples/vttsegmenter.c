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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "sei.h"

#define MAX_VTT_SIZE (10 * 1024 * 1024)
#define MAX_READ_SIZE 4096

void vtt_write_header(FILE* vttFile)
{
    fprintf(vttFile, "WEBVTT\r\n\r\n");
}

void vtt_write_block(vtt_block_t* block, FILE* vttFile)
{
    switch (block->type) {
    case VTT_REGION:
        fprintf(vttFile, "REGION\r\n%s\r\n\r\n", vtt_block_data(block));
        break;
    case VTT_STYLE:
        fprintf(vttFile, "STYLE\r\n%s\r\n\r\n", vtt_block_data(block));
        break;
    case VTT_NOTE:
        fprintf(vttFile, "NOTE\r\n%s\r\n\r\n", vtt_block_data(block));
        break;
    case VTT_CUE: {
        if (block->cue_id != NULL) {
            fprintf(vttFile, "%s\r\n", block->cue_id);
        }

        int hh1, hh2, mm1, mm2, ss1, ss2, ms1, ms2;
        vtt_crack_time(block->timestamp, &hh1, &mm1, &ss1, &ms1);
        vtt_crack_time(block->timestamp + block->duration, &hh2, &mm2, &ss2, &ms2);

        fprintf(vttFile, "%d:%02d:%02d.%03d --> %02d:%02d:%02d.%03d",
            hh1, mm1, ss1, ms1, hh2, mm2, ss2, ms2);

        if (block->cue_settings != NULL) {
            printf(" %s", block->cue_settings);
        }

        fprintf(vttFile, "\r\n%s\r\n\r\n", vtt_block_data(block));
    } break;
    }
}

/**
 * vttsegmenter filename.vtt segment_size duration output_pattern_%05d.vtt
 */
int main(int argc, char** argv)
{
    if (argc != 5) {
        fprintf(stderr, "Usage: vttsegmenter filename.vtt segment_size duration output%%05d.vtt\n");
        return 0;
    }

    double segment_size;
    double duration;
    if (1 != sscanf(argv[2], "%lf", &segment_size)) {
        return 1;
    }
    if (1 != sscanf(argv[3], "%lf", &duration)) {
        return 1;
    }

    size_t size;
    utf8_char_t* data = utf8_load_text_file(argv[1], &size);
    if (data == NULL) {
        fprintf(stderr, "Failed to load input file\n");
        return 1;
    }
    vtt_t* vtt = vtt_parse(data, size);
    if (vtt == NULL) {
        fprintf(stderr, "Failed to parse vtt\n");
        return 1;
    }

    char filename[1024];

    for (int i = 0; i * segment_size < duration; i++) {
        if (snprintf(filename, 1024, argv[4], i) < 0) {
            fprintf(stderr, "Invalid filename pattern for output\n");
            return 1;
        }

        FILE* outputFile = fopen(filename, "w");
        if (outputFile == NULL) {
            fprintf(stderr, "Failed to open output file for reading: '%s'\n", filename);
            return 1;
        }

        vtt_write_header(outputFile);
        vtt_block_t* region = vtt->region_head;
        while (region != NULL) {
            vtt_write_block(region, outputFile);
            region = region->next;
        }

        vtt_block_t* style = vtt->style_head;
        while (style != NULL) {
            vtt_write_block(style, outputFile);
            style = style->next;
        }

        double segment_start = i * segment_size;
        double segment_end = (i + 1) * segment_size;

        vtt_block_t* cue = vtt->cue_head;
        while (cue != NULL) {
            if ((cue->timestamp < segment_end) && ((cue->timestamp + cue->duration) > segment_start)) {
                vtt_write_block(cue, outputFile);
            }
            cue = cue->next;
        }

        fclose(outputFile);
    }

    vtt_free(vtt);
}
