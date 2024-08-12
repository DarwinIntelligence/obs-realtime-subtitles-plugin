#include "TranscriptPlugin.h"
#include "TranscriptPluginData.h"
#include "TranscriptPluginCallbacks.h"
#include "TranscriptPluginUtils.h"

#include <obs-module.h>

#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <regex>
#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif


void set_source_signals(transcript_data *audio_data, obs_source_t *parent_source)
{
	signal_handler_t *sh = obs_source_get_signal_handler(parent_source);
	signal_handler_connect(sh, "media_play", media_play_callback, audio_data);
	signal_handler_connect(sh, "media_started", media_started_callback, audio_data);
	signal_handler_connect(sh, "media_pause", media_pause_callback, audio_data);
	signal_handler_connect(sh, "media_restart", media_restart_callback, audio_data);
	signal_handler_connect(sh, "media_stopped", media_stopped_callback, audio_data);
	audio_data->source_signals_set = true;
}

void disconnect_source_signals(transcript_data *audio_data, obs_source_t *parent_source)
{
	signal_handler_t *sh = obs_source_get_signal_handler(parent_source);
	signal_handler_disconnect(sh, "media_play", media_play_callback, audio_data);
	signal_handler_disconnect(sh, "media_started", media_started_callback, audio_data);
	signal_handler_disconnect(sh, "media_pause", media_pause_callback, audio_data);
	signal_handler_disconnect(sh, "media_restart", media_restart_callback, audio_data);
	signal_handler_disconnect(sh, "media_stopped", media_stopped_callback, audio_data);
	audio_data->source_signals_set = false;
}


struct obs_audio_data *transcript_plugin_filter_audio(void *data, struct obs_audio_data *audio)
{
	info_log("filtering audio");
    if (!audio){
        return nullptr;
    }

    if (data == nullptr){
        return audio;
    }

    struct transcript_data *audio_data = static_cast<struct transcript_data *>(data);


    //Init of source sigs
    if (!audio_data->source_signals_set) {
		// obs_filter_get_parent only works in the filter function
		obs_source_t *parent_source = obs_filter_get_parent(audio_data->context);
		if (parent_source != nullptr) {
			set_source_signals(audio_data, parent_source);
		}
	}

    if(!audio_data->active){
        return audio;
    }

    //Check if process while muted. This would be the case if the user doesn't want this audio processed.
    if (!audio_data -> process_while_muted){
        //check if parent is muted
        obs_source_t *parent_source = obs_filter_get_parent(audio_data->context);
		if (parent_source != nullptr && obs_source_muted(parent_source)) {
			// Source is muted, do not process audio
			return audio;
		}    
    }
    {
        // check each channel and push the audio data into 
        for (size_t c = 0; c < audio_data->channels; c++){
            deque_push_back(&audio_data->input_buffers[c], audio->data[c], audio->frames * sizeof(float));
                

        }

        struct transcript_audio_info info = {0};
        info.frames = audio->frames; //number of frames in the audio packet
        info.timestamp_offset_ns = now_ns() - audio_data->start_timestamp_ms * 1000000; //the current offset in ns
        deque_push_back(&audio_data->info_buffer, &info, sizeof(info));//push all of the info into the shared deque so we can use it outside of this function
    }
    
    return audio; //at the end, let the audio pass through to be taken care of by OBS
}

const char *transcript_plugin_name(void *unused)
{
	UNUSED_PARAMETER(unused);
	return MT_("transcript_plugin");
}


void *transcript_plugin_create(obs_data_t *settings, obs_source_t *filter)
{
    info_log("Creating transcript Plugin");

    void *data = bmalloc(sizeof(struct transcript_data));
	struct transcript_data *audio_data = new (data) transcript_data();

    audio_data->channels = audio_output_get_channels(obs_get_audio());
    audio_data->sample_rate = audio_output_get_sample_rate(obs_get_audio());
	audio_data->frames = (size_t)((float)audio_data->sample_rate / (1000.0f / MAX_MS_WORK_BUFFER));
	audio_data->last_num_frames = 0;
	audio_data->min_sub_duration = (int)obs_data_get_int(settings, "min_sub_duration");
	audio_data->last_sub_render_time = now_ms();
	audio_data->log_level = (int)obs_data_get_int(settings, "log_level");
	audio_data->save_srt = obs_data_get_bool(settings, "subtitle_save_srt");
	audio_data->truncate_output_file = obs_data_get_bool(settings, "truncate_output_file");
	audio_data->save_only_while_recording = obs_data_get_bool(settings, "only_while_recording");
	audio_data->rename_file_to_match_recording =
		obs_data_get_bool(settings, "rename_file_to_match_recording");
	audio_data->process_while_muted = obs_data_get_bool(settings, "process_while_muted");
	audio_data->buffered_output = obs_data_get_bool(settings, "buffered_output");

    for (size_t i = 0; i < audio_data->channels; i++) {
		deque_init(&audio_data->input_buffers[i]);
	}
	deque_init(&audio_data->info_buffer);
	deque_init(&audio_data->resampled_buffer);

    // allocate copy buffers
	audio_data->copy_buffers[0] =
		static_cast<float *>(bzalloc(audio_data->channels * audio_data->frames * sizeof(float)));
	if (audio_data->copy_buffers[0] == nullptr) {
		info_log("Failed to allocate copy buffer");
		audio_data->active = false;
		return nullptr;
	}
	for (size_t c = 1; c < audio_data->channels; c++) { // set the channel pointers
		audio_data->copy_buffers[c] = audio_data->copy_buffers[0] + c * audio_data->frames;
	}
	memset(audio_data->copy_buffers[0], 0, audio_data->channels * audio_data->frames * sizeof(float));

	audio_data->context = filter; //technically, this is a filter. We take the audio, store it, but don't do anything to it here. 

	info_log( "channels %d, frames %d, sample_rate %d", (int)audio_data->channels,
		(int)audio_data->frames, audio_data->sample_rate);

	info_log( "setup audio resampler");
	struct resample_info src, dst;
	src.samples_per_sec = audio_data->sample_rate;
	src.format = AUDIO_FORMAT_FLOAT_PLANAR;
	src.speakers = convert_speaker_layout((uint8_t)audio_data->channels);

	dst.samples_per_sec = SAMPLE_RATE;
	dst.format = AUDIO_FORMAT_FLOAT_PLANAR;
	dst.speakers = convert_speaker_layout((uint8_t)1);

	audio_data->resampler_to_whisper = audio_resampler_create(&dst, &src); //RENAME: going to have to rename this later
	if (!audio_data->resampler_to_whisper) {
		info_log( "Failed to create resampler");
		audio_data->active = false;
		return nullptr;
	}

    
	info_log( "clear text source data");
	const char *subtitle_sources = obs_data_get_string(settings, "subtitle_sources");
	if (subtitle_sources == nullptr || strlen(subtitle_sources) == 0 ||
	    strcmp(subtitle_sources, "none") == 0 || strcmp(subtitle_sources, "(null)") == 0) {
		info_log( "Create text source");
		create_obs_text_source_if_needed();
		audio_data->text_source_name = "Darwin Realtime Subtitles";
		obs_data_set_string(settings, "subtitle_sources", "Darwin Realtime Subtitles");
        info_log("Made a text source: Darwin Realtime Subtitles");
	} else {
		// set the text source name
		audio_data->text_source_name = subtitle_sources;
	}
	info_log( "clear paths and whisper context");
	// audio_data->whisper_model_file_currently_loaded = "";
	audio_data->output_file_path = std::string("");
	// audio_data->whisper_model_path = std::string(""); // The update function will set the model path
	// audio_data->whisper_context = nullptr;

	signal_handler_t *sh_filter = obs_source_get_signal_handler(audio_data->context);
	if (sh_filter == nullptr) {
		info_log( "Failed to get signal handler");
		audio_data->active = false;
		return nullptr;
	}

	signal_handler_connect(sh_filter, "enable", enable_callback, audio_data);

	info_log( "run update");
	// get the settings updated on the filter data struct
	transcript_plugin_update(audio_data, settings);

	// handle the event OBS_FRONTEND_EVENT_RECORDING_STARTING to reset the srt sentence number
	// to match the subtitles with the recording
	obs_frontend_add_event_callback(recording_state_callback, audio_data);

	info_log( "filter created.");
	return audio_data;
}

void transcription_filter_destroy(void *data)
{
	struct transcript_data *audio_data =
		static_cast<struct transcript_data *>(data);

	signal_handler_t *sh_filter = obs_source_get_signal_handler(audio_data->context);
	signal_handler_disconnect(sh_filter, "enable", enable_callback, audio_data);

	info_log("filter destroy");
	// shutdown_whisper_thread(audio_data);

	if (audio_data->resampler_to_whisper) {
		audio_resampler_destroy(audio_data->resampler_to_whisper);
	}

	{
		// std::lock_guard<std::mutex> lockbuf(audio_data->whisper_buf_mutex);
		bfree(audio_data->copy_buffers[0]);
		audio_data->copy_buffers[0] = nullptr;
		for (size_t i = 0; i < audio_data->channels; i++) {
			deque_free(&audio_data->input_buffers[i]);
		}
	}
	deque_free(&audio_data->info_buffer);

	deque_free(&audio_data->resampled_buffer);

	// if (audio_data->captions_monitor.isEnabled()) {
	// 	audio_data->captions_monitor.stopThread();
	// }
	// if (audio_data->translation_monitor.isEnabled()) {
	// 	audio_data->translation_monitor.stopThread();
	// }

	bfree(audio_data);
}


void transcript_plugin_defaults(obs_data_t *s)
{
	info_log( "filter defaults");

	obs_data_set_default_bool(s, "buffered_output", false);
	obs_data_set_default_int(s, "buffer_num_lines", 2);
	obs_data_set_default_int(s, "buffer_num_chars_per_line", 30);
	// obs_data_set_default_int(s, "buffer_output_type",
	// 			 (int)TokenBufferSegmentation::SEGMENTATION_TOKEN);

	obs_data_set_default_bool(s, "vad_enabled", true);
	obs_data_set_default_double(s, "vad_threshold", 0.65);
	obs_data_set_default_int(s, "log_level", LOG_DEBUG);
	obs_data_set_default_bool(s, "log_words", false);
	obs_data_set_default_bool(s, "caption_to_stream", false);
	obs_data_set_default_string(s, "whisper_model_path", "Whisper Tiny English (74Mb)");
	obs_data_set_default_string(s, "whisper_language_select", "en");
	obs_data_set_default_string(s, "subtitle_sources", "none");
	obs_data_set_default_bool(s, "process_while_muted", false);
	obs_data_set_default_bool(s, "subtitle_save_srt", false);
	obs_data_set_default_bool(s, "truncate_output_file", false);
	obs_data_set_default_bool(s, "only_while_recording", false);
	obs_data_set_default_bool(s, "rename_file_to_match_recording", true);
	obs_data_set_default_int(s, "min_sub_duration", 3000);
	obs_data_set_default_bool(s, "advanced_settings", false);
	obs_data_set_default_bool(s, "translate", false);
	obs_data_set_default_string(s, "translate_target_language", "__es__");
	obs_data_set_default_bool(s, "translate_add_context", true);
	// obs_data_set_default_string(s, "translate_model", "whisper-based-translation");
	obs_data_set_default_string(s, "translation_model_path_external", "");
	// obs_data_set_default_int(s, "translate_input_tokenization_style", INPUT_TOKENIZAION_M2M100);
	obs_data_set_default_double(s, "sentence_psum_accept_thresh", 0.4);
	obs_data_set_default_bool(s, "partial_group", false);
	obs_data_set_default_int(s, "partial_latency", 1100);

	// // translation options
	// obs_data_set_default_double(s, "translation_sampling_temperature", 0.1);
	// obs_data_set_default_double(s, "translation_repetition_penalty", 2.0);
	// obs_data_set_default_int(s, "translation_beam_size", 1);
	// obs_data_set_default_int(s, "translation_max_decoding_length", 65);
	// obs_data_set_default_int(s, "translation_no_repeat_ngram_size", 1);
	// obs_data_set_default_int(s, "translation_max_input_length", 65);

	// // Whisper parameters
	// obs_data_set_default_int(s, "whisper_sampling_method", WHISPER_SAMPLING_BEAM_SEARCH);
	// obs_data_set_default_string(s, "initial_prompt", "");
	// obs_data_set_default_int(s, "n_threads", 4);
	// obs_data_set_default_int(s, "n_max_text_ctx", 16384);
	// obs_data_set_default_bool(s, "whisper_translate", false);
	// obs_data_set_default_bool(s, "no_context", true);
	// obs_data_set_default_bool(s, "single_segment", true);
	// obs_data_set_default_bool(s, "print_special", false);
	// obs_data_set_default_bool(s, "print_progress", false);
	// obs_data_set_default_bool(s, "print_realtime", false);
	// obs_data_set_default_bool(s, "print_timestamps", false);
	// obs_data_set_default_bool(s, "token_timestamps", false);
	// obs_data_set_default_bool(s, "dtw_token_timestamps", false);
	// obs_data_set_default_double(s, "thold_pt", 0.01);
	// obs_data_set_default_double(s, "thold_ptsum", 0.01);
	// obs_data_set_default_int(s, "max_len", 0);
	// obs_data_set_default_bool(s, "split_on_word", true);
	// obs_data_set_default_int(s, "max_tokens", 0);
	// obs_data_set_default_bool(s, "suppress_blank", false);
	// obs_data_set_default_bool(s, "suppress_non_speech_tokens", true);
	// obs_data_set_default_double(s, "temperature", 0.1);
	// obs_data_set_default_double(s, "max_initial_ts", 1.0);
	// obs_data_set_default_double(s, "length_penalty", -1.0);
}



void transcript_plugin_update(void *data, obs_data_t *s) //Mostly do nothing right now. None of our translation stuff is set up yet.
{
    info_log("Updating the plugin");
	struct transcript_data *audio_data =
		static_cast<struct transcript_data *>(data);
	info_log( "LocalVocal filter update");

	audio_data->log_level = (int)obs_data_get_int(s, "log_level");
	audio_data->vad_enabled = obs_data_get_bool(s, "vad_enabled");
	audio_data->log_words = obs_data_get_bool(s, "log_words");
	audio_data->caption_to_stream = obs_data_get_bool(s, "caption_to_stream");
	audio_data->save_to_file = obs_data_get_bool(s, "file_output_enable");
	audio_data->save_srt = obs_data_get_bool(s, "subtitle_save_srt");
	audio_data->truncate_output_file = obs_data_get_bool(s, "truncate_output_file");
	audio_data->save_only_while_recording = obs_data_get_bool(s, "only_while_recording");
	audio_data->rename_file_to_match_recording = obs_data_get_bool(s, "rename_file_to_match_recording");
	// Get the current timestamp using the system clock
	audio_data->start_timestamp_ms = now_ms();
	audio_data->sentence_number = 1;
	audio_data->process_while_muted = obs_data_get_bool(s, "process_while_muted");
	audio_data->min_sub_duration = (int)obs_data_get_int(s, "min_sub_duration");
	audio_data->last_sub_render_time = now_ms();
	audio_data->partial_transcription = obs_data_get_bool(s, "partial_group");
	audio_data->partial_latency = (int)obs_data_get_int(s, "partial_latency");
	// bool new_buffered_output = obs_data_get_bool(s, "buffered_output");
	// int new_buffer_num_lines = (int)obs_data_get_int(s, "buffer_num_lines");
	// int new_buffer_num_chars_per_line = (int)obs_data_get_int(s, "buffer_num_chars_per_line");
	// TokenBufferSegmentation new_buffer_output_type =
	// 	(TokenBufferSegmentation)obs_data_get_int(s, "buffer_output_type");
	// const char *filter_words_replace = obs_data_get_string(s, "filter_words_replace");
	// if (filter_words_replace != nullptr && strlen(filter_words_replace) > 0) {
	// 	info_log( "filter_words_replace: %s", filter_words_replace);
	// 	// deserialize the filter words replace
	// 	audio_data->filter_words_replace = deserialize_filter_words_replace(filter_words_replace);
	// } else {
	// 	// clear the filter words replace
	// 	audio_data->filter_words_replace.clear();
	// }

	if (audio_data->save_to_file) {
		audio_data->output_file_path = "";
		// set the output file path
		const char *output_file_path = obs_data_get_string(s, "subtitle_output_filename");
		if (output_file_path != nullptr && strlen(output_file_path) > 0) {
			audio_data->output_file_path = output_file_path;
		} else {
			info_log( "output file path is empty, but selected to save");
		}
	}

	
}


void transcription_filter_activate(void *data)
{
	struct transcript_data *audio_data =
		static_cast<struct transcript_data *>(data);
	info_log( "filter activated");
	audio_data->active = true;
}

void transcription_filter_deactivate(void *data)
{
	struct transcript_data *audio_data =
		static_cast<struct transcript_data *>(data);
	info_log( "filter deactivated");
	audio_data->active = false;
}