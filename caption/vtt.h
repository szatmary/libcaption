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
#ifndef LIBCAPTION_VTT_H
#define LIBCAPTION_VTT_H
#ifdef __cplusplus
extern "C" {
#endif

#include "caption.h"
#include "eia608.h"

enum VTT_BLOCK_TYPE {
    VTT_REGION = 0,
    VTT_STYLE = 1,
    VTT_NOTE = 2,
    VTT_CUE = 3
};

typedef struct  {
    utf8_codepoint_t *key;
    utf8_codepoint_t *val;
} vtt_attribute_t;
void vtt_attribute_ctor(vtt_attribute_t* attribute);
void vtt_attribute_dtor(vtt_attribute_t* attribute);
MAKE_VECTOR(vtt_attribute_t, vtt_attribute, vtt_attribute_ctor, vtt_attribute_dtor, 0);

// CUE represents a block of caption text
typedef struct {
    enum VTT_BLOCK_TYPE type;
    double timestamp;
    double duration; // -1.0 for no duration
    vtt_attribute_vector_t attributes;
    utf8_codepoint_t *identifier;
    utf8_codepoint_t *payload;
} vtt_t;

void vtt_ctor(vtt_t* vtt);
void vtt_dtor(vtt_t* vtt);
MAKE_VECTOR(vtt_t, vtt, vtt_ctor, vtt_dtor, 0);

vtt_vector_t vtt_parse(const utf8_codepoint_t* data);

// This only converts the current CUE, it does not walk the list
/*! \brief
    \param
*/
int vtt_cue_to_caption_frame(vtt_t* cue, caption_frame_t* frame);

/*! \brief
    \param
*/
libcaption_stauts_t vtt_cue_from_caption_frame(caption_frame_t* frame, vtt_vector_t vtt);
/*! \brief
    \param
*/
void vtt_dump(vtt_vector_t vtt);
void srt_dump(vtt_vector_t vtt);

#ifdef __cplusplus
}
#endif
#endif
