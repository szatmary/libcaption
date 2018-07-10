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
#ifndef LIBCAPTION_MPEG_H
#define LIBCAPTION_MPEG_H
#ifdef __cplusplus
extern "C" {
#endif

#include "caption.h"
#include "cea708.h"
#include "scc.h"
#include <float.h>
#include <stddef.h>
////////////////////////////////////////////////////////////////////////////////
// TODO prefix with lib caption!
#define STREAM_TYPE_H262 0x02
#define STREAM_TYPE_H264 0x1B
#define STREAM_TYPE_H265 0x24
#define H262_SEI_PACKET 0xB2
#define H264_SEI_PACKET 0x06
#define H265_SEI_PACKET 0x27 // There is also 0x28
#define MAX_NALU_SIZE (6 * 1024 * 1024)
#define MAX_REFRENCE_FRAMES 64

typedef struct {
    double dts, cts;
    libcaption_stauts_t status;
    uint8_vector_t* buffer;
    cea708_vector_t* cea708;
} mpeg_bitstream_t;

mpeg_bitstream_t* mpeg_bitstream_new();
void mpeg_bitstream_del(mpeg_bitstream_t* bs);
////////////////////////////////////////////////////////////////////////////////
// TODO make convenience functions for flv/mp4
/*! \brief
    \param
*/
size_t mpeg_bitstream_parse(mpeg_bitstream_t* bs, caption_frame_t* frame, const uint8_t* data, size_t size, unsigned stream_type, double dts, double cts);
/*! \brief
    \param
*/
static inline libcaption_stauts_t mpeg_bitstream_status(mpeg_bitstream_t* bs) { return bs->status; }
/*! \brief
        Flushes latent packets caused by out or order frames.
        Returns number of latent frames remaining, 0 when complete;
    \param
*/
size_t mpeg_bitstream_flush(mpeg_bitstream_t* bs, caption_frame_t* frame);
////////////////////////////////////////////////////////////////////////////////
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
typedef struct _sei_message_t {
    sei_msgtype_t type;
    uint8_vector_t* payload;
} sei_message_t;

void sei_message_ctor(sei_message_t* msg);
void sei_message_dtor(sei_message_t* msg);
MAKE_VECTOR(sei_message_t, sei_message, sei_message_ctor, sei_message_dtor, 0);

typedef struct {
    double timestamp;
    sei_message_vector_t* messages;
} sei_t;

/*! \brief
    \param
*/
void sei_ctor(sei_t* sei);
void sei_dtor(sei_t* sei);
/*! \brief
    \param
*/
void sei_cat(sei_t* to, sei_t* from, int itu_t_t35);
/*! \brief
    \param
*/
libcaption_stauts_t sei_parse(sei_t* sei, const uint8_t* data, size_t size);
/*! \brief
    \param
*/
////////////////////////////////////////////////////////////////////////////////
/*! \brief
    \param
*/
size_t sei_render_size(sei_t* sei);
/*! \brief
    \param
*/
size_t sei_render(sei_t* sei, uint8_t* data);
/*! \brief
    \param
*/
void sei_dump(sei_t* sei);
////////////////////////////////////////////////////////////////////////////////
/*! \brief
    \param
*/
libcaption_stauts_t sei_from_scc(sei_t* sei, scc_t* scc);
/*! \brief
    \param
*/
libcaption_stauts_t sei_from_caption_frame(sei_t* sei, caption_frame_t* frame);
/*! \brief
    \param
*/
libcaption_stauts_t sei_from_caption_clear(sei_t* sei);
/*! \brief
    \param
*/
libcaption_stauts_t sei_to_caption_frame(sei_t* sei, caption_frame_t* frame);
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif
#endif
