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
#include "flv.h"
#include "srt.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_SRT_SIZE (1 * 1024 * 1024)

// retunes number of bytes read
// negative number on error
// retursn 0 on 'not ready' and 'eof'
// eof set to 1 on end, otherwise zero
size_t fd_read(int fd, uint8_t* data, size_t size, int* eof)
{
    fd_set rfds;
    struct timeval tv;
    int retval;

    (*eof) = 0;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 1;
    retval = select(fd + 1, &rfds, NULL, NULL, &tv);

    if (0 > retval) {
        return retval;
    }

    // not ready
    if (!(retval && FD_ISSET(fd, &rfds))) {
        return 0;
    }

    retval = read(fd, data, size);

    if (0 == retval) {
        (*eof) = 1;
    }

    return retval;
}

size_t g_srt_size = 0;
utf8_char_t g_srt_data[MAX_SRT_SIZE];
srt_t* srt_from_fd(int fd)
{
    int eof;
    uint8_t c;

    for (;;) {
        int ret = fd_read(fd, &c, 1, &eof);

        if (eof || (1 == ret && 0 == c)) {
            srt_t* srt = srt_parse(&g_srt_data[0], g_srt_size);
            g_srt_size = 0;
            return srt;
        }

        if (1 == ret) {
            if (g_srt_size >= MAX_SRT_SIZE - 1) {
                fprintf(stderr, "Warning MAX_SRT_SIZE reached. Clearing buffer\n");
                g_srt_size = 0;
            }

            g_srt_data[g_srt_size] = c;
            g_srt_size += 1;
        } else {
            return 0;
        }
    }
}

int main(int argc, char** argv)
{
    flvtag_t tag;
    srt_t *old_srt = 0, *nxt_srt = 0;
    double timestamp, offset, clear_timestamp = 0;
    int has_audio, has_video;
    FILE* flv = flv_open_read(argv[1]);
    int fd = open(argv[2], O_RDWR);
    FILE* out = flv_open_write(argv[3]);

    flvtag_init(&tag);

    if (!flv_read_header(flv, &has_audio, &has_video)) {
        fprintf(stderr, "%s is not an flv file\n", argv[1]);
        return EXIT_FAILURE;
    }

    flv_write_header(out, has_audio, has_video);

    fprintf(stderr, "Reading flv from %s\n", argv[1]);
    fprintf(stderr, "Reading captons from %s\n", argv[2]);
    fprintf(stderr, "Writing flv to %s\n", argv[3]);

    while (flv_read_tag(flv, &tag)) {

        srt_t* new_srt = srt_from_fd(fd);
        timestamp = flvtag_pts_seconds(&tag);

        if (new_srt) {
            fprintf(stderr, "Loaded new SRT at time %f\n", timestamp);
            srt_free(old_srt);
            old_srt = new_srt;
            nxt_srt = new_srt;
            offset = timestamp;
            clear_timestamp = timestamp;
        }

        if (flvtag_avcpackettype_nalu == flvtag_avcpackettype(&tag)) {
            if (nxt_srt && (offset + nxt_srt->timestamp) <= timestamp) {
                fprintf(stderr, "T: %0.02f (%0.02fs):\n%s\n", (offset + nxt_srt->timestamp), nxt_srt->duration, srt_data(nxt_srt));
                clear_timestamp = (offset + nxt_srt->timestamp) + nxt_srt->duration;
                flvtag_addcaption_text(&tag, srt_data(nxt_srt));
                nxt_srt = nxt_srt->next;
            } else if (0 <= clear_timestamp && clear_timestamp <= timestamp) {
                fprintf(stderr, "T: %0.02f: [CAPTIONS CLEARED]\n", timestamp);
                flvtag_addcaption_text(&tag, NULL);
                clear_timestamp = -1;
            }
        }

        flv_write_tag(out, &tag);
    }

    srt_free(old_srt);
    flvtag_free(&tag);
    flv_close(flv);
    flv_close(out);
    return EXIT_SUCCESS;
}
