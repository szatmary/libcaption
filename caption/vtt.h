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

// REGION represents a WebVTT Region
typedef struct _region_t {
    struct _region_t* next;
    size_t id_size;
    char *region_id;
    size_t block_size;
    char *block_text;
} vtt_region_t;

// Embedded CSS stylesheet
typedef struct _style_t {
    struct _style_t* next;
    size_t block_size;
    char *block_text;
} vtt_style_t;

// CUE represents a block of caption text
typedef struct _cue_t {
    struct _cue_t* next;
    double timestamp;
    double duration; // -1.0 for no duration
    char *cue_settings;
    char *cue_id;
    vtt_region_t *region;
    size_t text_size;
    char *block_text;
} vtt_cue_t;

// VTT files are a collection of REGION, STYLE and CUE blocks.
// XXX: Comments (NOTE blocks) are ignored
typedef struct _vtt_t {
    vtt_region_t* region_head;
    vtt_region_t* region_tail;
    vtt_style_t* style_head;
    vtt_style_t* style_tail;
    vtt_cue_t* cue_head;
    vtt_cue_t* cue_tail;
} vtt_t;

/*! \brief
    \param
*/
vtt_t* vtt_new();
/*! \brief
    \param
*/
void vtt_free(vtt_t* vtt);

/*! \brief
    \param
*/
vtt_region_t *vtt_region_new(vtt_t* vtt, const utf8_char_t* data, size_t size);
/*! \brief
    \param
*/
void vtt_region_set_id(vtt_region_t* region, const utf8_char_t* data, size_t size);
/*! \brief
    \param
*/
vtt_region_t* vtt_region_free_head(vtt_region_t* region);

/*! \brief
    \param
*/
vtt_style_t *vtt_style_new(vtt_t* vtt, const utf8_char_t* data, size_t size);
/*! \brief
    \param
*/
vtt_style_t* vtt_style_free_head(vtt_style_t* style);
/*! \brief
    \param
*/
vtt_cue_t* vtt_cue_new(vtt_t* vtt, const utf8_char_t* data, size_t size);
/*! \brief
    \param
*/
vtt_cue_t* vtt_cue_free_head(vtt_cue_t* head);

// returns a vtt_t, containing linked lists of blocks. must be freed when done
/*! \brief
    \param
*/
vtt_t* vtt_parse(const utf8_char_t* data, size_t size);

/*! \brief
    \param
*/
static inline vtt_cue_t* vtt_cue_next(vtt_cue_t* cue) { return cue->next; }

/*! \brief
    \param
*/
static inline utf8_char_t* vtt_region_data(vtt_region_t* region) { return (utf8_char_t*)(region) + sizeof(vtt_region_t); }
/*! \brief
    \param
*/
static inline utf8_char_t* vtt_style_data(vtt_style_t* style) { return (utf8_char_t*)(style) + sizeof(vtt_style_t); }
/*! \brief
    \param
*/
static inline utf8_char_t* vtt_cue_data(vtt_cue_t* cue) { return (utf8_char_t*)(cue) + sizeof(vtt_cue_t); }

// This only converts the current CUE, it does not walk the list
/*! \brief
    \param
*/
int vtt_cue_to_caption_frame(vtt_cue_t* cue, caption_frame_t* frame);

// returns the new cue. Head is not tracher internally.
/*! \brief
    \param
*/
vtt_cue_t* vtt_cue_from_caption_frame(caption_frame_t* frame, vtt_t *vtt);
/*! \brief
    \param
*/
void vtt_dump(vtt_t* vtt);

#ifdef __cplusplus
}
#endif
#endif
