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

scc_vector_t* scc_parse(const utf8_codepoint_t* str)
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

    // Scenarist_SCC V1.0

    // 00:00:22:10	9420 94f2 97a2 d9ef 75a7 f2e5 2061 20ea e5f2 6b2c 2054 68ef 6dae 942c 8080 8080 942f

    /*!stags:re2c format = 'const utf8_codepoint_t *@@;';*/
    const utf8_codepoint_t *YYMARKER = 0, *YYCURSOR = str;
    const utf8_codepoint_t *a, *b, *c, *d, *e, *f, *g, *h;
    /*!re2c
    * {return 0;}
    */
}
