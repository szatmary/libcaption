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

// timestamp and duration are in seconds
typedef struct _vtt_t {
    struct _vtt_t* next;
    double timestamp;
    double duration;
    size_t aloc;
    char *cue_settings;
} vtt_t;

/*! \brief
    \param
*/
vtt_t* vtt_new(const utf8_char_t* data, size_t size, double timestamp, vtt_t* prev, vtt_t** head);
/*! \brief
    \param
*/
vtt_t* vtt_free_head(vtt_t* head);
// returns the head of the link list. must bee freed when done
/*! \brief
    \param
*/
vtt_t* vtt_parse(const utf8_char_t* data, size_t size);
/*! \brief
    \param
*/
void vtt_free(vtt_t* vtt);

/*! \brief
    \param
*/
static inline vtt_t* vtt_next(vtt_t* vtt) { return vtt->next; }
/*! \brief
    \param
*/
static inline utf8_char_t* vtt_data(vtt_t* vtt) { return (utf8_char_t*)(vtt) + sizeof(vtt_t); }
// This only converts teh surrent VTT, It does not walk the list
/*! \brief
    \param
*/
int vtt_to_caption_frame(vtt_t* vtt, caption_frame_t* frame);

// returns the new vtt. Head is not tracher internally.
/*! \brief
    \param
*/
vtt_t* vtt_from_caption_frame(caption_frame_t* frame, vtt_t* prev, vtt_t** head);
/*! \brief
    \param
*/
void vtt_dump(vtt_t* vtt);

#ifdef __cplusplus
}
#endif
#endif
