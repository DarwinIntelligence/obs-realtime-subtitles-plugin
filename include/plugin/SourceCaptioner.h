#ifndef SOURCECAPTIONER_H
#define SOURCECAPTIONER_H

#include "SourceAudioCaptureSession.h"
#include "DataStructs.h"

#include <QObject>
#include <QTimer>

typedef unsigned int uint;
using namespace std;

Q_DECLARE_METATYPE(std::string)

Q_DECLARE_METATYPE(shared_ptr<OutputCaptionResult>)

Q_DECLARE_METATYPE(CaptionResult)

#define MAX_HISTORY_VIEW_LENGTH 1000
#define HISTORY_ENTRIES_LOW_WM 40
#define HISTORY_ENTRIES_HIGH_WM 80

Q_DECLARE_METATYPE(std::shared_ptr<SourceCaptionerStatus>)

class SourceCaptioner : public QObject {
Q_OBJECT

    bool base_enabled;
    std::unique_ptr<SourceAudioCaptureSession> source_audio_capture_session;
    std::unique_ptr<OutputAudioCaptureSession> output_audio_capture_session;
    std::unique_ptr<ContinuousCaptions> continuous_captions;
    uint audio_chunk_count = 0;

    SourceCaptionerSettings settings;
    string selected_scene_collection_name;

    std::unique_ptr<CaptionResultHandler> caption_result_handler;
    std::recursive_mutex settings_change_mutex;

    std::chrono::steady_clock::time_point last_caption_at;
    bool last_caption_cleared;
    QTimer timer;

    std::vector<std::shared_ptr<OutputCaptionResult>> results_history; // final ones + last ones before interruptions
    std::shared_ptr<OutputCaptionResult> held_nonfinal_caption_result;

    OutputWriter<int> streaming_output;
    OutputWriter<int> recording_output;

    OutputWriter<TranscriptOutputSettings> transcript_streaming_output;
    OutputWriter<TranscriptOutputSettings> transcript_recording_output;
    OutputWriter<TranscriptOutputSettings> transcript_virtualcam_output;

    std::tuple<string, string> last_text_source_set;

    int audio_capture_id = 0;

    string last_caption_text;
    bool last_caption_final = false;

    void caption_was_output();

    void output_caption_writers(
            const CaptionOutput &output,
            bool to_stream,
            bool to_recoding,
            bool to_transcript_streaming,
            bool to_transcript_recording,
            bool to_transcript_virtualcam,
            bool is_clearance
    );

    void store_result(shared_ptr<OutputCaptionResult> output_result);

    void prepare_recent(string &recent_captions_output);

    void on_audio_data_callback(const int id, const uint8_t *data, const size_t size);

    void on_audio_capture_status_change_callback(const int id, const audio_source_capture_status status);

    void on_caption_text_callback(const CaptionResult &caption_result, bool interrupted);

    bool _start_caption_stream(bool restart_stream);

    void process_caption_result(const CaptionResult, bool interrupted);

    void process_audio_capture_status_change(const int id, const int new_status);

    void set_text_source_text(const string &text_source_name, const string &caption_text);

private slots:

    void clear_output_timer_cb();

signals:

    void received_caption_result(const CaptionResult, bool interrupted);

    void caption_result_received(
            shared_ptr<OutputCaptionResult> caption,
            bool cleared,
            string recent_caption_text);

    void audio_capture_status_changed(const int id, const int new_status);

    void source_capture_status_changed(shared_ptr<SourceCaptionerStatus> status);

public:

    SourceCaptioner(const bool enabled, const SourceCaptionerSettings &settings, const string &scene_collection_name, bool start);

    ~SourceCaptioner();

    bool set_settings(const SourceCaptionerSettings &new_settings, const string &scene_collection_name);

    bool start_caption_stream(const SourceCaptionerSettings &new_settings, const string &scene_collection_name);

    void stop_caption_stream(bool send_signal = true);


    void stream_started_event();

    void stream_stopped_event();

    void recording_started_event();

    void recording_stopped_event();

    void virtualcam_started_event();

    void virtualcam_stopped_event();

    void set_enabled(const bool enabled) {
        base_enabled = enabled;
    }

};


#endif //SOURCECAPTIONER_H
