#include <main_window.h>
#include <obs-module.h>
#include <plugin-support.h>
#include <obs-source.h>

OBS_DECLARE_MODULE();
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US");

OBS_MODULE_AUTHOR("DarwinIntelligence");

MainWidget *mainWidget = nullptr;

void menu_clicked();

void menu_clicked() {
    debug_log("Main Menu button clicked ");
    if (mainWidget) {
        mainWidget->show_self();
    }
}


bool obs_module_load(void)
{
	obs_log(LOG_INFO, "plugin loaded successfully (version %s)",
			PLUGIN_VERSION);
	mainWidget = new MainWidget(mainWidget);
	QAction *action = (QAction *) obs_frontend_add_tools_menu_qaction("RealTime Subtitles");
    action->connect(action, &QAction::triggered, &menu_clicked);
	return true;
}

void obs_module_unload(void)
{
	obs_log(LOG_INFO, "plugin unloaded");
}
