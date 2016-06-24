/**********************************************************************************************/
/* Copyright 2016-2016 Twitch Interactive, Inc. or its affiliates. All Rights Reserved.       */
/*                                                                                            */
/* Licensed under the Apache License, Version 2.0 (the "License"). You may not use this file  */
/* except in compliance with the License. A copy of the License is located at                 */
/*                                                                                            */
/*     http://aws.amazon.com/apache2.0/                                                       */
/*                                                                                            */
/* or in the "license" file accompanying this file. This file is distributed on an "AS IS"    */
/* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the    */
/* License for the specific language governing permissions and limitations under the License. */
/**********************************************************************************************/

#ifndef LIBCAPTION_SRT_H
#define LIBCAPTION_SRT_H

#include "eia608.h"
#include "caption.h"

typedef struct _srt_t {
    struct _srt_t* next;
    double str_pts;
    double end_pts;
    size_t aloc;
} srt_t;


// returns the head of teh link list. must bee freed when done
/*! \brief
    \param
*/
srt_t* srt_parse (const utf8_char_t* data, size_t size);
/*! \brief
    \param
*/
void srt_free (srt_t* srt);

/*! \brief
    \param
*/
static inline srt_t* srt_next (srt_t* srt) { return srt->next; }
/*! \brief
    \param
*/
static inline utf8_char_t* srt_data (srt_t* srt) { return (utf8_char_t*) (srt) + sizeof (srt_t); }

// This only converts teh surrent SRT, It does not walk the list
/*! \brief
    \param
*/
int srt_to_caption_frame (srt_t* srt, caption_frame_t* frame);

// returns teh new srt. Head is not tracher internally.
/*! \brief
    \param
*/
srt_t* srt_from_caption_frame (caption_frame_t* frame, srt_t* prev);
/*! \brief
    \param
*/
void srt_dump (srt_t* srt);
/*! \brief
    \param
*/
void vtt_dump (srt_t* srt);

#endif
