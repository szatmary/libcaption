#include "caption.h"
#include "utf8.h"
#include <stdio.h>
#include <string.h>

#define CAPTION_LINE_CHARS (32)
#define CAPTION_LINE_BYTES (4 * CAPTION_LINE_CHARS)
const char* t = "Very_long_string_with_out_spaces_very offen to force_line a split\nat\nobsucre_locations\n\nTwo \\n";

int main(int argc, const char** argv)
{
    const char* data = t;
    size_t bytes, size = strlen(data);

    for (size_t r = 0; (*data) && size && r < SCREEN_ROWS;) {
        // skip whitespace at start of line
        while (size && utf8_codepoint_is_whitespace(data)) {
            size_t s = utf8_codepoint_length(data);
            data += s, size -= s;
        }

        // get charcter count for wrap (or orest of line)
        size_t codepoint_count = utf8_string_wrap_length(data, SCREEN_COLS, &bytes);

        // write to caption frame
        fprintf(stderr, "'%.*s'\n", (int)bytes, data);
        data += bytes, size -= bytes;

        r += codepoint_count ? 1 : 0; // Update row only if not blank
    }
}