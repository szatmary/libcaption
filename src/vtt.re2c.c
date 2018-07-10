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
#include "vtt.h"
#include <stdio.h>
/*!re2c
        re2c:define:YYCTYPE = utf8_codepoint_t;
        re2c:yyfill:enable = 0;
        re2c:flags:tags = 1;

        sp = [ \t];
        eol = "\r\n" | "\r" | "\n\r" | "\n";
        eolx2 = "\r\r" | "\n\n" | "\r\n\r\n" | "\n\r\n\r";
        blank_line = sp* eol;
        line_of_text = eol? [^\r\n\x00]+;
        identifier = [^\r\n];
        timestamp = [0-9]+ ":" [0-9][0-9] ":" [0-9][0-9] [,\.] [0-9]+;
        // attributes
        attribute = [^ :]+ ":" [^ ];
        cue_attribute = sp+ attribute;
        regin_attribute =  attribute eol;
        cue_attr_list = attribute+;
        regin_attr_list = regin_attribute eol;
    */

/*!stags:re2c format = 'const utf8_codepoint_t *@@;';*/

void vtt_attribute_ctor(vtt_attribute_t* attribute)
{
    memset(attribute, 0, sizeof(vtt_attribute_t));
}

void vtt_attribute_dtor(vtt_attribute_t* attribute)
{
    if (attribute->key) {
        free(attribute->key);
    }

    if (attribute->val) {
        free(attribute->val);
    }
}

void vtt_ctor(vtt_t* vtt)
{
    vtt->type = VTT_NOTE;
    vtt->timestamp = 0.0;
    vtt->duration = -1.0;
    vtt->attributes = 0;
    vtt->identifier = 0;
    vtt->payload = 0;
}

void vtt_dtor(vtt_t* vtt)
{
    if (vtt->payload) {
        free(vtt->payload);
    }

    if (vtt->identifier) {
        free(vtt->identifier);
    }

    if (vtt->attributes) {
        vtt_attribute_dtor(vtt->attributes);
    }
}

#define VTTTIME2SECONDS(HH, MM, SS, MS) ((HH * 3600.0) + (MM * 60.0) + SS + (MS / 1000.0))
double vtt_parse_timestamp(const utf8_codepoint_t* line)
{
    int hh, mm, ss, ms;
    if (sscanf((const char*)line, "%d:%2d:%2d%*1[,.]%d", &hh, &mm, &ss, &ms) == 4) {
        return VTTTIME2SECONDS(hh, mm, ss, ms);
    }
    if (sscanf((const char*)line, "%d:%2d%*1[,.]%d", &mm, &ss, &ms) == 3) {
        return VTTTIME2SECONDS(0.0, mm, ss, ms);
    }
    return -1.0;
}

vtt_attribute_vector_t* vtt_parse_attributes(const utf8_codepoint_t* begin, const utf8_codepoint_t* end)
{
    vtt_attribute_vector_t* attr_vec = vtt_attribute_vector_new();
    const utf8_codepoint_t *YYMARKER = 0, *YYCURSOR = begin;
    const utf8_codepoint_t *a, *b, *c, *d, *e, *f, *g, *h;
    while (YYCURSOR < end) {
        /*!re2c
        @a [^ :]+ @b ":" @c [^ ] @d [ \t\r\n] {
        vtt_attribute_t* attr = vtt_attribute_vector_push_back(&attr_vec);
        attr->key = utf8_string_copy(a,b);
        attr->val = utf8_string_copy(c,d);
    }
    */
    }
    return attr_vec;
}

const utf8_codepoint_t* vtt_find_attribute(vtt_attribute_vector_t* vtt, const utf8_codepoint_t* key)
{
    for (size_t i = 0; i < vtt_attribute_vector_count(&vtt); ++i) {
        vtt_attribute_t* attr = vtt_attribute_vector_at(&vtt, i);
        if (0 == strcmp((const char*)key, (const char*)attr->key)) {
            return attr->val;
        }
    }

    return 0;
}

vtt_vector_t* vtt_parse(const utf8_codepoint_t* str)
{
    vtt_vector_t* vtt_vec = vtt_vector_new();
    const utf8_codepoint_t *YYMARKER = 0, *YYCURSOR = str;
    const utf8_codepoint_t *a, *b, *c, *d, *e, *f, *g, *h;
    for (;;) {
        /*!re2c
        * { return vtt_vec; }
        '\x00' { return vtt_vec; }
        blank_line+ { continue; }

        "STYLE" eol @a line_of_text* @b eolx2 {
            vtt_t *vtt = vtt_vector_push_back(&vtt_vec);
            vtt->type = VTT_STYLE;
            vtt->payload = utf8_string_copy(a,b);
            continue;
        }

        "REGON" eol @a line_of_text* @b eolx2 {
            vtt_t *vtt = vtt_vector_push_back(&vtt_vec);
            vtt->type = VTT_REGION;
            // TODO parse atribues
            vtt->attributes = vtt_parse_attributes(a, b);
            vtt->payload = utf8_string_copy(a, b);
            continue;
        }

        // we want to keep a leading newline if present to accurately reproduce later
        "NOTE" @a line_of_text* @b eolx2 {
            vtt_t *vtt = vtt_vector_push_back(&vtt_vec);
            vtt->type = VTT_NOTE;
            vtt->payload = utf8_string_copy(a,b);
            continue;
        }

        @a identifier? @b eol 
        @c timestamp sp+  "-->" sp+ @d timestamp
        @e cue_attribute* @f eol @g line_of_text* @h eolx2 {
            vtt_t *vtt = vtt_vector_push_back(&vtt_vec);
            vtt->type = VTT_CUE;
            vtt->identifier =  utf8_string_copy(a,b);
            vtt->timestamp = vtt_parse_timestamp(c);
            double end_time = vtt_parse_timestamp(d);
            vtt->duration = end_time - vtt->timestamp;
            vtt->attributes = vtt_parse_attributes(e, f);
            vtt->payload = utf8_string_copy(g,h);
            continue;
        }
    */
    }
error:
    vtt_vector_del(&vtt_vec);
    return 0;
}

int vtt_cue_to_caption_frame(vtt_t* cue, caption_frame_t* frame)
{
    return caption_frame_from_text(frame, cue->payload);
}

libcaption_stauts_t vtt_cue_from_caption_frame(caption_frame_t* frame, vtt_vector_t* vtt)
{
    vtt_t* cue = vtt_vector_back(&vtt);
    if (cue && 0 >= cue->duration) {
        cue->duration = frame->timestamp - cue->timestamp;
    }

    // CRLF per row, plus an extra at the end
    cue = vtt_vector_push_back(&vtt);
    cue->type = VTT_CUE;
    cue->payload = malloc(2 + CAPTION_FRAME_TEXT_BYTES);
    caption_frame_to_text(frame, cue->payload);
    cue->timestamp = frame->timestamp;
    return LIBCAPTION_OK;
}

static inline void vtt_crack_time(double tt, int* hh, int* mm, int* ss, int* ms)
{
    (*ms) = (int)((int64_t)(tt * 1000) % 1000);
    (*ss) = (int)((int64_t)(tt) % 60);
    (*mm) = (int)((int64_t)(tt / (60)) % 60);
    (*hh) = (int)((int64_t)(tt / (60 * 60)));
}

static void _dump(vtt_vector_t* vtt, int srt_mode)
{
    if (!srt_mode) {
        printf("WEBVTT\r\n\r\n");
    }

    size_t cue_count = 0;
    for (size_t i = 0; i < vtt_vector_count(&vtt); ++i) {
        vtt_t* block = vtt_vector_at(&vtt, i);
        switch (block->type) {
        case VTT_REGION:
            if (!srt_mode) {
                printf("REGION\r\n");
                for (size_t i = 0; i < vtt_attribute_count(&block->attributes); ++i) {
                    vtt_attribute_t* attr = vtt_attribute_at(&block->attributes, i);
                    printf("%s:%s\r\n", attr->key, attr->val);
                }
                printf("\r\n");
            }
            break;
        case VTT_STYLE:
            if (!srt_mode) {
                printf("STYLE\r\n%s\r\n\r\n", block->payload);
            }
            break;
        case VTT_NOTE:
            if (!srt_mode) {
                printf("NOTE\r\n%s\r\n\r\n", block->payload);
            }
            break;

        case VTT_CUE: {
            int hh1, hh2, mm1, mm2, ss1, ss2, ms1, ms2;
            vtt_crack_time(block->timestamp, &hh1, &mm1, &ss1, &ms1);
            vtt_crack_time(block->timestamp + block->duration, &hh2, &mm2, &ss2, &ms2);
            ++cue_count;

            if (srt_mode) {
                printf("%u\r\n", (unsigned)cue_count);
            } else if (block->identifier) {
                printf("%s\r\n", block->identifier);
            }

            printf("%02d:%02d:%02d.%03d --> %02d:%02d:%02d.%03d",
                hh1, mm1, ss1, ms1, hh2, mm2, ss2, ms2);

            if (!srt_mode && block->attributes) {
                for (size_t i = 0; i = vtt_attribute_count(&block->attributes); ++i) {
                    vtt_attribute_t* attr = vtt_attribute_at(&block->attributes, i);
                    printf(" %s:%s", attr->key, attr->val);
                }
                printf("\r\n");
            }

            if (srt_mode) {
                // TODO print payload without html like stuff
                printf("\r\n%s\r\n\r\n", block->payload);
            } else {
                printf("\r\n%s\r\n\r\n", block->payload);
            }
        } break;
        }; // switch
    }
}

void vtt_dump(vtt_vector_t* vtt) { _dump(vtt, 0); }
void srt_dump(vtt_vector_t* vtt) { _dump(vtt, 1); }
