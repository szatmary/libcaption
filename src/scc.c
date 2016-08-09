#include "scc.h"
#include "utf8.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FRAME_RATE (1000.0/30)
#define SCCTIME2MS(HH,MM,SS,FF) ((HH*3600.0 + MM*60.0 + SS) * 1000.0) + ( FF * FRAME_RATE )

// 00:00:25:16  9420 9440 aeae ae79 ef75 2068 6176 e520 79ef 75f2 20f2 ef62 eff4 e9e3 732c 2061 6e64 2049 94fe 9723 ea75 73f4 20f7 616e f420 f4ef 2062 e520 61f7 e573 ef6d e520 e96e 2073 7061 e3e5 ae80 942c 8080 8080 942f

double scc_to_708 (const char* line, cea708_t* cea708)
{
    int hh = 0, mm = 0, ss = 0, ff = 0, cc = 0;
    cea708_init (cea708);

    if (4 == sscanf (line, "%2d:%2d:%2d:%2d\t", &hh, &mm, &ss, &ff)) {
        line += 12;

        while (1 == sscanf (line, "%04x ", &cc)) {
            cea708_add_cc_data (cea708, 1, cc_type_ntsc_cc_field_1, cc);
            line += 5;
        }

        return SCCTIME2MS (hh,mm,ss,ff);
    }

    return -1;
}
