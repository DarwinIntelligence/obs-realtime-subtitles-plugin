#ifndef TRANSCRIPTIONPLUGINDATA_H
#define TRANSCRIPTIONPLUGINDATA_H

#include <util/deque.h>
#include <util/darray.h>
#include <media-io/audio-resampler.h>


#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <string>




#define MAX_PREPROC_CHANNELS 10

struct transcript_data {
	obs_source_t *context; // obs filter source (technically, this is a filter. We take the audio, store it, but don't do anything to it here. )
	size_t channels;       // number of channels
	uint32_t sample_rate;  // input sample rate
	// How many input frames (in input sample rate) are needed for the next deepgram frame
	size_t frames;
	// How many frames were processed in the last deepgram frame (this is dynamic)
	size_t last_num_frames;
	// Start begining timestamp in ms since epoch
	uint64_t start_timestamp_ms;
	// Sentence counter for srt
	size_t sentence_number;
	// Minimal subtitle duration in ms
	size_t min_sub_duration;
	// Last time a subtitle was rendered
	uint64_t last_sub_render_time;
	bool cleared_last_sub;

	/* PCM buffers */
	float *copy_buffers[MAX_PREPROC_CHANNELS];
	struct deque info_buffer;
	struct deque input_buffers[MAX_PREPROC_CHANNELS];
	// struct deque whisper_buffer;

	/* Resampler */
	audio_resampler_t *resampler_to_deepgram;
	struct deque resampled_buffer;

	/* whisper */
	// std::string whisper_model_path;
	// struct whisper_context *whisper_context;
	// whisper_full_params whisper_params;

	/* Silero VAD */
	// std::unique_ptr<VadIterator> vad;

	float filler_p_threshold;
	float sentence_psum_accept_thresh;

	bool do_silence;
	bool vad_enabled;
	int log_level = LOG_DEBUG;
	bool log_words;
	bool caption_to_stream;
	bool active = false;
	bool save_to_file = false;
	bool save_srt = false;
	bool truncate_output_file = false;
	bool save_only_while_recording = false;
	bool process_while_muted = false;
	bool rename_file_to_match_recording = false;
	bool translate = false;
	std::string target_lang;
	std::string translation_output;
	bool enable_token_ts_dtw = false;
	std::vector<std::tuple<std::string, std::string>> filter_words_replace;
	bool fix_utf8 = true;
	bool enable_audio_chunks_callback = false;
	bool source_signals_set = false;
	bool initial_creation = true;
	bool partial_transcription = false;
	int partial_latency = 1000;
	bool processed_successfully = false;


	//DeepGram Info
	// std::string

	// Last transcription result
	std::string last_text;
	std::string last_text_translation;

	// Text source to output the subtitles
	std::string text_source_name;
	// Callback to set the text in the output text source (subtitles)
	// std::function<void(const DetectionResultWithText &result)> setTextCallback;
	// Output file path to write the subtitles
	std::string output_file_path;
	// std::string whisper_model_file_currently_loaded;
	// bool whisper_model_loaded_new;

	// Use std for thread and mutex
	std::thread deepgram_thread;

	std::mutex deepgram_buf_mutex;
	std::mutex deepgram_ctx_mutex;
	// std::condition_variable wshiper_thread_cv;

	// translation context
	// struct translation_context translation_ctx;
	// std::string translation_model_index;
	// std::string translation_model_path_external;

	bool buffered_output = false;
	// TokenBufferThread captions_monitor;
	// TokenBufferThread translation_monitor;
	int buffered_output_num_lines = 2;
	int buffered_output_num_chars = 30;
	// TokenBufferSegmentation buffered_output_output_type =
	// 	TokenBufferSegmentation::SEGMENTATION_TOKEN;

	// ctor
	// transcription_filter_data() : whisper_buf_mutex(), whisper_ctx_mutex(), wshiper_thread_cv()
	// {
	// 	// initialize all pointers to nullptr
	// 	for (size_t i = 0; i < MAX_PREPROC_CHANNELS; i++) {
	// 		copy_buffers[i] = nullptr;
	// 	}
	// 	context = nullptr;
	// 	resampler_to_deepgram = nullptr;
	// 	whisper_model_path = "";
	// 	whisper_context = nullptr;
	// 	output_file_path = "";
	// 	whisper_model_file_currently_loaded = "";
	// }
};

struct transcript_audio_info{
	uint32_t frames;
	uint64_t timestamp_offset_ns; // offset since the start of processing in nano seconds.
};

#endif //TRANSCRIPTIONPLUGINDATA_H