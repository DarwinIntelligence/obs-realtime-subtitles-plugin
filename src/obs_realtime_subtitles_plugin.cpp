#include <QAction>

#include <MainWindow.h>
#include <obs-module.h>
#include <PluginSupport.h>
#include <obs-source.h>
#include <GUIDock.h>
#include <PluginManager.h>
#include "UIConstants.h"

OBS_DECLARE_MODULE();
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-GB");

OBS_MODULE_AUTHOR("DarwinIntelligence");

MainWindow *mainWindow = nullptr;
PluginManager *plugin_manager = nullptr;
GUIDock *dock = nullptr;

void menu_clicked()
{
	obs_log(LOG_INFO, "Main menu clicked");
    if (mainWindow) {
        mainWindow->show_self();
    }
}

bool obs_module_load(void)
{
	obs_log(LOG_INFO, "plugin loaded successfully (version %s)", PLUGIN_VERSION);
	obs_frontend_push_ui_translation(obs_module_get_string);
	mainWindow = new MainWindow();
	QAction *action = (QAction *) obs_frontend_add_tools_menu_qaction(T_WINDOW_TITLE);

    action->connect(action, &QAction::triggered, &menu_clicked);
	obs_frontend_pop_ui_translation();

	dock = new GUIDock();
    obs_frontend_add_dock_by_id("0", T_WINDOW_TITLE, dock);
	return true;
}

void obs_module_unload(void)
{
	obs_log(LOG_INFO, "plugin unloaded");
}
