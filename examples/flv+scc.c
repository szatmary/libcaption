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
#include "utf8.h"




int main (int argc, char** argv)
{

    size_t scc_file_size = 0;
    utf8_char_t* scc_file_data = utf8_load_text_file (argv[2], &scc_file_size);
    utf8_char_t* scc_file_curr = scc_file_data;

    do {

        size_t line_length = utf8_line_length (scc_file_curr);;
        scc_file_curr += line_length;
    } while (scc_file_curr < scc_file_data);


    // flvtag_t tag;
    // double clear_timestamp = 0;
    // FILE* flv = flv_open_read (argv[1]);
    // FILE* out = flv_open_write (argv[3]);
    //
    // int has_audio, has_video;
    // flvtag_init (&tag);
    //
    // if (!flv_read_header (flv,&has_audio,&has_video)) {
    //     fprintf (stderr,"%s is not an flv file\n", argv[1]);
    //     return EXIT_FAILURE;
    // }
    //
    // flv_write_header (out,has_audio,has_video);
    //
    // while (flv_read_tag (flv,&tag)) {
    //     // TODO handle B frames better
    //     double timestamp = flvtag_pts_seconds (&tag);
    //
    //     if (srt && timestamp >= srt->timestamp && flvtag_avcpackettype_nalu == flvtag_avcpackettype (&tag)) {
    //         fprintf (stderr,"%0.02f, %0.02f: %s\n", srt->timestamp, srt->duration, srt_data (srt));
    //         flvtag_addcaption (&tag, srt_data (srt));
    //         srt = srt->next;
    //         clear_timestamp = srt->timestamp + srt->duration;
    //     } else if (timestamp >= clear_timestamp) {
    //         fprintf (stderr,"%0.02f: [CAPTIONS CLEARED]\n", timestamp);
    //         flvtag_addcaption (&tag, NULL);
    //         clear_timestamp = timestamp + CLEAR_TIMEOUT;
    //     }
    //
    //     flv_write_tag (out,&tag);
    //     // Write tag out here
    // }

    return EXIT_SUCCESS;
}
