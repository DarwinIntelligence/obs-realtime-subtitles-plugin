#include "AudioCapture.h"


struct obs_source_info transcript_plugin_info = {
    .id = "transcript_plugin",
    .type = OBS_SOURCE_TYPE_FILTER,
    .output_flags = OBS_SOURCE_AUDIO,
    .get_name = transcript_plugin_name,
    .create = transcript_plugin_create,
	.destroy = transcript_plugin_destroy,
	.get_defaults = transcript_plugin_defaults,
	.get_properties = transcript_plugin_properties,
	.update = transcript_plugin_update,
	.activate = transcript_plugin_activate,
	.deactivate = transcript_plugin_deactivate,
	.filter_audio = transcript_plugin_filter_audio,
}