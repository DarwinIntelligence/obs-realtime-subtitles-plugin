#ifndef SOURCECAPTIONERSETTINGS_H
#define SOURCECAPTIONERSETTINGS_H

#include <QObject>

#include "TranscriptOutputSettings.h"
#include "SceneCollectionSettings.h"
#include "CaptionFormatSettings.h"

struct SourceCaptionerSettings {
    bool streaming_output_enabled;
    bool recording_output_enabled;

    TranscriptOutputSettings transcript_settings;

    SceneCollectionSettings scene_collection_settings;

    CaptionFormatSettings format_settings;

    SourceCaptionerSettings();

    const SceneCollectionSettings &get_scene_collection_settings(const string &scene_collection_name);

    void update_setting(const string &scene_collection_name, const SceneCollectionSettings &new_settings);
};

#endif //SOURCECAPTIONERSETTINGS_H