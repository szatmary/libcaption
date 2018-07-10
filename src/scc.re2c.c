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

#include "scc.h"
#include <stdio.h>
/*!re2c
    re2c:define:YYCTYPE = utf8_codepoint_t;
    re2c:yyfill:enable = 0;
    re2c:flags:tags = 1;

    sp = [ \t];
    eol = "\x00" | "\r" | "\r\n" | "\n" | "\n\r";
    eol2x = "\r\r" | "\r\n\r\n" | "\n\n" | "\n\r\n\r";
    timestamp = [0-9]+ ":" [0-9][0-9] ":" [0-9][0-9]  ":" [0-9]+;
    ccdata = [0-9a-fA_F] {4};
    sp_ccdata = sp+ ccdata;
*/
/*!stags:re2c format = 'const utf8_codepoint_t *@@;';*/

uint16_vector_t* scc_parse_ccdata(const utf8_codepoint_t* str)
{
    const utf8_codepoint_t* a;
    const utf8_codepoint_t *YYMARKER = 0, *YYCURSOR = str;
    uint16_vector_t* cc_vec = uint16_vector_new();
    for (;;) {
        /*!re2c
    * { return cc_vec; }
    @a ccdata sp* {
        unsigned int cc_data = 0;
        if( 1 != sscanf((const char*)a, "%04x", &cc_data)) {
            uint16_vector_del(&cc_vec);
            return 0;
        }

        *uint16_vector_push_back(&cc_vec) = cc_data;
        continue;
    }
    */
    }
}

scc_vector_t* scc_parse(const utf8_codepoint_t* str)
{
    // Scenarist_SCC V1.0
    //
    // 00:00:22:10	9420 94f2 97a2 d9ef 75a7 f2e5 2061 20ea e5f2 6b2c 2054 68ef 6dae 942c 8080 8080 942f

    const utf8_codepoint_t *a, *b;
    const utf8_codepoint_t *YYMARKER = 0, *YYCURSOR = str;
    scc_vector_t* scc_vec = scc_vector_new();
    for (;;) {
        /*!re2c
    * { return 0; }
    "Scenarist_SCC V1.0" eol2x { continue; }
    @a timestamp @b ccdata sp_ccdata* eol2x {
        int v1 = 0, v2 = 0, hh = 0, mm = 0, ss = 0, ff = 0;
        if (4 != sscanf((const char*)a, "%2d:%2d:%2d%*1[:;]%2d", &hh, &mm, &ss, &ff)) { goto error; }
        scc_t *scc = scc_vector_push_back(&scc_vec);
        scc->timestamp = (hh * 3600.0) + (mm * 60.0) + ss + (ff / 29.97);
        scc->cc_data = scc_parse_ccdata(b);
        continue;
    }
    */
    }
error:
    scc_vector_del(&scc_vec);
    return 0;
}
