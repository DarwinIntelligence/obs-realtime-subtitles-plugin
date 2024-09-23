#ifndef TRANSCRIPTIONPLUGINDATA_H
#define TRANSCRIPTIONPLUGINDATA_H

#include <util/deque.h>
#include <util/darray.h>
#include <media-io/audio-resampler.h>

#include "WebsocketEndpoint.h"
#include "nlohmann/json.hpp"

#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <string>




#define MAX_PREPROC_CHANNELS 10

struct transcript_data {

	
	WebsocketEndpoint *endpoint;
	int endpoint_id;
	std::string api_key = "8bC901-6Tf31a-7g7g7g-df45Te";
	std::string transcript;
	std::string translation;

	bool source_signals_set = false;


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

	

	float filler_p_threshold;
	float sentence_psum_accept_thresh;

	bool do_silence;
	bool caption_to_stream;
	bool active = false;
	bool save_to_file = true;
	bool save_srt = false;
	bool truncate_output_file = false;
	bool save_only_while_recording = false;
	bool process_while_muted = false;
	bool rename_file_to_match_recording = false;
	std::string source_lang = "";
	std::string target_lang = "";
	std::string translation_output;
	std::string url = "";
	bool initial_creation = true;
	bool processed_successfully = false;
	bool filter_created = false;
	bool update_thread = false;
	bool transcribe = false;
	bool translate = false;
	int thread_iter = 0;


	bool continue_deepgram_loop = true;

	//DeepGram Info
	// std::string

	// Text source to output the subtitles
	std::string text_source_name;
	std::string output_file_path;
	std::mutex deepgram_mutex;
	std::thread deepgram_thread;
	std::mutex endpoint_mutex;

	
};

struct transcript_audio_info{
	uint32_t frames;
	uint64_t timestamp_offset_ns; // offset since the start of processing in nano seconds.
};

#endif //TRANSCRIPTIONPLUGINDATA_H