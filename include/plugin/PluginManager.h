#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "CaptionPluginSettings.h"
#include "SourceCaptioner.h"
#include <PluginSupport.h>

class CaptionPluginManager : public QObject {
Q_OBJECT

public:
    CaptionPluginSettings plugin_settings;
    SourceCaptioner source_captioner;
    CaptioningState state;

private:

    int update_count = 0;

public:
    CaptionPluginManager(const CaptionPluginSettings &initial_settings);

    void external_state_changed(bool is_live, bool is_preview_open, bool is_recording, bool is_virtualcam_on,
                                const string &scene_collection_name);

    void update_settings(const CaptionPluginSettings &new_settings);

    bool toggle_enabled();

    CaptioningState captioning_state();

public slots:

    void set_settings(const CaptionPluginSettings &new_settings);

signals:

    void settings_changed(CaptionPluginSettings new_settings);
};


#endif //PLUGINMANAGER_H
