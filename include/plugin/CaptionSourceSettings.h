#ifndef CAPTIONSOURCESETTINGS_H
#define CAPTIONSOURCESETTINGS_H

using namespace std;

#include <QObject>

struct CaptionSourceSettings {
    string caption_source_name;
    CaptionSourceMuteType mute_when;
    string mute_source_name;
    string active_mute_source_name() const;
};

#endif //CAPTIONSOURCESETTINGS_H