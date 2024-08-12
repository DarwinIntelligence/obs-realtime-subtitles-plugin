#ifndef TRANSCRIPT_PLUGIN_CALLBACKS_H
#define TRANSCRIPT_PLUGIN_CALLBACKS_H


#include <string>
#include "TranscriptPluginData.h"
#include <obs-frontend-api.h>


void recording_state_callback(enum obs_frontend_event event, void *data);
void send_caption_to_source(const std::string &target_source_name, const std::string &str_copy);

void media_play_callback(void *data_, calldata_t *cd);
void media_started_callback(void *data_, calldata_t *cd);
void media_pause_callback(void *data_, calldata_t *cd);
void media_restart_callback(void *data_, calldata_t *cd);
void media_stopped_callback(void *data_, calldata_t *cd);
void enable_callback(void *data_, calldata_t *cd);



#endif /*TRANSCRIPT_PLUGIN_CALLBACKS_H*/