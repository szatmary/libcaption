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

#ifdef __cplusplus
extern "C" {
#endif
#include <libavformat/avformat.h>
#ifdef __cplusplus
extern
}
#endif

#include "mpeg.h"

double av_time_to_seconds(int64_t a, AVRational *r) {
    return (a * r->num) / (double)r->den;
}

int main(int argc, char** argv)
{
    AVPacket pkt;
    AVCodec* videoCodec = 0;
    AVFormatContext* formatCtx = 0;
    const char* path = argv[1];
    caption_frame_t frame;
    mpeg_bitstream_t* mpegbs = mpeg_bitstream_new();
    int err = avformat_open_input(&formatCtx, path, 0, 0);
    caption_frame_ctor(&frame);
    avformat_find_stream_info(formatCtx, 0);
    int videoStreamIdx = av_find_best_stream(formatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, &videoCodec, 0);
    int stream_type = 0;
    switch (formatCtx->streams[videoStreamIdx]->codecpar->codec_id) {
    case AV_CODEC_ID_MPEG2VIDEO:
        printf("AV_CODEC_ID_MPEG2VIDEO\n");
        stream_type = STREAM_TYPE_H262;
        break;
    case AV_CODEC_ID_H264:
        printf("AV_CODEC_ID_H264\n");
        stream_type = STREAM_TYPE_H264;
        break;
    case AV_CODEC_ID_H265:
        printf("AV_CODEC_ID_HEVC\n");
        stream_type = STREAM_TYPE_H265;
        break;
    default:
        fprintf(stderr, "Unsupported codec (libavformat code_id %d\n", formatCtx->streams[videoStreamIdx]->codecpar->codec_id);
        // TODO clean exit
        exit(EXIT_FAILURE);
        break;
    }

    // TODO Some containers require we switch to annex b

    av_init_packet(&pkt);
    while (0 <= av_read_frame(formatCtx, &pkt)) {
        if (videoStreamIdx == pkt.stream_index) {
            uint8_t *data = pkt.data;
            size_t size = pkt.size;
            double dts = av_time_to_seconds(pkt.dts, &formatCtx->streams[pkt.stream_index]->time_base);
            double cts = av_time_to_seconds(pkt.pts - pkt.dts, &formatCtx->streams[pkt.stream_index]->time_base);
            while(size) {
                size_t bytes_read = mpeg_bitstream_parse(mpegbs, &frame, data, size, stream_type, dts, cts);
                data += bytes_read, size -= bytes_read;
                switch (mpeg_bitstream_status(mpegbs)) {
                default:
                case LIBCAPTION_ERROR:
                    fprintf(stderr, "LIBCAPTION_ERROR == mpeg_bitstream_parse()\n");
                    return EXIT_FAILURE;
                    break;

                case LIBCAPTION_OK:
                    break;

                case LIBCAPTION_READY: {
                    caption_frame_dump(&frame);
                    // srt_cue_from_caption_frame(&frame, srt);
                } break;
                } //switch
            }
        }

        av_packet_unref(&pkt);
    }

    // Flush anything left
    // while (mpeg_bitstream_flush(mpegbs, &frame)) {
    //     if (mpeg_bitstream_status(mpegbs)) {
    //         srt_cue_from_caption_frame(&frame, srt);
    //     }
    // }

    mpeg_bitstream_del(mpegbs);
}