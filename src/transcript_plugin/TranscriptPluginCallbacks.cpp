#ifdef _WIN32
#define NOMINMAX
#endif

#include <obs.h>
#include <obs-frontend-api.h>

#include <curl/curl.h>

#include <fstream>
#include <iomanip>
#include <regex>
#include <string>
#include <vector>

#include "TranscriptPluginCallbacks.h"
#include "TranscriptPluginUtils.h"
#include "TranscriptPlugin.h"

void send_caption_to_source(const std::string &target_source_name, const std::string &caption)
{
	if (target_source_name.empty()) {
		return;
	}
	auto target = obs_get_source_by_name(target_source_name.c_str());
	if (!target) {
		info_log( "text_source target is null");
		return;
	}
	auto text_settings = obs_source_get_settings(target);
	obs_data_set_string(text_settings, "text", caption.c_str());
	obs_source_update(target, text_settings);
	obs_source_release(target);
}

void reset_caption_state(transcript_data *gf_)
{
	// if (gf_->captions_monitor.isEnabled()) {
	// 	gf_->captions_monitor.clear();
	// 	gf_->translation_monitor.clear();
	// }
	send_caption_to_source(gf_->text_source_name, "");
	send_caption_to_source(gf_->translation_output, "");
	// flush the buffer
	{
		// std::lock_guard<std::mutex> lock(gf_->whisper_buf_mutex);
		for (size_t c = 0; c < gf_->channels; c++) {
			if (gf_->input_buffers[c].data != nullptr) {
				deque_free(&gf_->input_buffers[c]);
			}
		}
		if (gf_->info_buffer.data != nullptr) {
			deque_free(&gf_->info_buffer);
		}
		// if (gf_->whisper_buffer.data != nullptr) {
		// 	deque_free(&gf_->whisper_buffer);
		// }
	}
}

void recording_state_callback(enum obs_frontend_event event, void *data)
{
	struct transcript_data *audio_data_ =
		static_cast<struct transcript_data *>(data);
	if (event == OBS_FRONTEND_EVENT_RECORDING_STARTING) {
		if (audio_data_->save_srt && audio_data_->save_only_while_recording) {
			info_log( "Recording started. Resetting srt file.");
			// truncate file if it exists
			std::ofstream output_file(audio_data_->output_file_path,
						  std::ios::out | std::ios::trunc);
			output_file.close();
			audio_data_->sentence_number = 1;
			audio_data_->start_timestamp_ms = now_ms();
            info_log("Here is the output file: %s", audio_data_->output_file_path.c_str());
		}
	} else if (event == OBS_FRONTEND_EVENT_RECORDING_STOPPED) {
		if (audio_data_->save_srt && audio_data_->save_only_while_recording &&
		    audio_data_->rename_file_to_match_recording) {
			info_log( "Recording stopped. Rename srt file.");
			// rename file to match the recording file name with .srt extension
			// use obs_frontend_get_last_recording to get the last recording file name
			std::string recording_file_name = obs_frontend_get_last_recording();
			// remove the extension
			recording_file_name = recording_file_name.substr(
				0, recording_file_name.find_last_of("."));
			std::string srt_file_name = recording_file_name + ".srt";
			// rename the file
			std::rename(audio_data_->output_file_path.c_str(), srt_file_name.c_str());
            info_log("Here is the output file: %s", audio_data_->output_file_path.c_str());

		}
	}
}



void media_play_callback(void *data_, calldata_t *cd)
{
	UNUSED_PARAMETER(cd);
	transcript_data *gf_ = static_cast<struct transcript_data *>(data_);
	info_log( "media_play");
	gf_->active = true;
}

void media_started_callback(void *data_, calldata_t *cd)
{
	UNUSED_PARAMETER(cd);
	transcript_data *gf_ = static_cast<struct transcript_data *>(data_);
	info_log( "media_started");
	gf_->active = true;
	reset_caption_state(gf_);
}

void media_pause_callback(void *data_, calldata_t *cd)
{
	UNUSED_PARAMETER(cd);
	transcript_data *gf_ = static_cast<struct transcript_data *>(data_);
	info_log( "media_pause");
	gf_->active = false;
}

void media_restart_callback(void *data_, calldata_t *cd)
{
	UNUSED_PARAMETER(cd);
	transcript_data *gf_ = static_cast<struct transcript_data *>(data_);
	info_log( "media_restart");
	gf_->active = true;
	reset_caption_state(gf_);
}

void media_stopped_callback(void *data_, calldata_t *cd)
{
	UNUSED_PARAMETER(cd);
	transcript_data *gf_ = static_cast<struct transcript_data *>(data_);
	info_log( "media_stopped");
	gf_->active = false;
	reset_caption_state(gf_);
}

void enable_callback(void *data_, calldata_t *cd)
{
	transcript_data *gf_ = static_cast<struct transcript_data *>(data_);
	bool enable = calldata_bool(cd, "enabled");
	if (enable) {
		info_log("enable_callback: enable");
		gf_->active = true;
		reset_caption_state(gf_);
		// update_whisper_model(gf_);
	} else {
		info_log( "enable_callback: disable");
		gf_->active = false;
		reset_caption_state(gf_);
		// shutdown_whisper_thread(gf_);
	}
}

