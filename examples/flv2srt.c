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

#include "flv.h"
#include "srt.h"
#include "sei.h"

#define LENGTH_SIZE 4
int main (int argc, char** argv)
{
    const char* path = argv[1];

    sei_t sei;
    flvtag_t tag;
    srt_t* srt = 0, *head = 0;
    sei_message_t* msg;
    int i, has_audio, has_video;
    caption_frame_t frame;

    sei_init (&sei);
    flvtag_init (&tag);
    caption_frame_init (&frame);

    FILE* flv = flv_open_read (path);

    if (!flv_read_header (flv,&has_audio,&has_video)) {
        printf ("Not an flv file\n");
    }

    while (flv_read_tag (flv,&tag)) {

        if (flvtag_avcpackettype_nalu == flvtag_avcpackettype (&tag)) {
            ssize_t  size = flvtag_payload_size (&tag);
            uint8_t* data = flvtag_payload_data (&tag);

            while (0<size) {
                ssize_t  nalu_size = (data[0]<<24) | (data[1]<<16) | (data[2]<<8) | data[3];
                uint8_t* nalu_data = &data[4];
                uint8_t  nalu_type = nalu_data[0]&0x1F;
                data += nalu_size + LENGTH_SIZE;
                size -= nalu_size + LENGTH_SIZE;

                if (6 == nalu_type) {
                    sei_parse_nalu (&sei, nalu_data, nalu_size, flvtag_pts (&tag), flvtag_dts (&tag));

                    while (0 != (msg = sei_message_take_head (&sei))) {
                        cea708_t cea708;

                        if (sei_type_user_data_registered_itu_t_t35 == sei_message_type (msg)) {
                            sei_decode_cea708 (msg,&cea708);
                            int count = cea708_cc_count (&cea708.user_data);
                            // cea708_dump (&cea708);

                            for (i = 0 ; i < count ; ++i) {
                                cea708_cc_type_t type; int valid;
                                uint16_t cc_data = cea708_cc_data (&cea708.user_data, i, &valid, &type);

                                if (valid && (cc_type_ntsc_cc_field_1 == type || cc_type_ntsc_cc_field_2 == type)) {
                                    if (LIBCAPTION_READY == caption_frame_decode (&frame,cc_data,sei_message_pts (msg))) {
                                        // utf8_char_t buff[CAPTION_FRAME_DUMP_BUF_SIZE];
                                        // size_t size = caption_frame_dump (&frame, buff);
                                        // fprintf (stderr,"%s\n", buff);
                                        srt = srt_from_caption_frame (&frame,srt);

                                        if (!head) {head = srt;}
                                    }
                                }
                            }
                        }

                        sei_message_free (msg);
                    }
                }
            }
        }
    }

    srt_dump (head);
    srt_free (head);
    sei_free (&sei);

    return 1;
}
