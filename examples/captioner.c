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

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>
#include "caption.h"
#include "flv.h"

char charcode[] = {
    0,   0, '1', '2', '3', '4', '5', '6',  '7', '8', '9',  '0', '-', '=',   0,   0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',  'O', 'P', '[',  ']','\n',   0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`',   0, '\\', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ',', '.', '/',   0, '*',    0, ' ',   0,    0,   0,   0,   0,   0,
};

int data_ready (int fd)
{
    fd_set set;
    struct timeval timeout = {0,0};
    FD_ZERO (&set);
    FD_SET (fd,&set);
    int cnt = select (fd+1, &set, 0, 0, &timeout);
    FD_ZERO (&set);
    return (0 < cnt);
}

#define MAX_CAP_LENGTH (SCREEN_ROWS*SCREEN_COLS)
int main (int argc, char** argv)
{
    int fd;
    ssize_t n;
    flvtag_t tag;
    struct input_event ev;
    int has_audio, has_video;
    const char* dev = argv[1];
    char text[MAX_CAP_LENGTH+1];
    memset (text,0,MAX_CAP_LENGTH+1);

    FILE* flv = flv_open_read ("-");
    FILE* out = flv_open_write ("-");
    fd = open (dev, O_RDONLY);

    if (fd == -1) {
        fprintf (stderr, "Cannot open %s: %s.\n", dev, strerror (errno));
        return EXIT_FAILURE;
    }

    if (!flv_read_header (flv,&has_audio,&has_video)) {
        fprintf (stderr,"%s is not an flv file\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (!flv_write_header (out,has_audio,has_video)) {
        return EXIT_FAILURE;
    }

    flvtag_init (&tag);

    while (flv_read_tag (flv,&tag)) {
        if (flvtag_avcpackettype_nalu == flvtag_avcpackettype (&tag)) {
            if (data_ready (fd)) {
                n = read (fd, &ev, sizeof ev);

                if (n == (ssize_t)-1) {
                    if (errno == EINTR) {
                        continue;
                    } else {
                        break;
                    }
                } else if (n != sizeof ev) {
                    errno = EIO;
                    break;
                }

                int len = strlen (text);
                char c = (EV_KEY == ev.type && 1 == ev.value && ev.code < 64) ? charcode[ev.code] : 0;

                if (0 < len && '\n' == c) {
                    fprintf (stderr,"='%s'\n", text);
                    flvtag_addcaption (&tag, text);
                    memset (text,0,MAX_CAP_LENGTH+1);
                } else if (0 != c && len < MAX_CAP_LENGTH) {
                    text[len] = c;
                }
            }
        }

        flv_write_tag (out,&tag);
    }

    return EXIT_SUCCESS;
}
