#include <QAction>

#include <MainWindow.h>
#include <obs-source.h>
#include <obs-module.h>
#include <PluginSupport.h>
#include <GUIDock.h>
#include "transcript_plugin/TranscriptPlugin.h"
#include "transcript_plugin/TranscriptPluginCallbacks.h"
#include "transcript_plugin/TranscriptPluginData.h"
#include "transcript_plugin/TranscriptPluginUtils.h"
#include "UIConstants.h"

OBS_DECLARE_MODULE();
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-GB");

OBS_MODULE_AUTHOR("DarwinIntelligence");

MainWindow *mainWindow = nullptr;
GUIDock *dock = nullptr;

void menu_clicked()
{
	info_log( "Main menu clicked");
    if (mainWindow) {
        mainWindow->show_self();
    }
}

extern struct obs_source_info transcript_plugin_info;

bool obs_module_load(void)
{
	info_log("plugin loaded successfully (version %s)", PLUGIN_VERSION);
	const auto main_window =
		static_cast<QMainWindow *>(obs_frontend_get_main_window());
	obs_frontend_push_ui_translation(obs_module_get_string);

	mainWindow = new MainWindow(main_window);
	QAction *action = (QAction *) obs_frontend_add_tools_menu_qaction(T_WINDOW_TITLE);
	action->connect(action, &QAction::triggered, &menu_clicked);

	dock = new GUIDock(main_window);
	obs_frontend_add_custom_qdock("0", dock);
	obs_register_source(&transcript_plugin_info);
	obs_frontend_pop_ui_translation();
	return true;
}

void obs_module_unload(void)
{
	info_log( "plugin unloaded");
}
