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

void get_dudes (char* str)
{
    strcpy (str, "XXX(-_-)XXX XXX(-_-)XXX XXX(-_-)XXX");
    memcpy (&str[0], ! (rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT, 3);
    memcpy (&str[8], ! (rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_LEFT, 3);
    memcpy (&str[12], ! (rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT, 3);
    memcpy (&str[20], ! (rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_LEFT, 3);
    memcpy (&str[24], ! (rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT, 3);
    memcpy (&str[32], ! (rand() % 2) ? EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT : EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_LEFT, 3);
}

int main (int argc, char** argv)
{
    flvtag_t tag;
    uint32_t nextParty = 0;
    int has_audio, has_video;
    FILE* flv = flv_open_read (argv[1]);
    FILE* out = flv_open_write (argv[2]);
    char partyDudes[35];

    flvtag_init (&tag);

    if (!flv_read_header (flv,&has_audio,&has_video)) {
        fprintf (stderr,"%s is not an flv file\n", argv[1]);
        return EXIT_FAILURE;
    }

    flv_write_header (out,has_audio,has_video);

    while (flv_read_tag (flv,&tag)) {

        if (nextParty <= flvtag_timestamp (&tag)) {
            get_dudes (partyDudes);
            flvtag_addcaption (&tag, partyDudes);
            fprintf (stderr,"%d: %s\n", flvtag_timestamp (&tag), partyDudes);
            nextParty += 1000; // party every second
        }

        flv_write_tag (out,&tag);
    }

    return EXIT_SUCCESS;
}
