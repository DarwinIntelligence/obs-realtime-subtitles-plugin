#include "TranscriptPlugin.h"


struct obs_source_info transcript_plugin_info = {
    .id = "Darwin Translation",
    .type = OBS_SOURCE_TYPE_FILTER,
    .output_flags = OBS_SOURCE_AUDIO,
    .get_name = transcript_plugin_name, //written
    .create = transcript_plugin_create, //written
	.destroy = transcript_plugin_destroy, //written
	.get_defaults = transcript_plugin_defaults,//written
	.get_properties = transcript_plugin_properties, //Written (NOT part of OBS)
	.update = transcript_plugin_update, //written (Nothing really in it now. Most will be translation settings work)
	.activate = transcript_plugin_activate, //written
	.deactivate = transcript_plugin_deactivate, //written
	.filter_audio = transcript_plugin_filter_audio, //written
	.filter_remove = transcript_plugin_remove,
};
