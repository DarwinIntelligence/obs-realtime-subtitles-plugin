#ifndef SCENECOLLECTIONSETTINGS_H
#define SCENECOLLECTIONSETTINGS_H

using namespace std;

#include <QObject>
#include "CaptionSourceSettings.h"
#include "SourceCaptionerSettings.h"
#include "TextOutputSettings.h"

struct SceneCollectionSettings {
    CaptionSourceSettings caption_source_settings;
    std::vector<TextOutputSettings> text_outputs;
};


#endif //SCENECOLLECTIONSETTINGS_H
