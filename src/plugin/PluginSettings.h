#ifndef PLUGINSETTINGS_H
#define PLUGINSETTINGS_H

using namespace std;

#include <QObject>
#include <GeneralSettings.h>

struct PluginSettings {
    GeneralSettings general_settings;

    PluginSettings();
};

#endif //PLUGINSETTINGS_H
