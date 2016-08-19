#include "flv.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <librtmp/rtmp.h>
#include <sys/select.h>

int main (int argc, const char** argv)
{
    FILE* flv;
    RTMP* rtmp;
    RTMPPacket rtmpPacket;

    flvtag_t tag;
    int has_audio, has_video;
    char* url = 0;

    if (2 >= argc) {
        fprintf (stderr,"Usage %s [input] [url]\n",argv[0]);
    }

    url = (char*) argv[2];
    flv = flv_open_read (argv[1]);

    if (! flv) {
        fprintf (stderr,"Could not open %s\n",argv[1]);
        return EXIT_FAILURE;
    }

    if (! flv_read_header (flv, &has_audio, &has_video)) {
        fprintf (stderr,"Not an flv file %s\n",argv[1]);
        return EXIT_FAILURE;
    }

    flvtag_init (&tag);
    rtmp = RTMP_Alloc();
    RTMP_Init (rtmp);
    fprintf (stderr,"Connecting to %s\n", url);
    RTMP_SetupURL (rtmp, url);
    RTMP_EnableWrite (rtmp);

    RTMP_Connect (rtmp, NULL);
    RTMP_ConnectStream (rtmp, 0);
    memset (&rtmpPacket, 0, sizeof (RTMPPacket));

    if (! RTMP_IsConnected (rtmp)) {
        fprintf (stderr,"RTMP_IsConnected() Error\n");
        return EXIT_FAILURE;
    }

    while (flv_read_tag (flv,&tag)) {
        if (! RTMP_IsConnected (rtmp) || RTMP_IsTimedout (rtmp)) {
            fprintf (stderr,"RTMP_IsConnected() Error\n");
            return EXIT_FAILURE;
        }

        RTMP_Write (rtmp, (const char*) flvtag_raw_data (&tag),flvtag_raw_size (&tag));

        // Handle RTMP ping and such
        fd_set sockset; struct timeval timeout = {0,0};
        FD_ZERO (&sockset); FD_SET (RTMP_Socket (rtmp), &sockset);
        register int result = select (RTMP_Socket (rtmp) + 1, &sockset, NULL, NULL, &timeout);

        if (result == 1 && FD_ISSET (RTMP_Socket (rtmp), &sockset)) {
            RTMP_ReadPacket (rtmp, &rtmpPacket);

            if (! RTMPPacket_IsReady (&rtmpPacket)) {
                fprintf (stderr,"Received RTMP packet\n");
                RTMP_ClientPacket (rtmp,&rtmpPacket);
                RTMPPacket_Free (&rtmpPacket);
            }
        }
    }

    return EXIT_SUCCESS;
}
