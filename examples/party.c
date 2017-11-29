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
#include "avc.h"
#include "flv.h"
// #include "hexdump.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPTION_METHOD_SEI_708 0 // embedded 708
#define CAPTION_METHOD_AMF_708 1 // onCaptionInfo type = 708
#define CAPTION_METHOD_AMF_UTF8 2 // onCaptionInfo type = utf8
#define CAPTION_METHOD CAPTION_METHOD_SEI_708

// t is time in miliseconds
void get_kappaclock(double t, double p, double* x, double* y)
{
    static const double Tau = (2 * 3.14159265359);
    t *= Tau;
    (*x) = p * sin(t), (*y) = p * cos(t);
}

void get_kappaclock_json(double ts, char* str)
{
    double x1, x2, x3, y1, y2, y3;
    get_kappaclock(fmod(ts, 60000.0), 0.85, &x1, &y1);
    get_kappaclock(fmod(ts, 1000.0), 0.80, &x2, &y2);
    get_kappaclock(fmod(ts, 60.0), 0.75, &x3, &y3);

    sprintf(str, "{\"img\":[\"%s\",\"%s\",\"%s\"],\"x\":[%0.3f,%0.3f,%0.3f],\"y\":[%0.3f,%0.3f,%0.3f]}",
        "https://static-cdn.jtvnw.net/emoticons/v1/58127/1.0", "https://static-cdn.jtvnw.net/emoticons/v1/1/1.0",
        "https://static-cdn.jtvnw.net/emoticons/v1/25/1.0", x1, x2, x3, y1, y2, y3);
}

void get_dudes(char* str)
{
    sprintf(str, " %s%s %s(-_-)%s %s(-_-)%s %s(-_-)%s %s%s", EIA608_CHAR_EIGHTH_NOTE, EIA608_CHAR_EIGHTH_NOTE,
        !(rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT,
        !(rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_LEFT,
        !(rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT,
        !(rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_LEFT,
        !(rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT,
        !(rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_LEFT,
        EIA608_CHAR_EIGHTH_NOTE, EIA608_CHAR_EIGHTH_NOTE);
}

void write_amfcaptions_708(FILE* out, uint32_t timestamp, const char* text)
{
    sei_t sei;
    flvtag_t tag;
    sei_message_t* msg;
    caption_frame_t frame;
    sei_init(&sei);
    flvtag_init(&tag);
    caption_frame_init(&frame);
    caption_frame_from_text(&frame, text);
    sei_from_caption_frame(&sei, &frame);
    // caption_frame_dump (&frame);

    for (msg = sei_message_head(&sei); msg; msg = sei_message_next(msg), ++timestamp) {
        flvtag_amfcaption_708(&tag, timestamp, msg);
        flv_write_tag(out, &tag);
    }

    sei_free(&sei);
    flvtag_free(&tag);
}

void write_amfcaptions_utf8(FILE* out, uint32_t timestamp, const utf8_char_t* text)
{
    flvtag_t tag;
    flvtag_init(&tag);
    flvtag_amfcaption_utf8(&tag, timestamp, text);
    flv_write_tag(out, &tag);
    flvtag_free(&tag);
}

#define KAPPA_CLOCK_GUID "3ab3c3a1-0358-4c67-b11c-76257ddb3597"
int main(int argc, char** argv)
{
    flvtag_t tag;
    flvtag_t metatag;
    double nextClock = 0;
    uint32_t nextParty = 1000;
    int has_audio, has_video;
    FILE* flv = flv_open_read(argv[1]);
    FILE* out = flv_open_write(argv[2]);
    char partyDudes[64];
    char kappaClock[2048];

    flvtag_init(&tag);
    flvtag_init(&metatag);

    if (!flv_read_header(flv, &has_audio, &has_video)) {
        fprintf(stderr, "%s is not an flv file\n", argv[1]);
        return EXIT_FAILURE;
    }

    flv_write_header(out, has_audio, has_video);

    while (flv_read_tag(flv, &tag)) {

        uint32_t timestamp = flvtag_timestamp(&tag);
        while (nextClock < timestamp) {
            get_kappaclock_json(nextClock, kappaClock);
            flvtag_amftimedmetadata(&metatag, nextClock);
            flvtag_amftimedmetadata_append(&metatag, KAPPA_CLOCK_GUID, kappaClock);
            flv_write_tag(out, &metatag);
            // hexdump(flvtag_raw_data(&metatag), flvtag_raw_size(&metatag));
            nextClock += (1000 / 60.0);
        }

        if (flvtag_avcpackettype_nalu == flvtag_avcpackettype(&tag) && nextParty <= timestamp) {
            get_dudes(partyDudes);

            if (CAPTION_METHOD == CAPTION_METHOD_SEI_708) {
                flvtag_addcaption_text(&tag, partyDudes);
            } else if (CAPTION_METHOD == CAPTION_METHOD_AMF_708) {
                write_amfcaptions_708(out, nextParty, partyDudes);
            } else if (CAPTION_METHOD == CAPTION_METHOD_AMF_708) {
                write_amfcaptions_utf8(out, nextParty, partyDudes);
            } else {
                fprintf(stderr, "Unknnow method\n");
                return EXIT_FAILURE;
            }

            fprintf(stderr, "%d: %s\n", nextParty, partyDudes);
            nextParty += 500; // party all the time
        }

        flv_write_tag(out, &tag);
    }

    return EXIT_SUCCESS;
}
