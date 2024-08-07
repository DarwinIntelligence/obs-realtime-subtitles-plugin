#ifndef SOURCECAPTIONERSETTINGS_H
#define SOURCECAPTIONERSETTINGS_H

#include "SourceCaptionerSettings.h"
#include "Types.h"

struct SourceCaptionerStatus {
    // TODO: add starting error msg string
    SourceCaptionerStatusEvent event_type;

    bool settings_changed;
    bool stream_settings_changed;
    SourceCaptionerSettings settings;
    string scene_collection_name;

    audio_source_capture_status audio_capture_status;

    bool active;

    SourceCaptionerStatus(SourceCaptionerStatusEvent eventType, bool settingsChanged, bool streamSettingsChanged,
                          const SourceCaptionerSettings &settings, string scene_collection_name,
                          audio_source_capture_status audioCaptureStatus, bool active);
};

#endif //SOURCECAPTIONERSETTINGS_H