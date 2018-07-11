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
#include "utf8.h"
#include <stdio.h>

int main(int argc, const char** argv)
{
    size_t size = 0;
    caption_frame_t frame;
    utf8_codepoint_t* text = utf8_load_text_file(argv[1], &size);
    scc_vector_t* scc_vec = scc_parse(text);
    free(text);

    caption_frame_ctor(&frame);
    for (size_t i = 0; i < scc_vector_count(&scc_vec); ++i) {
        scc_t* scc = scc_vector_at(&scc_vec, i);
        fprintf(stderr, "Timestamp: %f\n", scc->timestamp);
        for (size_t j = 0; j < uint16_vector_count(&scc->cc_data); ++j) {
            uint16_t* cc_data = uint16_vector_at(&scc->cc_data, j);
            eia608_dump(*cc_data);
            if (LIBCAPTION_READY == caption_frame_decode(&frame, *cc_data, scc->timestamp)) {
                caption_frame_dump(&frame);
            }
        }
    }

    scc_vector_clear(&scc_vec);
}