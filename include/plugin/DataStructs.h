#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

using namespace std;

#include <QObject>
#include <QTimer>

Q_DECLARE_METATYPE(std::string)

enum CaptionSourceMuteType {
    CAPTION_SOURCE_MUTE_TYPE_FROM_OWN_SOURCE,
    CAPTION_SOURCE_MUTE_TYPE_ALWAYS_CAPTION,
    CAPTION_SOURCE_MUTE_TYPE_USE_OTHER_MUTE_SOURCE,
};

struct CaptionSourceSettings {
    string caption_source_name;
    CaptionSourceMuteType mute_when;
    string mute_source_name;

    bool operator==(const CaptionSourceSettings &rhs) const {
        return caption_source_name == rhs.caption_source_name &&
               mute_when == rhs.mute_when &&
               mute_source_name == rhs.mute_source_name;
    }

    bool operator!=(const CaptionSourceSettings &rhs) const {
        return !(rhs == *this);
    }

    string active_mute_source_name() const {
        if (mute_when == CAPTION_SOURCE_MUTE_TYPE_USE_OTHER_MUTE_SOURCE)
            return mute_source_name;

        return caption_source_name;
    }

    void print(const char *line_prefix = "") {
        printf("%sCaptionSourceSettings\n", line_prefix);
        printf("%s  caption_source_name: %s\n", line_prefix, caption_source_name.c_str());
        printf("%s  mute_when: %d\n", line_prefix, mute_when);
        printf("%s  mute_source_name: %s\n", line_prefix, mute_source_name.c_str());
    }
};

struct TextOutputSettings {
    bool enabled;
    string text_source_name;
    uint line_length;
    uint line_count;
    bool insert_punctuation;
    CapitalizationType capitalization;

    void print(const char *line_prefix = "") const {
        printf("%sTextOutputSettings\n", line_prefix);
        printf("%s  enabled: %d\n", line_prefix, enabled);
        printf("%s  text_source_name: %s\n", line_prefix, text_source_name.c_str());
        printf("%s  line_length: %d\n", line_prefix, line_length);
        printf("%s  line_count: %d\n", line_prefix, line_count);
        printf("%s  insert_punctuation: %d\n", line_prefix, insert_punctuation);
        printf("%s  capitalization: %d\n", line_prefix, capitalization);
    }

    bool isValid() const {
        return !text_source_name.empty() && line_count && line_length;
    }

    bool isValidEnabled() const {
        return enabled && isValid();
    }

    bool operator==(const TextOutputSettings &rhs) const {
        return enabled == rhs.enabled &&
               text_source_name == rhs.text_source_name &&
               line_length == rhs.line_length &&
               line_count == rhs.line_count &&
               insert_punctuation == rhs.insert_punctuation &&
               capitalization == rhs.capitalization;
    }

    bool operator!=(const TextOutputSettings &rhs) const {
        return !(rhs == *this);
    }
};

struct SceneCollectionSettings {
    CaptionSourceSettings caption_source_settings;
    std::vector<TextOutputSettings> text_outputs;

    bool operator==(const SceneCollectionSettings &rhs) const {
        return caption_source_settings == rhs.caption_source_settings &&
               text_outputs == rhs.text_outputs;
    }

    bool operator!=(const SceneCollectionSettings &rhs) const {
        return !(rhs == *this);
    }
};

struct TranscriptOutputSettings {
    bool enabled;
    string output_path;
    string format;

    string recording_filename_type;
    string recording_filename_custom;
    string recording_filename_exists;

    string streaming_filename_type;
    string streaming_filename_custom;
    string streaming_filename_exists;

    string virtualcam_filename_type;
    string virtualcam_filename_custom;
    string virtualcam_filename_exists;

    uint srt_target_duration_secs;
    uint srt_target_line_length;
    bool srt_add_punctuation;
    bool srt_split_single_sentences;
    CapitalizationType srt_capitalization;

    bool streaming_transcripts_enabled;
    bool recording_transcripts_enabled;
    bool virtualcam_transcripts_enabled;


    TranscriptOutputSettings(bool enabled, const string &outputPath, const string &format,
                             const string &recordingFilenameType,
                             const string &recordingFilenameCustom, const string &recordingFilenameExists,
                             const string &streamingFilenameType, const string &streamingFilenameCustom,
                             const string &streamingFilenameExists, const string &virtualcamFilenameType,
                             const string &virtualcamFilenameCustom, const string &virtualcamFilenameExists,
                             uint srtTargetDurationSecs, uint srtTargetLineLength,
                             bool srtAddPunctuation, bool srtSplitSingleSentences,
                             CapitalizationType srtCapitalization,
                             bool streamingTranscriptsEnabled, bool recordingTranscriptsEnabled,
                             bool virtualcamTranscriptsEnabled)
            : enabled(enabled), output_path(outputPath), format(format),
              recording_filename_type(recordingFilenameType),
              recording_filename_custom(recordingFilenameCustom),
              recording_filename_exists(recordingFilenameExists),
              streaming_filename_type(streamingFilenameType),
              streaming_filename_custom(streamingFilenameCustom),
              streaming_filename_exists(streamingFilenameExists),
              virtualcam_filename_type(virtualcamFilenameType),
              virtualcam_filename_custom(virtualcamFilenameCustom),
              virtualcam_filename_exists(virtualcamFilenameExists),
              srt_target_duration_secs(srtTargetDurationSecs),
              srt_target_line_length(srtTargetLineLength),
              srt_add_punctuation(srtAddPunctuation),
              srt_split_single_sentences(srtSplitSingleSentences),
              srt_capitalization(srtCapitalization),
              streaming_transcripts_enabled(streamingTranscriptsEnabled),
              recording_transcripts_enabled(recordingTranscriptsEnabled),
              virtualcam_transcripts_enabled(virtualcamTranscriptsEnabled) {}


    bool operator==(const TranscriptOutputSettings &rhs) const {
        return enabled == rhs.enabled &&
               output_path == rhs.output_path &&
               format == rhs.format &&
               recording_filename_type == rhs.recording_filename_type &&
               recording_filename_custom == rhs.recording_filename_custom &&
               recording_filename_exists == rhs.recording_filename_exists &&
               streaming_filename_type == rhs.streaming_filename_type &&
               streaming_filename_custom == rhs.streaming_filename_custom &&
               streaming_filename_exists == rhs.streaming_filename_exists &&
               virtualcam_filename_type == rhs.virtualcam_filename_type &&
               virtualcam_filename_custom == rhs.virtualcam_filename_custom &&
               virtualcam_filename_exists == rhs.virtualcam_filename_exists &&
               srt_target_duration_secs == rhs.srt_target_duration_secs &&
               srt_target_line_length == rhs.srt_target_line_length &&
               srt_add_punctuation == rhs.srt_add_punctuation &&
               srt_split_single_sentences == rhs.srt_split_single_sentences &&
               srt_capitalization == rhs.srt_capitalization &&
               streaming_transcripts_enabled == rhs.streaming_transcripts_enabled &&
               recording_transcripts_enabled == rhs.recording_transcripts_enabled &&
               virtualcam_transcripts_enabled == rhs.virtualcam_transcripts_enabled;
    }

    bool operator!=(const TranscriptOutputSettings &rhs) const {
        return !(rhs == *this);
    }

    bool hasBaseSettings() const;

    void print(const char *line_prefix = "") {
        printf("%sTranscriptSettings\n", line_prefix);
        printf("%s  enabled: %d\n", line_prefix, enabled);
        printf("%s  output_path: %s\n", line_prefix, output_path.c_str());
        printf("%s  format: %s\n", line_prefix, format.c_str());

        printf("%s  recording_filename_type: %s\n", line_prefix, recording_filename_type.c_str());
        printf("%s  recording_filename_custom: %s\n", line_prefix, recording_filename_custom.c_str());
        printf("%s  recording_filename_exists: %s\n", line_prefix, recording_filename_exists.c_str());

        printf("%s  streaming_filename_type: %s\n", line_prefix, streaming_filename_type.c_str());
        printf("%s  streaming_filename_custom: %s\n", line_prefix, streaming_filename_custom.c_str());
        printf("%s  streaming_filename_exists: %s\n", line_prefix, streaming_filename_exists.c_str());

        printf("%s  virtualcam_filename_type: %s\n", line_prefix, virtualcam_filename_type.c_str());
        printf("%s  virtualcam_filename_custom: %s\n", line_prefix, virtualcam_filename_custom.c_str());
        printf("%s  virtualcam_filename_exists: %s\n", line_prefix, virtualcam_filename_exists.c_str());

        printf("%s  srt_target_duration_secs: %d\n", line_prefix, srt_target_duration_secs);
        printf("%s  srt_target_line_length: %d\n", line_prefix, srt_target_line_length);
        printf("%s  srt_add_punctuation: %d\n", line_prefix, srt_add_punctuation);
        printf("%s  srt_split_single_sentences: %d\n", line_prefix, srt_split_single_sentences);
        printf("%s  srt_capitalization: %d\n", line_prefix, srt_capitalization);
        printf("%s  streaming_transcripts_enabled: %d\n", line_prefix, streaming_transcripts_enabled);
        printf("%s  recording_transcripts_enabled: %d\n", line_prefix, recording_transcripts_enabled);
        printf("%s  virtualcam_transcripts_enabled: %d\n", line_prefix, virtualcam_transcripts_enabled);
    }
};

struct SourceCaptionerSettings {
    bool streaming_output_enabled;
    bool recording_output_enabled;

    TranscriptOutputSettings transcript_settings;

    SceneCollectionSettings scene_collection_settings;

    CaptionFormatSettings format_settings;
    ContinuousCaptionStreamSettings stream_settings;

    SourceCaptionerSettings();

    SourceCaptionerSettings(
            bool streaming_output_enabled,
            bool recording_output_enabled,

            const TranscriptOutputSettings &transcript_settings,
            const SceneCollectionSettings &scene_collection_settings,
            const CaptionFormatSettings &format_settings,
            const ContinuousCaptionStreamSettings &stream_settings
    ) :
            streaming_output_enabled(streaming_output_enabled),
            recording_output_enabled(recording_output_enabled),

            transcript_settings(transcript_settings),
            scene_collection_settings(scene_collection_settings),
            format_settings(format_settings),
            stream_settings(stream_settings) {}

    bool operator==(const SourceCaptionerSettings &rhs) const {
        return streaming_output_enabled == rhs.streaming_output_enabled &&
               recording_output_enabled == rhs.recording_output_enabled &&

               transcript_settings == rhs.transcript_settings &&
               scene_collection_settings == rhs.scene_collection_settings &&
               format_settings == rhs.format_settings &&
               stream_settings == rhs.stream_settings;
    }


    bool operator!=(const SourceCaptionerSettings &rhs) const {
        return !(rhs == *this);
    }

    void print(const char *line_prefix = "") {
        printf("%sSourceCaptionerSettings\n", line_prefix);
        printf("%s  streaming_output_enabled: %d\n", line_prefix, streaming_output_enabled);
        printf("%s  recording_output_enabled: %d\n", line_prefix, recording_output_enabled);
        printf("%s  Scene Collection Settings:\n", line_prefix);
        scene_collection_settings.caption_source_settings.print((string(line_prefix) + "    ").c_str());
        printf("%s  Text Output Settings (%lu):\n", line_prefix, scene_collection_settings.text_outputs.size());

        int cnt = 0;
        for (const auto &i: scene_collection_settings.text_outputs) {
            printf("%s     #%d:\n", line_prefix, cnt++);
            i.print((string(line_prefix) + "      ").c_str());
        }

        stream_settings.print((string(line_prefix) + "  ").c_str());
        format_settings.print((string(line_prefix) + "  ").c_str());
        transcript_settings.print((string(line_prefix) + "  ").c_str());
    }

    const SceneCollectionSettings &get_scene_collection_settings(const string &scene_collection_name) const {
        return scene_collection_settings;
    }

    void update_setting(const string &scene_collection_name, const SceneCollectionSettings &new_settings) {
        scene_collection_settings = new_settings;
    }

};

struct CaptionOutput {
    shared_ptr<OutputCaptionResult> output_result;
    bool is_clearance;

    CaptionOutput(shared_ptr<OutputCaptionResult> output_result, bool is_clearance) :
            output_result(output_result),
            is_clearance(is_clearance) {};

    CaptionOutput() : is_clearance(false) {}
};

template<typename T>
struct CaptionOutputControl {
    moodycamel::BlockingConcurrentQueue<CaptionOutput> caption_queue;
    volatile bool stop = false;
    T arg;

    CaptionOutputControl(T arg) : arg(arg) {}

    void stop_soon();

    ~CaptionOutputControl();
};

template<typename T>
struct OutputWriter {
    std::recursive_mutex control_change_mutex;
    std::shared_ptr<CaptionOutputControl<T>> control;

    void clear() {
        std::lock_guard<recursive_mutex> lock(control_change_mutex);
        if (control) {
            control->stop_soon();
            control = nullptr;
        }
    }

    void set_control(const std::shared_ptr<CaptionOutputControl<T>> &set_control) {
        std::lock_guard<recursive_mutex> lock(control_change_mutex);
        clear();
        this->control = set_control;
    }

    bool enqueue(const CaptionOutput &output) {
        std::lock_guard<recursive_mutex> lock(control_change_mutex);
        if (control && !control->stop) {
            control->caption_queue.enqueue(output);
            return true;
        }
        return false;
    }
};

enum SourceCaptionerStatusEvent {
    SOURCE_CAPTIONER_STATUS_EVENT_STOPPED,

    SOURCE_CAPTIONER_STATUS_EVENT_STARTED_OK,
    SOURCE_CAPTIONER_STATUS_EVENT_STARTED_ERROR,

    SOURCE_CAPTIONER_STATUS_EVENT_NEW_SETTINGS_STOPPED,

    SOURCE_CAPTIONER_STATUS_EVENT_AUDIO_CAPTURE_STATUS_CHANGE,
};

struct SourceCaptionerStatus {
    // TODO: add starting error msg string
    SourceCaptionerStatusEvent event_type;

    bool settings_changed;
    bool stream_settings_changed;
    SourceCaptionerSettings settings;
    string scene_collection_name;

    audio_source_capture_status audio_capture_status;

    bool active;

    SourceCaptionerStatus(SourceCaptionerStatusEvent eventType, bool settingsChanged, bool streamSettingsChanged,
                          const SourceCaptionerSettings &settings, string scene_collection_name,
                          audio_source_capture_status audioCaptureStatus, bool active)
            : event_type(eventType), settings_changed(settingsChanged), stream_settings_changed(streamSettingsChanged), settings(settings),
              scene_collection_name(scene_collection_name), audio_capture_status(audioCaptureStatus), active(active) {}
};

#endif //DATASTRUCTS_H