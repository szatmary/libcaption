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
#include "caption.h"
#include "scc.h"
#include "srt.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    int i;
    scc_t* scc = NULL;
    size_t scc_size = 0;
    caption_frame_t frame;
    srt_t *srt = 0, *head = 0;
    utf8_char_t* scc_data_ptr = utf8_load_text_file(argv[1], &scc_size);
    utf8_char_t* scc_data = scc_data_ptr;

    caption_frame_init(&frame);
    scc_data += scc_to_608(&scc, scc_data);

    while (scc->cc_size) {
        for (i = 0; i < scc->cc_size; ++i) {
            // eia608_dump (scc->cc_data[i]);

            if (LIBCAPTION_READY == caption_frame_decode(&frame, scc->cc_data[i], scc->timestamp)) {
                srt = srt_from_caption_frame(&frame, srt, &head);
            }
        }

        scc_data += scc_to_608(&scc, scc_data);
    }

    srt_dump(head);
    srt_free(head);
    free(scc_data_ptr);
    return EXIT_SUCCESS;
}
