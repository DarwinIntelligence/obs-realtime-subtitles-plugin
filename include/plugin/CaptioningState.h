#ifndef CAPTIONINGSTATE_H
#define CAPTIONINGSTATE_H

#include <string>

using namespace std;

struct CaptioningState {
    bool external_is_streaming = false;
    bool external_is_recording = false;
    bool external_is_preview_open = false;
    bool external_is_virtualcam_on = false;
    string external_scene_collection_name = "";

    bool is_captioning = false;
    bool is_captioning_streaming = false;
    bool is_captioning_recording = false;
    bool is_captioning_virtualcam = false;
    bool is_captioning_preview = false;
    bool is_captioning_text_output = false;
    string captioning_scene_collection_name = "";
};

#endif //CAPTIONINGSTATE_H
