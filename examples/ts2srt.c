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
#include "srt.h"
#include "ts.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    const char* path = argv[1];

    ts_t ts;
    srt_t* srt;
    mpeg_bitstream_t mpegbs;
    caption_frame_t frame;
    uint8_t pkt[TS_PACKET_SIZE];
    ts_init(&ts);
    caption_frame_init(&frame);
    mpeg_bitstream_init(&mpegbs);

    srt = srt_new();
    FILE* file = fopen(path, "rb+");

    // This fread 188 bytes at a time is VERY slow. Need to rewrite that
    while (TS_PACKET_SIZE == fread(&pkt[0], 1, TS_PACKET_SIZE, file)) {
        if (LIBCAPTION_READY == ts_parse_packet(&ts, &pkt[0])) {
            while (ts.size) {
                size_t bytes_read = mpeg_bitstream_parse(&mpegbs, ts.data, ts.size);
                ts.data += bytes_read, ts.size -= bytes_read;
                switch (mpeg_bitstream_status(&mpegbs)) {
                case LIBCAPTION_OK:
                    break;

                case LIBCAPTION_ERROR:
                    fprintf(stderr, "LIBCAPTION_ERROR == mpeg_bitstream_parse()\n");
                    avcnalu_init(&mpegbs);
                    return EXIT_FAILURE;
                    break;

                case LIBCAPTION_READY: {
                    switch (mpeg_bitstream_packet_type(&mpegbs)) {
                    case H262_SEI_PACKET:
                            if (LIBCAPTION_READY == h262_user_data_to_caption_frame(&frame, &mpegbs, ts_dts_seconds(&ts), ts_cts_seconds(&ts))) {
                                caption_frame_dump(&frame);
                                srt_cue_from_caption_frame(&frame, srt);
                            }
                        break;

                    case H264_SEI_PACKET:{
                            // TODO 
                            // fprintf(stderr, "h264nalu_type %02x (%ld)\n", h264nalu_type(&mpegbs), avcnalu_size(&mpegbs));
                            sei_t sei;
                            sei_init(&sei);
                            sei_parse_avcnalu(&sei, &mpegbs, ts_dts_seconds(&ts), ts_cts_seconds(&ts));
                            // sei_dump(&sei);

                            if (LIBCAPTION_READY == sei_to_caption_frame(&sei, &frame)) {
                                caption_frame_dump(&frame);
                                srt_cue_from_caption_frame(&frame, srt);
                            }

                            sei_free(&sei);
                        }
                        break;
                    case H265_SEI_PACKET:
                        break;
                    } // switch

                    // Reset mpegbs
                    mpeg_bitstream_init(&mpegbs);
                } break;
                } //switch
            } // while
        } // if
    } // while

    // TODO make sure we are handeling the final nalu

    srt_dump(srt);
    srt_free(srt);

    return EXIT_SUCCESS;
}
