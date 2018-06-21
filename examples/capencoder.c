#include "caption.h"
#include "flv.h"
#include <stdio.h>
#include <string.h>
#include <x264.h>
#include <stdlib.h>

#define WIDTH (40 * 16)
#define HEIGHT (32 * 16)

int main(int argc, char** argv)
{
    // TODO animate this frame
    uint8_t Y[WIDTH * HEIGHT];
    uint8_t U[(WIDTH >> 1) * (HEIGHT >> 1)];
    uint8_t V[(WIDTH >> 1) * (HEIGHT >> 1)];
    cea708_t cea708[1024];

    // Twitch Purple
    memset(&Y[0], 86, WIDTH * HEIGHT);
    memset(&U[0], 171, (WIDTH >> 1) * (HEIGHT >> 1));
    memset(&V[0], 137, (WIDTH >> 1) * (HEIGHT >> 1));

    x264_t* x264;
    x264_param_t param;
    flvtag_t tag;
    FILE* flv = flv_open_write(argv[1]);
    x264_param_default_preset(&param, "fast", 0);
    param.i_width = WIDTH;
    param.i_height = HEIGHT;
    param.b_vfr_input = 0;
    param.i_timebase_num = 1;
    param.i_timebase_den = 1000;

    //Rate control:
    param.rc.i_rc_method = X264_RC_CRF;
    param.rc.f_rf_constant = 25;
    param.rc.f_rf_constant_max = 35;

    // Bitstream format
    param.b_aud = 0;
    param.b_annexb = 0;
    param.b_repeat_headers = 0;

    x264_param_apply_profile(&param, "main");
    x264 = x264_encoder_open(&param);
    {

        int i_nals;
        x264_nal_t* nals;
        uint8_t *sps_data = 0, *pps_data = 0;
        size_t sps_size = 0, pps_size = 0;
        x264_encoder_headers(x264, &nals, &i_nals);

        for (int i = 0; i < i_nals; ++i) {
            if (7 == nals[i].i_type && !sps_data) {
                sps_data = nals[i].p_payload + 4;
                sps_size = nals[i].i_payload - 4;
            } else if (8 == nals[i].i_type && !pps_data) {
                pps_data = nals[i].p_payload + 4;
                pps_size = nals[i].i_payload - 4;
            }
        }

        flv_write_header(flv, 0, 1);
        flvtag_avcsequenceheader(&tag, sps_data, sps_size, pps_data, pps_size);
        flv_write_tag(flv, &tag);
    }

    int cc = 0;

    { // create caption array
        for (int i = 0; i < 1024; ++i) {
            cea708_init(&cea708[i], 0.0);
        }

        // reset to blank
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_control_command(eia608_control_end_of_caption, 0));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_control_command(eia608_control_erase_display_memory, 0));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_control_command(eia608_control_erase_non_displayed_memory, 0));

        // Set rollup 2
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_control_command(eia608_control_resume_direct_captioning, 0));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_control_command(eia608_control_roll_up_2, 0));

        // Hello World
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("T", "h"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("e", " "));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("Q", "u"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("i", "c"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("k", " "));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("B", "r"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("o", "w"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("n", " "));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("F", "o"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("x", ""));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_control_command(eia608_control_carriage_return, 0));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_control_command(eia608_control_roll_up_2, 0));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_padding());
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("J", "u"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("m", "p"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("e", "d"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2(" ", "O"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("v", "e"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("r", " "));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("T", "h"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("e", " "));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("L", "a"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("z", "y"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_control_command(eia608_control_carriage_return, 0));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_control_command(eia608_control_roll_up_2, 0));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_padding());
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("D", "o"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("g", "s"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2(" ", "B"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("a", "c"));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_from_utf8_2("k", ""));
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_padding());
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_padding());
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_padding());
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_padding());
        cea708_add_cc_data(&cea708[cc++], 1, cc_type_ntsc_cc_field_1, eia608_padding());
    }

    x264_picture_t pic_in, pic_out;
    for (int j = 0;; ++j) {
        x264_picture_init(&pic_in);
        pic_in.img.plane[0] = Y;
        pic_in.img.plane[1] = U;
        pic_in.img.plane[2] = V;
        pic_in.img.i_stride[0] = WIDTH;
        pic_in.img.i_stride[1] = WIDTH >> 1;
        pic_in.img.i_stride[2] = WIDTH >> 1;
        pic_in.img.i_plane = 3;
        pic_in.img.i_csp = X264_CSP_I420;
        pic_in.i_pts = (j * param.i_timebase_den) / 15;
        // Caption
        pic_in.extra_sei.payloads = (x264_sei_payload_t*)malloc(sizeof(x264_sei_payload_t));
        pic_in.extra_sei.payloads->payload =  (uint8_t*)malloc(CEA708_MAX_SIZE);
        pic_in.extra_sei.payloads->payload_size = cea708_render(&cea708[j % cc], pic_in.extra_sei.payloads->payload, CEA708_MAX_SIZE);
        pic_in.extra_sei.payloads->payload_type = sei_type_user_data_registered_itu_t_t35;
        pic_in.extra_sei.num_payloads = 1;
        pic_in.extra_sei.sei_free = free;

        int i_nals;
        x264_nal_t* nals;

        if (0 < x264_encoder_encode(x264, &nals, &i_nals, &pic_in, &pic_out)) {
            // TODO negative dts handeling
            flvtag_initavc(&tag, pic_out.i_dts, pic_out.i_pts - pic_out.i_dts, pic_out.b_keyframe ? flvtag_frametype_keyframe : flvtag_frametype_interframe);
            for (int i = 0; i < i_nals; ++i) {
                // TODO write the whole frame in one go
                flvtag_avcwritenal(&tag, nals[i].p_payload + 4, nals[i].i_payload - 4);
            }
            flv_write_tag(flv, &tag);
        }
    }
}