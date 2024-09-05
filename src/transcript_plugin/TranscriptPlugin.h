// #ifndef TRANSCRIPTPLUGIN_H
// #define TRANSCRIPTPLUGIN_H



#include <obs-module.h>
// #include "TranscriptPluginData.h"


// #include <thread>
// #include <memory>
// #include <mutex>
// #include <condition_variable>
// #include <functional>
// #include <string>


#ifdef __cplusplus
extern "C" {
#endif

#define MT_ obs_module_text

#define info_log(format, ...) blog(LOG_INFO, format, ##__VA_ARGS__)

#define SAMPLE_RATE 16000
#define MAX_MS_WORK_BUFFER 11000


void transcript_plugin_activate(void *data);
void *transcript_plugin_create(obs_data_t *settings, obs_source_t *filter);
void transcript_plugin_update(void *data, obs_data_t *s);
void transcript_plugin_destroy(void *data);
const char *transcript_plugin_name(void *unused);
struct obs_audio_data *transcript_plugin_filter_audio(void *data, struct obs_audio_data *audio);
void transcript_plugin_deactivate(void *data);
void transcript_plugin_defaults(obs_data_t *s);
obs_properties_t *transcript_plugin_properties(void *data);
int16_t f32_to_i16(float f);


#ifdef __cplusplus
}
#endif
// #endif //TRANSCRIPTPLUGIN_H