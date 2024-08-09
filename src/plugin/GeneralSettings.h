#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

using namespace std;

#include <QObject>

struct GeneralSettings {
    unsigned int audio_source_dropdown_index;
    unsigned int source_language_dropdown_index;
    unsigned int translate_language_dropdown_index;
    unsigned int caption_view_dropdown_index;
    unsigned int profanity_filter_dropdown_index;
    unsigned int output_to_dropdown_index;
    unsigned int caption_timeout_edit_value;
};

#endif //GENERALSETTINGS_H
