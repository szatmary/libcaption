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

// void srt_cue_from_sei(srt_t* srt, caption_frame_t* frame, const uint8_t* data, size_t size, double pts, double cts)
// {
//     sei_t sei;
//     sei_init(&sei);
//     // IS the nalu_type always 1 byte?
//     sei_parse_avcnalu(&sei, data + 1, size - 1, pts, cts);
//     sei_dump(&sei);

//     if (LIBCAPTION_READY == sei_to_caption_frame(&sei, frame)) {
//         caption_frame_dump(frame);
//         srt_cue_from_caption_frame(frame, srt);
//     }

//     sei_free(&sei);
// }

int main(int argc, char** argv)
{
    const char* path = argv[1];

    ts_t ts;
    srt_t* srt;
    avcnalu_t nalu;
    caption_frame_t frame;
    uint8_t pkt[TS_PACKET_SIZE];
    ts_init(&ts);
    avcnalu_init(&nalu);
    caption_frame_init(&frame);

    srt = srt_new();

    FILE* file = fopen(path, "rb+");

    // This fread 188 bytes at a time is VERY slow. Need to rewrite that
    while (TS_PACKET_SIZE == fread(&pkt[0], 1, TS_PACKET_SIZE, file)) {
        switch (ts_parse_packet(&ts, &pkt[0])) {
        case LIBCAPTION_OK:
            // fprintf (stderr,"read ts packet\n");
            break;

        case LIBCAPTION_READY: {
            size_t pkt_size = ts.size;
            const uint8_t* pkt_data = ts.data;

            while (pkt_size) {
                switch (avcnalu_parse_annexb(&nalu, &pkt_data, &pkt_size)) {
                case LIBCAPTION_OK:
                    break;

                case LIBCAPTION_ERROR:
                    fprintf(stderr, "LIBCAPTION_ERROR == avcnalu_parse_annexb()\n");
                    avcnalu_init(&nalu);
                    break;

                case LIBCAPTION_READY: {
                    switch (ts.stream_type) {
                    case STREAM_TYPE_h262:
                        if (0xB2 == h262nalu_type(&nalu)) {
                            if (LIBCAPTION_READY == h262_user_data_to_caption_frame(&frame, nalu.data, nalu.size, ts_dts_seconds(&ts), ts_cts_seconds(&ts))) {
                                caption_frame_dump(&frame);
                                srt_cue_from_caption_frame(&frame, srt);
                            }
                        }
                        break;

                    case STREAM_TYPE_h264:
                        if (6 == h264nalu_type(&nalu)) {
                            // fprintf(stderr, "h264nalu_type %02x (%ld)\n", h264nalu_type(&nalu), avcnalu_size(&nalu));
                            sei_t sei;
                            sei_init(&sei);
                            sei_parse_avcnalu(&sei, &nalu, ts_dts_seconds(&ts), ts_cts_seconds(&ts));
                            sei_dump(&sei);

                            if (LIBCAPTION_READY == sei_to_caption_frame(&sei, &frame)) {
                                caption_frame_dump(&frame);
                                srt_cue_from_caption_frame(&frame, srt);
                            }

                            sei_free(&sei);
                        }
                        break;
                    case STREAM_TYPE_h265:
                        break;
                    } // switch

                    // Reset nalu
                    avcnalu_init(&nalu);
                } break;
                } //switch
            } // while
        } break;

        case LIBCAPTION_ERROR:
            // fprintf (stderr,"read ts packet ERROR\n");
            break;
        }
    }

    // TODO make sure we are handeling the final nalu


    srt_dump(srt);
    srt_free(srt);

    return 1;
}
