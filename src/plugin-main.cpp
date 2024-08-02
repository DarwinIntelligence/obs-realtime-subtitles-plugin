#include <obs-module.h>
#include <plugin-support.h>
#include <obs-source.h>
#include "qt_gui.h"

OBS_DECLARE_MODULE();
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US");

OBS_MODULE_AUTHOR("DarwinIntelligence");


bool obs_module_load(void)
{
	obs_log(LOG_INFO, "[OBS RealTime Subtitles] plugin loaded successfully (version %s)",
			PLUGIN_VERSION);
	return true;
}

void obs_module_unload(void)
{
	obs_log(LOG_INFO, "plugin unloaded");
}
