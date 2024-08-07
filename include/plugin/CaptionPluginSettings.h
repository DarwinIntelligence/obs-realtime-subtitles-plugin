#ifndef CAPTIONPLUGINSETTINGS_H
#define CAPTIONPLUGINSETTINGS_H

#include "SourceCaptioner.h"

struct CaptionPluginSettings {
    bool enabled;
    SourceCaptionerSettings source_cap_settings;

    CaptionPluginSettings(bool enabled, const SourceCaptionerSettings &source_cap_settings) :
            enabled(enabled),
            source_cap_settings(source_cap_settings) {}

    bool operator==(const CaptionPluginSettings &rhs) const {
        return enabled == rhs.enabled &&
               source_cap_settings == rhs.source_cap_settings;
    }

    bool operator!=(const CaptionPluginSettings &rhs) const {
        return !(rhs == *this);
    }

    void print(const char *line_prefix = "") {
        printf("%sCaptionPluginSettings\n", line_prefix);
        printf("%senabled: %d\n", line_prefix, enabled);
        source_cap_settings.print((string(line_prefix) + "").c_str());
    }
};


#endif //CAPTIONPLUGINSETTINGS_H