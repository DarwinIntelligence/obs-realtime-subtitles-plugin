#ifndef CAPTIONFORMATSETTINGS_H
#define CAPTIONFORMATSETTINGS_H

#include <string>

#include "Types.h"

struct CaptionFormatSettings {
    uint caption_line_length;
    uint caption_line_count;
    CapitalizationType capitalization;
    bool caption_insert_newlines;
    bool caption_insert_punctuation;

    bool caption_timeout_enabled;
    double caption_timeout_seconds;

    CaptionFormatSettings(
            uint caption_line_length,
            uint caption_line_count,
            CapitalizationType capitalization,
            bool caption_insert_newlines,
            bool caption_insert_punctuation,
            bool caption_timeout_enabled,
            double caption_timeout_seconds
    );
};

#endif //CAPTIONFORMATSETTINGS_H