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
#include <stdlib.h>

#define LENGTH_SIZE 4
int main(int argc, char** argv)
{
    flvtag_t tag;
    srt_t* srt = 0;
    int has_audio, has_video;
    caption_frame_t frame;
    mpeg_bitstream_t mpegbs;
    const char* path = argv[1];

    flvtag_init(&tag);
    caption_frame_init(&frame);
    mpeg_bitstream_init(&mpegbs);

    FILE* flv = flv_open_read(path);
    srt = srt_new();

    if (!flv_read_header(flv, &has_audio, &has_video)) {
        fprintf(stderr, "'%s' Not an flv file\n", path);
    } else {
        fprintf(stderr, "Reading from '%s'\n", path);
    }

    while (flv_read_tag(flv, &tag)) {
        if (flvtag_avcpackettype_nalu == flvtag_avcpackettype(&tag)) {
            ssize_t size = flvtag_payload_size(&tag);
            uint8_t* data = flvtag_payload_data(&tag);

            while (0 < size) {
                size_t nalu_size = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
                mpeg_bitstream_parse(&mpegbs, &frame, (const uint8_t*)"\0\0\1", 3, STREAM_TYPE_H264, flvtag_dts_seconds(&tag), flvtag_cts_seconds(&tag));
                mpeg_bitstream_parse(&mpegbs, &frame, &data[LENGTH_SIZE], nalu_size, STREAM_TYPE_H264, flvtag_dts_seconds(&tag), flvtag_cts_seconds(&tag));
                data += nalu_size + LENGTH_SIZE, size -= nalu_size + LENGTH_SIZE;
                switch (mpeg_bitstream_status(&mpegbs)) {
                default:
                case LIBCAPTION_ERROR:
                    fprintf(stderr, "LIBCAPTION_ERROR == mpeg_bitstream_parse()\n");
                    mpeg_bitstream_init(&mpegbs);
                    return EXIT_FAILURE;
                    break;

                case LIBCAPTION_OK:
                    break;

                case LIBCAPTION_READY: {
                    caption_frame_dump(&frame);
                    srt_cue_from_caption_frame(&frame, srt);
                } break;
                } //switch
            }
        }
    }

    srt_dump(srt);
    srt_free(srt);

    return 1;
}
