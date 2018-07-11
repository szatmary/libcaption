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
    re2c:define:YYCTYPE = uint8_t;
    re2c:yyfill:enable = 0;
    re2c:flags:tags = 1;

    sp = [ \t];
    eol = ("\r" | "\r\n" | "\n" | "\n\r");
    eol2x = ("\r\r" | "\r\n\r\n" | "\n\n" | "\n\r\n\r");
    timestamp = [0-9]+ ":" [0-9][0-9] ":" [0-9][0-9]  ":" [0-9]+;
    ccdata = ([0-9a-fA_F] {4});
*/

/*!stags:re2c format = 'const uint8_t *@@;';*/

void scc_ctor(scc_t* scc)
{
    scc->timestamp = 0.0;
    scc->cc_data = 0;
}

void scc_dtor(scc_t* scc)
{
    uint16_vector_del(&scc->cc_data);
}

uint16_vector_t scc_parse_ccdata(const uint8_t* str)
{
    const uint8_t* a;
    unsigned int cc_data = 0;
    const uint8_t *YYMARKER = 0, *YYCURSOR = (const uint8_t*)str;
    uint16_vector_t cc_vec = uint16_vector_new();
    for (;;) {
        /*!re2c
    * { return cc_vec; }
    sp+ @a ccdata {
        if( 1 == sscanf((const char*)a, "%04x", &cc_data)) {
            *uint16_vector_push_back(&cc_vec) = cc_data;
            continue;
        } else {
            uint16_vector_del(&cc_vec);
            return 0;
        }
    }
    */
    }
}

scc_vector_t scc_parse(const utf8_codepoint_t* str)
{
    if (!str) {
        return 0;
    }
    // Scenarist_SCC V1.0
    //
    // 00:00:22:10	9420 94f2 97a2 d9ef 75a7 f2e5 2061 20ea e5f2 6b2c 2054 68ef 6dae 942c 8080 8080 942f

    const uint8_t *a, *b;
    const uint8_t *YYMARKER = 0, *YYCURSOR = (const uint8_t*)str;
    for (;;) {
        /*!re2c
    * { goto error;; }
    "Scenarist_SCC V1.0" sp* eol2x { break; }
    */
    }

    scc_vector_t scc_vec = scc_vector_new();
    for (;;) {
        /*!re2c
    * { return scc_vec; }
    sp* eol { continue; }
    @a timestamp @b (sp+ ccdata)+ eol2x {
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
