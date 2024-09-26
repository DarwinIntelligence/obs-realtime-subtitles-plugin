#include <obs-module.h>
#include "PluginSupport.h"
#include "TranscriptPlugin.h"
#include "TranscriptPluginData.h"
#include "TranscriptPluginCallbacks.h"
#include "TranscriptPluginUtils.h"
#include "DeepGramProcessing.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <regex>
#include <string>
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

	// info_log("Heres how many audio frames we got %u", audio->frames);
	// info_log("filtering audio");
    if (!audio){
        return nullptr;
    }

    if (data == nullptr){
        return audio;
    }

	

    struct transcript_data *audio_data = static_cast<struct transcript_data *>(data);


    // //Init of source sigs
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
   

	

	
	
	if(audio_data->endpoint != NULL){
		if (audio != NULL){
			uint16_t *i16_audio = (uint16_t *)malloc(audio->frames * sizeof(float)/2); //I believe it is divided by two because the size of float is 32 bits and we want 16? 
			for (u_int i = 0; i < audio->frames; i++) {
					float sample_float;
					memcpy(&sample_float,
						&audio->data[0]
								[0 + i * sizeof(float)],
						sizeof(float));
					i16_audio[i] = f32_to_i16(sample_float);
				}
			//Need to lock this so that it doesn't get used in two threads at once
			audio_data->endpoint->send_binary(
				audio_data->endpoint_id, i16_audio, audio->frames * sizeof(float) / 2);
			free(i16_audio);
		
		}
	}
    
    return audio; //at the end, let the audio pass through to be taken care of by OBS
}

const char *transcript_plugin_name(void *unused)
{
	UNUSED_PARAMETER(unused);
	return MT_("Darwin Translation");
}


void *transcript_plugin_create(obs_data_t *settings, obs_source_t *filter)
{
	info_log("Here is the new thread id: %zu", std::hash<std::thread::id>{}(std::this_thread::get_id()));

    info_log("Creating transcript Plugin");

    void *data = bmalloc(sizeof(struct transcript_data));
	struct transcript_data *audio_data = new (data) transcript_data();

    audio_data->channels = audio_output_get_channels(obs_get_audio());
    audio_data->sample_rate = audio_output_get_sample_rate(obs_get_audio());
	audio_data->frames = (size_t)((float)audio_data->sample_rate / (1000.0f / MAX_MS_WORK_BUFFER));
	audio_data->last_num_frames = 0;
	audio_data->min_sub_duration = (int)obs_data_get_int(settings, "min_sub_duration");
	audio_data->last_sub_render_time = now_ms();
	audio_data->endpoint = NULL;
	audio_data->source_lang = obs_data_get_string(settings, "source_language");
	audio_data->target_lang = obs_data_get_string(settings, "target_language");
	audio_data->translate = obs_data_get_bool(settings, "translate");
	audio_data->transcribe = obs_data_get_bool(settings, "transcribe");
	audio_data->rename_file_to_match_recording =
		obs_data_get_bool(settings, "rename_file_to_match_recording");
	audio_data->process_while_muted = obs_data_get_bool(settings, "process_while_muted");
	

	audio_data->context = filter; //technically, this is a filter. We take the audio, store it, but don't do anything to it here. 
	
	audio_data->text_source_name = create_obs_text_source_if_needed();
	obs_data_set_string(settings, audio_data->text_source_name.c_str(), audio_data->text_source_name.c_str());


	info_log( "channels %d, frames %d, sample_rate %d", (int)audio_data->channels,
		(int)audio_data->frames, audio_data->sample_rate);

	info_log( "setup audio resampler");
	struct resample_info src, dst;
	src.samples_per_sec = audio_data->sample_rate;
	src.format = AUDIO_FORMAT_FLOAT_PLANAR;
	src.speakers = convert_speaker_layout((uint8_t)audio_data->channels);

	dst.samples_per_sec = SAMPLE_RATE;
	dst.format = AUDIO_FORMAT_16BIT;
	dst.speakers = convert_speaker_layout((uint8_t)1);

	

    
	info_log( "clear text source data");


	// const char *subtitle_sources = obs_data_get_string(settings, "subtitle_sources");
	
	// if (subtitle_sources == nullptr || strlen(subtitle_sources) == 0 ||
	//     strcmp(subtitle_sources, "none") == 0 || strcmp(subtitle_sources, "(null)") == 0) {
	// 	info_log( "Create text source");
	// 	create_obs_text_source_if_needed();
	// 	audio_data->text_source_name = "Darwin Realtime Subtitles";
	// 	obs_data_set_string(settings, "subtitle_sources", "Darwin Realtime Subtitles");
    //     info_log("Made a text source: Darwin Realtime Subtitles");
	// } else {
	// 	// set the text source name
	// 	audio_data->text_source_name = subtitle_sources;
	// }

	audio_data->output_file_path = std::string("");


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
	audio_data->filter_created = true;

	// handle the event OBS_FRONTEND_EVENT_RECORDING_STARTING to reset the srt sentence number
	// to match the subtitles with the recording
	obs_frontend_add_event_callback(recording_state_callback, audio_data);

	info_log( "filter created.");
	return audio_data;
}

void transcript_plugin_destroy(void *data)
{
	struct transcript_data *audio_data =
		static_cast<struct transcript_data *>(data);


	signal_handler_t *sh_filter = obs_source_get_signal_handler(audio_data->context);
	signal_handler_disconnect(sh_filter, "enable", enable_callback, audio_data);
	
	info_log("filter destroy");
	info_log("shutting down deepgram thread. PLUGIN DESTROY");
	audio_data->continue_deepgram_loop = false;
	if(audio_data->deepgram_thread.joinable()){
		info_log("Trying to join threads");
		audio_data->deepgram_thread.join();
	}
	// obs_source_t *text = obs_get_source_by_name(audio_data->text_source_name.c_str());
	// obs_source_release(text);
	// obs_source_remove(text);//get rid of the subtitle source so another can be made when it is made again.
	remove_sources();

	info_log("Done PLUGIN DESTROY");

	

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


	obs_data_set_default_string(s, "source_language", "ja");
	obs_data_set_default_string(s, "target_language", "ko");
	obs_data_set_default_bool(s,"translate", false);
	obs_data_set_default_bool(s,"transcribe", false);




}



void transcript_plugin_update(void *data, obs_data_t *s) //Mostly do nothing right now. None of our translation stuff is set up yet.
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100));//to stop the 
    info_log("Updating the plugin");
	struct transcript_data *audio_data =
		static_cast<struct transcript_data *>(data);
	info_log( "Darwin Realtime filter update");

	// audio_data->log_level = (int)obs_data_get_int(s, "log_level");
	// audio_data->vad_enabled = obs_data_get_bool(s, "vad_enabled");
	// audio_data->log_words = obs_data_get_bool(s, "log_words");
	audio_data->caption_to_stream = obs_data_get_bool(s, "caption_to_stream");
	audio_data->save_to_file = obs_data_get_bool(s, "file_output_enable");
	audio_data->save_srt = obs_data_get_bool(s, "subtitle_save_srt");
	audio_data->truncate_output_file = obs_data_get_bool(s, "truncate_output_file");
	audio_data->save_only_while_recording = obs_data_get_bool(s, "only_while_recording");
	audio_data->rename_file_to_match_recording = obs_data_get_bool(s, "rename_file_to_match_recording");
	// Get the current timestamp using the system clock
	audio_data->sentence_number = 1;
	audio_data->process_while_muted = obs_data_get_bool(s, "process_while_muted");
	audio_data->min_sub_duration = (int)obs_data_get_int(s, "min_sub_duration");
	audio_data->last_sub_render_time = now_ms();
	// audio_data->partial_transcription = obs_data_get_bool(s, "partial_group");
	// audio_data->partial_latency = (int)obs_data_get_int(s, "partial_latency");
	info_log("Source Lang %s New Lang %s", audio_data->source_lang.c_str(), obs_data_get_string(s, "source_language"));
	// if(audio_data->text_source_name != ""){//fi we have a text source, get rid of it so we can make a new one
	// 	info_log("Not an empty subtitle string. %s", audio_data->text_source_name.c_str());
	// 	obs_source_t *text = obs_get_source_by_name(audio_data->text_source_name.c_str());
	// 	obs_source_release(text);
	// 	obs_source_remove(text);
	// }
		
	// audio_data->text_source_name = create_obs_text_source_if_needed();
	// obs_data_set_string(s, audio_data->text_source_name.c_str(), audio_data->text_source_name.c_str());
	if (audio_data->source_lang != obs_data_get_string(s, "source_language") || audio_data->target_lang != obs_data_get_string(s, "target_language") || audio_data->translate != obs_data_get_bool(s, "translate") || audio_data->transcribe != obs_data_get_bool(s, "transcribe")){
		info_log("Changed settings");
		audio_data->source_lang = obs_data_get_string(s, "source_language");
		audio_data->target_lang = obs_data_get_string(s, "target_language");
		audio_data->translate = obs_data_get_bool(s, "translate");
		audio_data->transcribe = obs_data_get_bool(s, "transcribe");
		audio_data->url = "wss://translate.darwinai.link/listen?client_id=1111&source=" + audio_data->source_lang + "&target=" + audio_data->target_lang;
		audio_data-> update_thread = true;
		audio_data->continue_deepgram_loop = false; //so we can stop the loop
		info_log("setting change joining threads");
		if(audio_data->deepgram_thread.joinable()){
			audio_data->deepgram_thread.join();
		}

	}

	
	
	
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
	
	if (!(audio_data->filter_created)) {
		info_log("Initial filter creation and source enabled");
		
		// source was enabled on creation
		std::thread new_deepgram_thread(deepgram_loop,audio_data);
		audio_data->deepgram_thread.swap(new_deepgram_thread);
		
		audio_data->active = true;
		audio_data->initial_creation = false;
	} 
	else{
		info_log("In the else");
		if(audio_data->update_thread){
			if(!(audio_data->deepgram_thread.joinable())){//if the thread isn't joinable, then there isn't a new thread running and we should run one
				info_log("Creating new deepgram thread");
				audio_data->update_thread = false;
				std::thread new_deepgram_thread(deepgram_loop,audio_data);

				audio_data->deepgram_thread.swap(new_deepgram_thread);

				
			}
		}
	}
	

	
}

obs_properties_t *transcript_plugin_properties(void *data){
	
	
	info_log("Running transcript_plugin_properties.");

		
	obs_properties_t *ppts = obs_properties_create();


	// instead of starting with these text properties, try adding them later with "obs_properties_add_group"
	// obs_properties_t *properties = obs_source_properties(dgsd->text_source);

	// TODO: add a drop-down for language
	obs_property_t *source_language = obs_properties_add_list(ppts, "source_language", MT_("The Langauge being Spoken"),
										OBS_COMBO_TYPE_LIST,
										OBS_COMBO_FORMAT_STRING);
	//add the languages to the list
	obs_property_list_add_string(source_language, MT_("日本語"), "ja");
	obs_property_list_add_string(source_language, MT_("한국어"), "ko");
	obs_property_list_add_string(source_language, MT_("日语"), "zh");

	obs_property_t *target = obs_properties_add_list(ppts, "target_language", MT_("Translate to this language"),
										OBS_COMBO_TYPE_LIST,
										OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(target, MT_("한국어"), "ko");
	obs_property_list_add_string(target, MT_("Japanese - 日本語"), "ja");
	obs_property_list_add_string(target, MT_("Chinese - 日语"), "zh");

	obs_properties_add_bool(ppts, MT_("translate"), "Translate");
	obs_properties_add_bool(ppts, MT_("transcribe"), "Transcribe");



	UNUSED_PARAMETER(data);
	info_log("End of get properties");
	return ppts;
}

void transcript_plugin_activate(void *data)
{
	struct transcript_data *audio_data =
		static_cast<struct transcript_data *>(data);
	info_log( "filter activated");
	audio_data->active = true;
}

void transcript_plugin_deactivate(void *data)
{
	struct transcript_data *audio_data =
		static_cast<struct transcript_data *>(data);
	info_log( "filter deactivated");
	
	// obs_source_t *text = obs_get_source_by_name(audio_data->text_source_name.c_str());
	// obs_source_release(text);
	// obs_source_remove(text);
	remove_sources();
	audio_data->active = false;
}

void transcript_plugin_remove(void *data, obs_source_t *source)
{
	struct transcript_data *df =
		static_cast<struct transcript_data *>(data);

	info_log("filter remove");

	disconnect_source_signals(df, source);
	// obs_source_t *text = obs_get_source_by_name(df->text_source_name.c_str());
	// obs_source_release(text);
	// obs_source_remove(text);
	remove_sources();
	info_log("filter remove done");
}

int16_t f32_to_i16(float f)
{
	f = f * 32768;
	if (f > 32767) {
		return 32767;
	}
	if (f < -32768) {
		return -32768;
	}
	return (int16_t)f;
}


