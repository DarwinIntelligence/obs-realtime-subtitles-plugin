#ifndef TRANSCRIPTOUTPUTSETTINGS_H
#define TRANSCRIPTOUTPUTSETTINGS_H

using namespace std;

#include <QObject>
#include "Types.h"

struct TranscriptOutputSettings {
    bool enabled;
    string output_path;
    string format;

    string recording_filename_type;
    string recording_filename_custom;
    string recording_filename_exists;

    string streaming_filename_type;
    string streaming_filename_custom;
    string streaming_filename_exists;

    string virtualcam_filename_type;
    string virtualcam_filename_custom;
    string virtualcam_filename_exists;

    uint srt_target_duration_secs;
    uint srt_target_line_length;
    bool srt_add_punctuation;
    bool srt_split_single_sentences;
    CapitalizationType srt_capitalization;

    bool streaming_transcripts_enabled;
    bool recording_transcripts_enabled;
    bool virtualcam_transcripts_enabled;


    TranscriptOutputSettings() {}
};

#endif //TRANSCRIPTOUTPUTSETTINGS_H