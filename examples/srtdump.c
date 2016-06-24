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
#include "srt.h"
#include "sei.h"
// #include "sei.h"

#define MAX_SRT_SIZE (10*1024*1024)
#define MAX_READ_SIZE 4096

size_t read_file (FILE* file, utf8_char_t* data, size_t size)
{
    size_t read, totl = 0;

    while (0 < (read = fread (data,1,MAX_READ_SIZE<size?MAX_READ_SIZE:size,file))) {
        totl += read; data += read; size -= read;
    }

    return totl;
}

int main (int argc, char** argv)
{
    srt_t* srt;
    caption_frame_t frame;
    char frame_buf[CAPTION_FRAME_DUMP_BUF_SIZE];

    if (argc < 2) {
        return 0;
    }

    FILE* file = fopen (argv[1],"r");

    if (! file) {
        return 0;
    }

    utf8_char_t* data = (utf8_char_t*) malloc (MAX_SRT_SIZE);
    size_t size = read_file (file,data,MAX_SRT_SIZE);
    srt_t* head = srt_parse (data,size);

    for (srt = head ; srt ; srt = srt->next) {
        caption_frame_init (&frame);
        srt_to_caption_frame (srt,&frame);
        caption_frame_dump (&frame,frame_buf);
        printf ("%s\n%s\n",srt_data (srt),frame_buf);

        // Render to se messages
        sei_t sei;
        size_t sei_size;
        sei_message_t* msg;
        sei_init (&sei);
        sei_from_caption_frame (&sei,&frame);
        // uint8_t* sei_data = sei_render_alloc (&sei, &sei_size);

        for (msg = sei_message_head (&sei) ; msg ; msg = sei_message_next (msg)) {
            uint8_t* data = sei_message_data (msg);
            int size = sei_message_size (msg);
            // printf ("sei_message_size %d ", size);

            for (int i = 0 ; i < size ; ++i) {
                printf ("%02X ", data[i]);
            }

            printf ("\n----------------------------------------------------\n");
        }
    }

    srt_free (head);
}
