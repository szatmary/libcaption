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

#include "srt.h"
#include <stdio.h>

srt_t* srt_new()
{
    return vtt_new();
}

void srt_del(srt_t* srt)
{
    vtt_del(srt);
}

void srt_dump(srt_t* srt)
{
    for (size_t i = 0; i < vtt_block_vector_count(&srt->cue); ++i) {
        vtt_block_t** block = vtt_block_vector_at(&srt->cue, i);
        int hh1, hh2, mm1, mm2, ss1, ss2, ms1, ms2;
        vtt_crack_time((*block)->timestamp, &hh1, &mm1, &ss1, &ms1);
        vtt_crack_time((*block)->timestamp + (*block)->duration, &hh2, &mm2, &ss2, &ms2);

        printf("%02d\r\n%d:%02d:%02d,%03d --> %02d:%02d:%02d,%03d\r\n%s\r\n", (int)i,
            hh1, mm1, ss1, ms1, hh2, mm2, ss2, ms2, vtt_block_data(*block));
    }
}

double _srt_parse_timestamp(utf8_codepoint_t* str) {
    return 0.0;
}

// TODO this will eventually parse vtt as well. But im keeping in srt for now
srt_t* srt_parse(const utf8_codepoint_t* str, size_t size)
{
    /*!re2c
        re2c:define:YYCTYPE = utf8_codepoint_t;
        re2c:yyfill:enable = 0;
        re2c:flags:tags = 1;

        ws = [ \t];
        eol = "\r\n" | "\r" | "\n\r" | "\n";
        eolx2 = "\r\r" | "\n\n" | "\r\n\r\n" | "\n\r\n\r";
        blank_line = ws* eol;
        line_of_text = eol? [^\r\n\x00]+;
        identifier = [1-9][0-9]*;
        timestamp = [0-9]+ ":" [0-9][0-9] ":" [0-9][0-9] [,\.] [0-9]+;
    */

        // eol_2x = "\r\r" | "\n\n" | "\r\n\r\n" | "\n\r\n\r";

    /*!stags:re2c format = 'const utf8_codepoint_t *@@;';*/
    srt_t *srt = srt_new();
    const utf8_codepoint_t *YYMARKER = 0, *YYCURSOR = str;
    const utf8_codepoint_t *a, *b, *c, *d, *e, *f, *g, *h;
    for(;;) {
    /*!re2c
        * { return srt; }
        '\x00' { return srt; }
        blank_line* @a identifier @b eol @c timestamp @d ws+  "-->" ws+ @e timestamp @f ws* eol @g line_of_text* @h eolx2 {

            vtt_block_t **cue = 0;
            cue = vtt_block_vector_push_back(&srt->cue);
            (*cue) = vtt_block_new(g, h - g, VTT_CUE);
            double end_time = vtt_parse_timestamp(e);
            (*cue)->timestamp = vtt_parse_timestamp(c);
            (*cue)->duration = end_time - (*cue)->timestamp;

            fprintf(stderr,"identifier: '%.*s'\n", (int)(b - a), a);
            fprintf(stderr,"timestamp:  %f\n", (*cue)->timestamp);
            fprintf(stderr,"duration:   %f\n", (*cue)->duration);
            fprintf(stderr,"text:       '%.*s'\n", (int)(h - g), g);

            continue;
         }
    */
    }

    return _vtt_parse(str, size, 1);
}
