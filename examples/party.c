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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flv.h"
#include "sei.h"

#define USE_AMF 1

void get_dudes (char* str)
{
    sprintf (str, " %s%s.%s(-_-)%s.%s(-_-)%s.%s(-_-)%s.%s%s", EIA608_CHAR_EIGHTH_NOTE, EIA608_CHAR_EIGHTH_NOTE,
             ! (rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT,
             ! (rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_LEFT,
             ! (rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT,
             ! (rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_LEFT,
             ! (rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT,
             ! (rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_LEFT,
             EIA608_CHAR_EIGHTH_NOTE, EIA608_CHAR_EIGHTH_NOTE);
}

void write_amfcaptions (FILE* out, uint32_t timestamp, const char* text)
{
    sei_t sei;
    flvtag_t tag;
    sei_message_t* msg;
    caption_frame_t frame;
    sei_init (&sei);
    flvtag_init (&tag);
    caption_frame_init (&frame);
    caption_frame_from_text (&frame, text);
    sei_from_caption_frame (&sei, &frame);
    caption_frame_dump (&frame);

    for (msg = sei_message_head (&sei); msg; msg=sei_message_next (msg),++timestamp) {
        sei_dump (&sei);
        flvtag_amfcaption (&tag,timestamp,msg);
        fprintf (stderr,"Wrote AMF %d\n", (int) flvtag_raw_size (&tag));
        flv_write_tag (out,&tag);
    }

    sei_free (&sei);
    flvtag_free (&tag);
}

int main (int argc, char** argv)
{
    flvtag_t tag;
    uint32_t nextParty = 1000;
    int has_audio, has_video;
    FILE* flv = flv_open_read (argv[1]);
    FILE* out = flv_open_write (argv[2]);
    char partyDudes[64];

    flvtag_init (&tag);

    if (!flv_read_header (flv,&has_audio,&has_video)) {
        fprintf (stderr,"%s is not an flv file\n", argv[1]);
        return EXIT_FAILURE;
    }

    flv_write_header (out,has_audio,has_video);

    while (flv_read_tag (flv,&tag)) {

        if (nextParty <= flvtag_timestamp (&tag)) {
            get_dudes (partyDudes);

            if (USE_AMF) {
                write_amfcaptions (out,nextParty,partyDudes);
            } else {
                flvtag_addcaption (&tag, partyDudes);
            }

            fprintf (stderr,"%d: %s\n",nextParty, partyDudes);
            nextParty += 250; // party all the time
        }

        flv_write_tag (out,&tag);
    }

    return EXIT_SUCCESS;
}
