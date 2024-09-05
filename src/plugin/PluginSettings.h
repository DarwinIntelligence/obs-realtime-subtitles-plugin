#ifndef PLUGINSETTINGS_H
#define PLUGINSETTINGS_H

#include <QObject>
#include <GeneralSettings.h>

struct PluginSettings {
    GeneralSettings general_settings;

    PluginSettings();
};

#endif //PLUGINSETTINGS_H
