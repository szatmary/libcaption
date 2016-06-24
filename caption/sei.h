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


#ifndef LIBCAPTION_sei_H
#define LIBCAPTION_sei_H
#include "cea708.h"
#include "caption.h"
#include <float.h>

typedef struct _sei_message_t sei_message_t;

typedef enum {
    sei_type_buffering_period = 0,
    sei_type_pic_timing = 1,
    sei_type_pan_scan_rect = 2,
    sei_type_filler_payload = 3,
    sei_type_user_data_registered_itu_t_t35 = 4,
    sei_type_user_data_unregistered = 5,
    sei_type_recovery_point = 6,
    sei_type_dec_ref_pic_marking_repetition = 7,
    sei_type_spare_pic = 8,
    sei_type_scene_info = 9,
    sei_type_sub_seq_info = 10,
    sei_type_sub_seq_layer_characteristics = 11,
    sei_type_sub_seq_characteristics = 12,
    sei_type_full_frame_freeze = 13,
    sei_type_full_frame_freeze_release = 14,
    sei_type_full_frame_snapshot = 15,
    sei_type_progressive_refinement_segment_start = 16,
    sei_type_progressive_refinement_segment_end = 17,
    sei_type_motion_constrained_slice_group_set = 18,
    sei_type_film_grain_characteristics = 19,
    sei_type_deblocking_filter_display_preference = 20,
    sei_type_stereo_video_info = 21,
} sei_msgtype_t;
////////////////////////////////////////////////////////////////////////////////
typedef struct  {
    double dts;
    sei_message_t* head;
} sei_t;

/*! \brief
    \param
*/
void sei_init (sei_t* sei);
/*! \brief
    \param
*/
void sei_free (sei_t* sei);
/*! \brief
    \param
*/
int sei_parse_nalu (sei_t* sei, const uint8_t* data, size_t size, double pts, double dts);
/*! \brief
    \param
*/
static inline int sei_finish (sei_t* sei) { return sei_parse_nalu (sei,0,0,0.0,DBL_MAX); }
/*! \brief
    \param
*/
static inline sei_message_t* sei_message_head (sei_t* sei) { return sei->head; }
/*! \brief
    \param
*/
/*! \brief
    \param

    Returns the first message in pts order. message MUST be freed!
*/
sei_message_t* sei_message_take_head (sei_t* sei);

/*! \brief
    \param
*/
sei_message_t* sei_message_next (sei_message_t* msg);
/*! \brief
    \param
*/
sei_msgtype_t sei_message_type (sei_message_t* msg);
/*! \brief
    \param
*/
double sei_message_pts (sei_message_t* msg);
/*! \brief
    \param
*/
size_t sei_message_size (sei_message_t* msg);
/*! \brief
    \param
*/
uint8_t* sei_message_data (sei_message_t* msg);
/*! \brief
    \param
*/
sei_message_t* sei_message_new (sei_msgtype_t type, uint8_t* data, size_t size, double pts);
/*! \brief
    \param
*/
static inline sei_message_t* sei_message_copy (sei_message_t* msg)
{
    return sei_message_new (sei_message_type (msg), sei_message_data (msg), sei_message_size (msg), sei_message_pts (msg));
}
/**
Free message and all accoiated data. Messaged added to sei_t by using sei_append_message MUST NOT be freed
These messages will be freed by calling sei_free()
*/
/*! \brief
    \param
*/
void  sei_message_free (sei_message_t* msg);
////////////////////////////////////////////////////////////////////////////////
/*! \brief
    \param
*/
static inline int sei_decode_cea708 (sei_message_t* msg, cea708_t* cea708)
{
    if (sei_type_user_data_registered_itu_t_t35 == sei_message_type (msg)) {
        return cea708_parse (sei_message_data (msg), sei_message_size (msg), cea708);
    } else {
        return 0;
    }
}
////////////////////////////////////////////////////////////////////////////////
/*! \brief
    \param
*/
size_t sei_render_size (sei_t* sei);
/*! \brief
    \param
*/
size_t sei_render (sei_t* sei, uint8_t* data);
/*! \brief
    \param
*/
void sei_dump (sei_t* sei);
/*! \brief
    \param
*/
void sei_dump_messages (sei_message_t* head);
////////////////////////////////////////////////////////////////////////////////
/*! \brief
    \param
*/
int sei_from_caption_frame (sei_t* sei, caption_frame_t* frame);
/*! \brief
    \param
*/
int sei_to_caption_frame (caption_frame_t* frame, sei_t* sei);
/*! \brief
    \param
*/
static inline int nalu_to_caption_frame (caption_frame_t* frame, const uint8_t* data, size_t size, double pts, double dts)
{
    sei_t sei;
    sei_init (&sei);
    sei_parse_nalu (&sei, data, size, pts, dts);
    sei_to_caption_frame (frame, &sei);
    sei_free (&sei);
    return 1;
}
////////////////////////////////////////////////////////////////////////////////

#endif
