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
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    flvtag_t tag;
    scc_t* scc = NULL;
    size_t scc_size = 0;

    if (4 > argc) {
        fprintf(stderr, "Usage: %s input.flv input.scc output.flv\nuse '-' for stdin or stdout\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* flv = flv_open_read(argv[1]);
    utf8_char_t* scc_data_ptr = utf8_load_text_file(argv[2], &scc_size);
    utf8_char_t* scc_data = scc_data_ptr;
    FILE* out = flv_open_write(argv[3]);

    if (!flv) {
        fprintf(stderr, "Falule to open input flv '%s'\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (!scc_data) {
        fprintf(stderr, "Falule to open input scc '%s'\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    if (!out) {
        fprintf(stderr, "Falule to open output flv '%s'\n", argv[3]);
        exit(EXIT_FAILURE);
    }

    int has_audio, has_video;
    flvtag_init(&tag);

    if (!flv_read_header(flv, &has_audio, &has_video)) {
        fprintf(stderr, "%s is not an flv file\n", argv[1]);
        return EXIT_FAILURE;
    }

    flv_write_header(out, has_audio, has_video);

    // read the first scc
    scc_data += scc_to_608(&scc, scc_data);

    while (flv_read_tag(flv, &tag)) {
        double timestamp = flvtag_pts_seconds(&tag);

        if (scc && scc->cc_size && scc->timestamp < timestamp && flvtag_avcpackettype_nalu == flvtag_avcpackettype(&tag)) {
            flvtag_addcaption_scc(&tag, scc);
            scc_data += scc_to_608(&scc, scc_data);
        }

        flv_write_tag(out, &tag);
    }

    free(scc_data_ptr);
    flvtag_free(&tag);
    return EXIT_SUCCESS;
}
