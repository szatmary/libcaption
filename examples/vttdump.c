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
#include "avc.h"
#include "vtt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "sei.h"

#define MAX_VTT_SIZE (10 * 1024 * 1024)
#define MAX_READ_SIZE 4096

size_t read_file(FILE* file, utf8_char_t* data, size_t size)
{
    size_t read, totl = 0;

    while (0 < (read = fread(data, 1, MAX_READ_SIZE < size ? MAX_READ_SIZE : size, file))) {
        totl += read;
        data += read;
        size -= read;
    }

    return totl;
}

int main(int argc, char** argv)
{
    caption_frame_t frame;

    if (argc < 2) {
        return 0;
    }

    FILE* file = fopen(argv[1], "r");

    if (!file) {
        return 0;
    }

    utf8_char_t* data = (utf8_char_t*)malloc(MAX_VTT_SIZE);
    size_t size = read_file(file, data, MAX_VTT_SIZE);
    vtt_t* vtt = vtt_parse(data, size);

    for (vtt_cue_t* cue = vtt->cue_head; cue != NULL; cue = cue->next) {
        caption_frame_init(&frame);
        vtt_cue_to_caption_frame(cue, &frame);
        caption_frame_dump(&frame);
    }

    vtt_free(vtt);
}
