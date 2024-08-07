#include <memory>

#include "SourceCaptioner.h"

typedef std::tuple<string, string> TextOutputTup;

void set_text_source_text(const string &text_source_name, const string &caption_text);

SourceCaptioner::SourceCaptioner(const bool enabled, const SourceCaptionerSettings &settings, const string &scene_collection_name, bool start) :
        QObject(),
        base_enabled(enabled),
        settings(settings),
        selected_scene_collection_name(scene_collection_name),
        last_caption_at(std::chrono::steady_clock::now()),
        last_caption_cleared(true) {
}
void SourceCaptioner::stop_caption_stream(bool send_signal);
bool SourceCaptioner::set_settings(const SourceCaptionerSettings &new_settings, const string &scene_collection_name);
bool SourceCaptioner::start_caption_stream(const SourceCaptionerSettings &new_settings, const string &scene_collection_name);
bool SourceCaptioner::_start_caption_stream(bool restart_stream);
void SourceCaptioner::on_audio_capture_status_change_callback(const int id, const audio_source_capture_status status);
void SourceCaptioner::process_audio_capture_status_change(const int cb_audio_capture_id, const int new_status);
void SourceCaptioner::on_audio_data_callback(const int id, const uint8_t *data, const size_t size);
void SourceCaptioner::clear_output_timer_cb();
void SourceCaptioner::store_result(shared_ptr<OutputCaptionResult> output_result);
void SourceCaptioner::prepare_recent(string &recent_captions_output);
void SourceCaptioner::on_caption_text_callback(const CaptionResult &caption_result, bool interrupted);
void SourceCaptioner::process_caption_result(const CaptionResult caption_result, bool interrupted);
void SourceCaptioner::output_caption_writers();
void SourceCaptioner::caption_was_output();
void SourceCaptioner::stream_started_event();
void SourceCaptioner::stream_stopped_event();
void SourceCaptioner::recording_started_event();
void SourceCaptioner::recording_stopped_event();
void SourceCaptioner::virtualcam_started_event();
void SourceCaptioner::virtualcam_stopped_event();
void SourceCaptioner::set_text_source_text(const string &text_source_name, const string &caption_text);
SourceCaptioner::~SourceCaptioner();
template<class T>
void CaptionOutputControl<T>::stop_soon() {
    debug_log("CaptionOutputControl stop_soon()");
    stop = true;
    caption_queue.enqueue(CaptionOutput());
}

template<class T>
CaptionOutputControl<T>::~CaptionOutputControl() {
    debug_log("~CaptionOutputControl");
}

bool TranscriptOutputSettings::hasBaseSettings() const {
    if (!enabled || output_path.empty() || format.empty())
        return false;

    return true;
}