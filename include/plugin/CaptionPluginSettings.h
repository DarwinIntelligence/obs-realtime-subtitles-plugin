#ifndef CAPTIONPLUGINSETTINGS_H
#define CAPTIONPLUGINSETTINGS_H

#include "SourceCaptionerSettings.h"

struct CaptionPluginSettings {
    bool enabled;
    SourceCaptionerSettings source_cap_settings;

    CaptionPluginSettings();
};


#endif //CAPTIONPLUGINSETTINGS_H
