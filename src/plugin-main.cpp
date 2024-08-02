#include <countdown-widget.hpp>
#include <obs-module.h>
#include <plugin-support.h>
#include <obs-source.h>

OBS_DECLARE_MODULE();
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US");

OBS_MODULE_AUTHOR("DarwinIntelligence");

CountdownDockWidget *countdownWidget = nullptr;

bool obs_module_load(void)
{
	obs_log(LOG_INFO, "plugin loaded successfully (version %s)",
			PLUGIN_VERSION);
	const auto main_window =
		static_cast<QMainWindow *>(obs_frontend_get_main_window());
	obs_frontend_push_ui_translation(obs_module_get_string);
	countdownWidget = new CountdownDockWidget(main_window);

	const char* test = "0";

	obs_frontend_add_dock_by_id(test, "RealTime Subtitles", countdownWidget);
	obs_frontend_pop_ui_translation();
	return true;
}

void obs_module_post_load(void)
{
	countdownWidget->ConfigureWebSocketConnection();
}


void obs_module_unload(void)
{
	obs_log(LOG_INFO, "plugin unloaded");
}
