#include "scc.h"
#include "utf8.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FRAME_RATE (1000.0/30)
#define SCCTIME2MS(HH,MM,SS,FF) (((HH*3600.0 + MM*60.0 + SS) * 1000.0) + ( FF * FRAME_RATE ))

// 00:00:25:16  9420 9440 aeae ae79 ef75 2068 6176 e520 79ef 75f2 20f2 ef62 eff4 e9e3 732c 2061 6e64 2049 94fe 9723 ea75 73f4 20f7 616e f420 f4ef 2062 e520 61f7 e573 ef6d e520 e96e 2073 7061 e3e5 ae80 942c 8080 8080 942f

int scc_to_608 (const char* line, double* pts, uint16_t* cc, int cc_max)
{
    int cc_count = 0, cc_data = 0, hh = 0, mm = 0, ss = 0, ff = 0;

    // TODO if ';' use 29.79 fps, if ':' use 30 fls
    if (4 == sscanf (line, "%2d:%2d:%2d;%2d", &hh, &mm, &ss, &ff)) {
        (*pts) = SCCTIME2MS (hh,mm,ss,ff); // scc files start at one hour for some reason
        line += 12;

        while (1 == sscanf (line, "%04x ", &cc_data)) {
            line += 5; cc[cc_count] = cc_data; ++cc_count;

            if (cc_count >= cc_max) {
                break;
            }
        }
    }

    return cc_count;
}
