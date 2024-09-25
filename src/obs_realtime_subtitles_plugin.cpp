#include <QAction>

#include <obs-module.h>
#include <obs-source.h>
#include <PluginSupport.h>
#include "transcript_plugin/TranscriptPlugin.h"
#include "transcript_plugin/TranscriptPluginCallbacks.h"
#include "transcript_plugin/TranscriptPluginData.h"
#include "transcript_plugin/TranscriptPluginUtils.h"
#include "UIConstants.h"


OBS_DECLARE_MODULE();
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-GB");

OBS_MODULE_AUTHOR("DarwinIntelligence");

extern struct obs_source_info transcript_plugin_info;

bool obs_module_load(void)
{
	info_log("plugin loaded successfully (version %s)", PLUGIN_VERSION);
	obs_register_source(&transcript_plugin_info);

	info_log("Returning True");
	return true;
}

void obs_module_unload(void)
{
	info_log( "plugin unloaded");
}
